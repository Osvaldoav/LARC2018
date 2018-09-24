import cv2

class Cam:
	# Port depends on how many cameras are already in used
	def __init__(self, port):
		self.port = port
		self.cam = cv2.VideoCapture(port)
		ret, self.image = self.cam.read()

	# Takes a picture, saves it into the images folder and updates self.image
	def shoot(self):
		for i in range(3):
			ret, self.image = self.cam.read()

		cv2.imwrite("./images/Image %s.png"%(self.port), self.image)

	# Shows the image taken with a waitKey()
	def show(self):
		cv2.imshow("Image %s"%(self.port), self.image)
		cv2.waitKey(0)

	# Returns the image
	def getImage(self):
		return self.image

	# Releases the camera
	def release(self):
		self.cam.release()