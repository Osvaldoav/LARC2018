import cv2
import sys
sys.path.insert(0, './lib/')
from serial_comunication import Serial
from cam import Cam
from algorithm import Algorithm
import time

def printChar(pair):
	print "char sent = ",
	print serial.convert(pair)
	print "stack = ",
	print pair[0],
	print "   color = ",
	print pair[1]

# SETUP
brain = Algorithm()
serial = Serial()

print "starting..."
serial.start()
c = serial.read()
cam1 = Cam(0)
cam2 = Cam(1)

cam1.shoot()    
cam2.shoot()

brain.updateContainers(cam1.getImage(), 3, False)
brain.updateContainers(cam2.getImage(), 2, True)
first = brain.firstPick()  
brain.set_last_color(first[1])

brain.printMatrix()

serial.send(serial.convert(first))
printChar(first)
brain.popContainer(first) 
brain.printMatrix()

first_time = True

# MAIN
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
		brain.updateContainers(cam1.getImage(), 1, False)
		brain.updateContainers(cam2.getImage(), 0, True)
		second = brain.secondPick()
		brain.popContainer(second)
		printChar(second)
		serial.send(serial.convert(second))
		first_time = False
	else:
		res = brain.solve()
		brain.popContainer(res)
		printChar(res)
		serial.send(serial.convert(res))
	
	brain.printMatrix()

cam1.release()
cam2.release()
cv2.destroyAllWindows()
