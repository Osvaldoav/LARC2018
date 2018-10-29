import sys
sys.path.insert(0, './lib/')
import stacks

class Screen:
	def __init__(self):
		self.containers = [['X' for j in range(3)] for i in range(8)]
		self.stack = [3 for i in range(8)]
		self.blues = 0
		self.greens = 0
		self.last_color = 'B'

	# Draws rectangles with a specific color and position regarding the image and section
	def drawContainers(self, image, section, left_cam):
		_stacks = stacks.getMatrix(image, left_cam)

		for c, s in enumerate(_stacks):
			for b in range(3):
				self.containers[section*2 + c][b] = s[b]

	# Remove the top container of a specific stack
	def popContainer(self, pair):

		if pair[1] == 'G':
			self.greens += 1
		elif pair[1] == 'B':
			self.blues += 1

		self.last_color = pair[1]
		self.containers[pair[0]].pop(0)
		self.stack[pair[0]] -= 1

	# Returns the containers attribute
	def getParams(self):
		return self.containers, self.last_color, self.greens, self.blues

	def printMatrix(self):
		for b in range(3):
			for c, s in enumerate(self.containers):
				if 3 - self.stack[c] > b:
					print " ",
				else:
					print s[b-(3-self.stack[c])],
			print '\n'
		print "======================="
