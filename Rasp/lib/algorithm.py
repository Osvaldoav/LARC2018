
BLUE_ORDER = [7,6,3,2,4,5,0,1]
GREEN_ORDER = [0,1,4,5,3,2,7,6]
R1_ORDER = [1,2,0,3,5,6,4,7]
R2_ORDER = [6,5,7,4,2,1,3,0]

class Algorithm:

	def __init__(self, matrix):
		self.matrix = matrix
		self.last_color = 'B'
		self.blues = 0
		self.greens = 0
		self.reds = 0
		self.priority = -1
		self.order = BLUE_ORDER

	# Return stack 6 or 7 as first pick
	def firstPick(self):
		if self.matrix[7][0] == 'B':
			self.last_color = 'B'
			return 7, 'B'
		elif self.matrix[6][0] == 'B':
			self.last_color = 'B'
			return 6, 'B'
		elif self.matrix[7][0] == 'G':
			self.last_color = 'G'
			return 7, 'G'
		else:
			self.last_color = 'R'
			return 7, 'R'

	# Return stack 2 or 3 as second pick
	def secondPick(self):
		if self.matrix[3][0] == 'G':
			return 3, 'G'
		elif self.matrix[2][0] == 'G':
			return 2, 'G'
		elif self.matrix[3][0] == 'B':
			return 3, 'B'
		else:
			return 3, 'R'

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

	# Sets as priority a stack where a 'R' is found
	def setPriority(self):
		self.updateOrder()
		for s in self.order:
			for b in self.matrix[s]:
				if b == 'R':
					self.priority = s
					return
		self.priority = -1

	# Returns the next stack and color to pick
	def solve(self):
		# Returns 'R' if there is any red container as priority
		if self.priority != -1:
			return self.priority, 'R'
		
		self.updateOrder()

		# Sets the color we are trying to find
		color = self.last_color
		if self.last_color == 'R1' or self.last_color == 'R2':
			color = 'B' if self.blues > self.greens else 'G'

		# Looks for the first specific colored container in the corresponding order
		for s in self.order:
			print "len = ",
			print len(self.matrix[s]) 
			if len(self.matrix[s]) > 0:
				if self.matrix[s][0] == color:
					return s, color

		# If there isn't any picks the first container in the corresponding order, non-color specified 
		for s in self.order:
			if len(self.matrix[s]) > 0:
				return s, self.matrix[s][0]

		# Returns -1 if there are not more containers to pick
		return -1, 'B'


	# Sets the last_color picked
	def set_last_color(self, color):
		self.last_color = color

	# Prints the matrix
	def printMatrix(self):
		print self.matrix