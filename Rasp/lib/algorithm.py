
BLUE_ORDER = [7,6,3,2,4,5,0,1]
GREEN_ORDER = [0,1,4,5,3,2,7,6]
R1_ORDER = [1,2,0,3,5,6,4,7]
R2_ORDER = [6,5,7,4,2,1,3,0]

class Algorithm:

	def __init__(self, params):
		self.matrix, self.last_color, self.greens, self.blues = params
		self.reds = 0
		self.priority = -1
		self.order = BLUE_ORDER

	# Return stack 6 or 7 as first pick
	def firstPick(self):
		if self.matrix[7][0] == 'R':
			return 7, 'R'
		elif self.matrix[6][0] == 'R':
			return 6, 'R'
		elif self.matrix[7][0] == 'B':
			return 7, 'B'
		elif self.matrix[6][0] == 'B':
			return 6, 'B'
		else:
			return 7, 'G'

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

	# Prints the matrix
	def printMatrix(self):
		print self.matrix