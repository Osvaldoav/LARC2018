import cv2
import numpy as np

# Color ranges
lower_range_g = np.array([23,20,10], dtype=np.uint8)
upper_range_g = np.array([90,255,255], dtype=np.uint8)
lower_range_b = np.array([91,20,10], dtype=np.uint8)
upper_range_b = np.array([150,255,255], dtype=np.uint8)
lower_range_r = np.array([0, 50, 20], dtype=np.uint8)
upper_range_r = np.array([22, 255, 255], dtype=np.uint8)
lower_range_r2 = np.array([151,50,20], dtype=np.uint8)
upper_range_r2 = np.array([255,255,255], dtype=np.uint8)

# Returns from a single image three color masks: red, green and blue
def getMasks(image):
	hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

	mask_blue = cv2.inRange(hsv, lower_range_b, upper_range_b)
	mask_green = cv2.inRange(hsv, lower_range_g, upper_range_g)
	mask_red = cv2.inRange(hsv, lower_range_r, upper_range_r)
	mask_red2 = cv2.inRange(hsv, lower_range_r2, upper_range_r2)
	mask_red = mask_red + mask_red2

	return mask_red, mask_green, mask_blue

# Splits a mask in six different smaller images at a specific position
def checking(mask):
	height, width = mask.shape[:2]
	size_h = int(height*0.18)
	size_w = int(width*0.18)

	start_row, start_col = int(40), int(75)
	end_row, end_col = size_h, size_w

	list_rows = []

	for col in range(2):
		list_cols = []
		for row in range(3):
			end_col = start_col + size_w
			cnt = np.array([[start_col,start_row],[start_col,start_row+size_h],[start_col+size_w,start_row+size_h],[start_col+size_w,start_row]], dtype=np.int32)
			cv2.drawContours(mask, [cnt], -1, (255,0,0), 2)
			cv2.imshow('Image', mask)			
			cv2.waitKey(0)
			cropped = mask[start_row:end_row , start_col:end_col]
			list_cols.append(cropped)
			start_col += size_w + int(width*0.13)
		list_rows.append(list_cols)
		start_col, end_col = int(75), start_col + size_w
		start_row, end_row = height-size_h-40, height

	# return list_rows


# Splits a mask in six different smaller images at a specific position
def splitMask(mask, left_cam):
	height, width = mask.shape[:2]
	size_h = int(height*0.18)
	size_w = int(width*0.18)

	start_row, start_col = int(40), int(75)
	end_row, end_col = size_h, size_w

	list_rows = []

	for col in range(2):
		list_cols = []
		for row in range(3):
			end_col = start_col + size_w
			cropped = mask[start_row:end_row , start_col:end_col]
			list_cols.append(cropped)
			start_col += size_w + int(width*0.13)
		list_rows.append(list_cols)
		start_col, end_col = int(75), start_col + size_w
		start_row, end_row = height-size_h-40, height

	if left_cam:
		for i in range(2):
			temp = list_rows[i][0]
			list_rows[i][0] = list_rows[i][2]
			list_rows[i][2] = temp
		temp = list_rows[0]
		list_rows[0] = list_rows[1] 
		list_rows[1] = temp

	return list_rows

# Returns the biggest contour found in a single image/mask
def getContour(img):
	contours, hierarchy = cv2.findContours(img.copy(), cv2.RETR_LIST, cv2.CHAIN_APPROX_NONE)
	if len(contours) > 0:
		sorted_contours = sorted(contours, key=cv2.contourArea, reverse=True)
		return sorted_contours[0]
	else:
		return np.array([[1,1],[1,2],[2,2]], dtype=np.int32)

# Returns 'R','G' or 'B' depending on which color_contour has the biggest area
def getMaxColor(red_cnt, green_cnt, blue_cnt):
	red_area, green_area, blue_area = cv2.contourArea(red_cnt), cv2.contourArea(green_cnt), cv2.contourArea(blue_cnt)
	if red_area > green_area and red_area > blue_area:
		return 'R'
	elif green_area > blue_area:
		return 'G'
	else:
		return 'B'

# Returns a matrix 2x3 with the color of each container as 'R','G' or 'B'
def getMatrix(image, left_cam):
	matrix = [['X' for j in range(3)] for i in range(2)]

	red_mask, green_mask, blue_mask = getMasks(image)
	masks = []
	masks.append(splitMask(red_mask, left_cam))
	masks.append(splitMask(green_mask, left_cam))
	masks.append(splitMask(blue_mask, left_cam))

	_range = range(1, -1, -1)
	for c, s in enumerate(_range):
		for b in range(3):
			matrix[s][b] = getMaxColor(getContour(masks[0][c][b]), getContour(masks[1][c][b]), getContour(masks[2][c][b]))

	return matrix


image = cv2.imread('../borrar/Image 2.png')
cv2.imshow('Image', image)
checking(image)
cv2.waitKey(0)