import sys
sys.path.insert(0, './lib/')
import stacks

BLUE_ORDER = [7,6,3,2,4,5,0,1]
GREEN_ORDER = [0,1,4,5,3,2,7,6]
R1_ORDER = [1,2,0,3,5,6,4,7]
R2_ORDER = [6,5,7,4,2,1,3,0]

class Algorithm:

	def __init__(self):
		self.matrix = [['X' for j in range(3)] for i in range(8)]
		self.priority = -1
		self.order = BLUE_ORDER
		self.blues = 0
		self.greens = 0
		self.reds = 0
		self.last_color = 'B'
		self.stack = [3 for i in range(8)]

	# Draws rectangles with a specific color and position regarding the image and section
	def updateContainers(self, image, section, left_cam):
		_stacks = stacks.getMatrix(image, left_cam)

		for c, s in enumerate(_stacks):
			for b in range(3):
				self.matrix[section*2 + c][b] = s[b]

	# Remove the top container of a specific stack
	def popContainer(self, pair):
		if pair[1] == 'G':
			self.greens += 1
		elif pair[1] == 'B':
			self.blues += 1

		self.last_color = pair[1]
		self.matrix[pair[0]].pop(0)
		self.stack[pair[0]] -= 1

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

	# Updates the order regarding last_color
	def updateOrder(self):
		if self.last_color == 'B':
			self.order = BLUE_ORDER
		elif self.last_color == 'G':
			self.order = GREEN_ORDER
		elif self.last_color == 'R1':
			self.order = R1_ORDER
		else:
			self.order = R2_ORDER

	# Returns the next stack and color to pick
	def solve(self):
		self.updateOrder()

		# Sets the color we are trying to find
		if self.greens % 5 == 0 and self.greens > 0:
			color = 'B'
		elif self.blues % 5 == 0 and self.blues > 0:
			color = 'G'
		else:
			color = 'B' if self.blues % 5 > self.greens % 5 else 'G'

		# Looks for the first red colored container in the corresponding order
		for s in self.order:
			if len(self.matrix[s]) > 0:
				if self.matrix[s][0] == 'R':
					return s, 'R'

		# Looks for the first specific colored container in the corresponding order
		for s in self.order:
			if len(self.matrix[s]) > 0:
				if self.matrix[s][0] == color:
					return s, color

		# If there isn't any, picks the first container in the corresponding order, non-color specified 
		for s in self.order:
			if len(self.matrix[s]) > 0:
				return s, self.matrix[s][0]

		# Returns -1,'X' if there are not more containers to pick
		return -1, 'X'

	# Sets the last_color picked
	def set_last_color(self, color):
		self.last_color = color

	# Print all the containers matrix
	def printMatrix(self):
		for b in range(3):
			for c, s in enumerate(self.matrix):
				if 3 - self.stack[c] > b:
					print " ",
				else:
					print s[b-(3-self.stack[c])],
			print '\n'
		print "======================="