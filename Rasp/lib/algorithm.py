import sys
sys.path.insert(0, '/home/pi/Documents/LARC2018/Rasp/lib/')
import stacks

BLUE_ORDER = [7,4,6,5,3,0,2,1]
GREEN_ORDER = [0,3,1,2,4,7,5,6]
R1_ORDER = [1,2,0,3,5,6,4,7]
R2_ORDER = [6,5,7,4,2,1,3,0]

class Algorithm:

	def __init__(self):
		self.matrix = [['X' for j in range(3)] for i in range(8)]
		self.stacks = [(i, 3) for i in range(8)]
		self.blues, self.greens, self.reds = 0, 0, 0

	# Updates the stacks matrix with the corresponding color regarding the image and section
	def updateContainers(self, image, section, left_cam):
		_stacks = stacks.getMatrix(image, left_cam)

		for c, s in enumerate(_stacks):
			for b in range(3):
				self.matrix[section*2 + c][b] = s[b]

	# Updates the order regarding last_color
	def updateOrder(self):
		if self.last_color == 'B':
			order = BLUE_ORDER
		elif self.last_color == 'G':
			order = GREEN_ORDER
		elif self.last_color == 'R1':
			order = R1_ORDER
		else:
			order = R2_ORDER

		self.stacks = sorted(self.stacks, key = lambda x: -x[1])
		for i in range(3,0,-1):
			x1 = [c for c,index in enumerate(self.stacks) if index[1] == i]
			if len(x1) < 1:
				continue
			x1 = x1[0]
			x2 = x1 + len([index for index in self.stacks if index[1] == i])
			for x in range(x2-x1-1):
				for y in range(x1, x2-1):
					t1 = [c for c,index in enumerate(order) if index == self.stacks[y][0]][0]
					t2 = [c for c,index in enumerate(order) if index == self.stacks[y+1][0]][0]
					if t1 > t2:
						temp = self.stacks[y]
						self.stacks[y] = self.stacks[y+1]
						self.stacks[y+1] = temp
				x2 -= 1
		# if self.last_color == 'B' or self.last_color == 'R2':
		# 	self.stacks = sorted(self.stacks, key=lambda x: (-x[1], -x[0]))
		# else:
		# 	self.stacks = sorted(self.stacks, key=lambda x: (-x[1], x[0]))

	# Remove the top container of a specific stack
	def popContainer(self, pair):
		if pair[1] == 'G':
			self.greens += 1
		elif pair[1] == 'B':
			self.blues += 1

		self.last_color = pair[1]
		self.matrix[pair[0]].pop(0)
		x = [c for c,i in enumerate(self.stacks) if i[0] == pair[0]][0]
		self.stacks[x] = (self.stacks[x][0], self.stacks[x][1]-1)

	# Return stack 6 or 7 as first pick
	def firstPick(self):
		if self.matrix[7][0] == 'R':
			return 7, 'R'
		elif self.matrix[6][0] == 'R':
			return 6, 'R'
		elif self.matrix[7][0] == 'G':
			return 7, 'G'
		elif self.matrix[6][0] == 'G':
			return 6, 'G'
		else:
			return 7, 'B'

	# Return stack 2 or 3 as second pick
	def secondPick(self):
		color1 = 'B' if self.blues % 5 > self.greens % 5 else 'G'
		color2 = 'B' if color1 == 'G' else 'G'

		if self.matrix[3][0] == 'R':
			return 3, 'R'
		elif self.matrix[2][0] == 'R':
			return 2, 'R'
		elif self.matrix[3][0] == color1:
			return 3, color1
		elif self.matrix[2][0] == color1:
			return 2, color1
		else:
			return 3, color2

	# Returns the next stack and color to pick
	def solve(self):
		self.updateOrder()

		print self.stacks

		# Sets the color we are trying to find
		if self.greens % 5 == 0 and self.greens > 0:
			color = 'B'
		elif self.blues % 5 == 0 and self.blues > 0:
			color = 'G'
		else:
			color = 'B' if self.blues % 5 > self.greens % 5 else 'G'

		# Looks for the first red colored container in the corresponding order
		for s in self.stacks:
			if len(self.matrix[s[0]]) > 0:
				if self.matrix[s[0]][0] == 'R':
					return s[0], 'R'

		# Looks for the first specific colored container in the corresponding order
		for s in self.stacks:
			if len(self.matrix[s[0]]) > 0:
				if self.matrix[s[0]][0] == color:
					return s[0], color

		# If there isn't any, picks the first container in the corresponding order, non-color specified 
		for s in self.stacks:
			if len(self.matrix[s[0]]) > 0 and self.matrix[s[0]][0] != 'X':
				return s[0], self.matrix[s[0]][0]


		# Infinite loop if there aren't more containers to pick
		print "There aren't more containers to pick"
		while True:
			pass

		return -1, 'X'

	# Sets the last_color picked
	def set_last_color(self, color):
		self.last_color = color

	# Print all the containers matrix
	def printMatrix(self):
		for b in range(3):
			for c, s in enumerate(self.matrix):
				lvl = [i for i in self.stacks if i[0] == c][0][1]
				if 3 - lvl > b:
					print " ",
				else:
					print s[b-(3-lvl)],
			print '\n'
		print "======================="