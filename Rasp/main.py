import cv2
import sys
sys.path.insert(0, './lib/')
from serial_comunication import Serial
from cam import Cam
from tkinter import Screen
from algorithm import Algorithm
import time

def printChar(pair):
	print "char sent = ",
	print serial.convert(pair)
	print "stack = ",
	print pair[0],
	print "   color = ",
	print pair[1]


# MAIN
cam1 = Cam(0)
cam2 = Cam(1)

screen = Screen()
brain = Algorithm(screen.getParams())

serial = Serial(screen)
print "starting..."
serial.start()
serial.read()

cam1.shoot()    
cam2.shoot()

screen.drawContainers(cam1.getImage(), 3, False)
screen.drawContainers(cam2.getImage(), 2, True)
first = brain.firstPick()  
brain.set_last_color(first[1])

screen.printMatrix()

serial.send(serial.convert(first))
printChar(first)
screen.popContainer(first) 
screen.printMatrix()

first_time = True

while True:
	c = serial.read() 
	
	if c == 'R' or c == 'S':
		c = 'R1' if c == 'R' else 'R2'

	brain.set_last_color(c)

	if(first_time and c == 'G'):
		print "FIRST TIME GREEN"
		serial.send('S')  
		serial.read()
		cam1.shoot()
		cam2.shoot()
		screen.drawContainers(cam1.getImage(), 1, False)
		screen.drawContainers(cam2.getImage(), 0, True)
		second = brain.secondPick()
		screen.popContainer(second)
		printChar(second)
		serial.send(serial.convert(second))
		first_time = False
	else:
		res = brain.solve()
		screen.popContainer(res)
		printChar(res)
		serial.send(serial.convert(res))
	
	screen.printMatrix()

cam1.release()
cam2.release()
cv2.destroyAllWindows()
