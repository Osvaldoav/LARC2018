# from Tkinter import *

import sys
sys.path.insert(0, './lib/')
import stacks

rect_w = 35
rect_h = 18

class Screen:
	def __init__(self):
		self.containers = [['X' for j in range(3)] for i in range(8)]
		# self.root = Tk()
		# self.canvas = Canvas(self.root, width=320, height=240)
		# self.canvas.pack()
		self.stack = [3 for i in range(8)]
		self.blues = 0
		self.greens = 0
		self.last_color = 'B'

	# # Draws title and indexes in the screen
	# def drawText(self):
	# 	self.canvas.create_text(160,12, text="ROBORREGOS")
	# 	x = 21

	# 	for i in range(8):
	# 		self.canvas.create_text(x, 98, text=str(i))
	# 		x += rect_w + 4 

	# Draws rectangles with a specific color and position regarding the image and section
	def drawContainers(self, image, section, left_cam):
		_stacks = stacks.getMatrix(image, left_cam)

		# x0, y0, x1, y1 = 4, 25, 39, 43
		# x0 += section * (rect_w + 4) * 2
		# x1 = x0 + rect_w

		for c, s in enumerate(_stacks):
			for b in range(3):
				# color = "red" if s[b] == 'R' else "green" if s[b] == 'G' else "blue"
				# self.canvas.create_rectangle(x0, y0, x1, y1, fill=color)
				self.containers[section*2 + c][b] = s[b]
				# y0 += rect_h + 4
				# y1 = y0 + rect_h
			# x0 += rect_w + 4
			# x1 = x0 + rect_w
			# y0, y1 = 25, 43

	# Remove the top container of a specific stack
	def popContainer(self, pair):
		# x0 = pair[0] * (rect_w + 4) + 4
		# x1 = x0 + rect_w + 1
		# y0 = 91 - len(self.containers[pair[0]]) * (rect_h + 4)
		# y1 = y0 + rect_h + 1

		if pair[1] == 'G':
			self.greens += 1
		elif pair[1] == 'B':
			self.blues += 1

		self.last_color = pair[1]
		self.containers[pair[0]].pop(0)
		self.stack[pair[0]] -= 1

		# self.canvas.create_rectangle(x0, y0, x1, y1, fill="white", outline="")

	# # Displays an error message on the screen
	# def errorMessage(self, msg):
	# 	txt = "Error: " + msg
	# 	self.canvas.create_text(160,230, text=txt, fill="red")

	# Returns the containers attribute
	def getParams(self):
		return self.containers, self.last_color, self.greens, self.blues

	# # Runs a function after certain time without closing the screen window
	# def run(self, milliseconds, function):
	# 	self.root.after(milliseconds, function)

	# # Calls Tk.mainloop
	# def mainloop(self):
	# 	self.root.mainloop()

	def printMatrix(self):
		for b in range(3):
			for c, s in enumerate(self.containers):
				if 3 - self.stack[c] > b:
					print " ",
				else:
					print s[b-(3-self.stack[c])],
			print '\n'
		print "======================="
