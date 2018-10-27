import cv2
import sys
sys.path.insert(0, './lib/')
from serial_comunication import Serial
from cam import Cam
from tkinter import Screen
from algorithm import Algorithm
import time

cam1 = Cam(0)
cam2 = Cam(1)

screen = Screen()
screen.drawText()

brain = Algorithm(screen.getParams())

serial = Serial(screen)
print "starting"
serial.start()   #
serial.read()   #
# cam1.shoot()    #
# cam2.shoot()    #


# -------------------------
while True:
	serial.send('B')
	time.sleep(0.5)
	c = serial.read()
	print "first = ",
	print c
	serial.send('j')
	time.sleep(0.5)
	c = serial.read()
	print "second = ",
	print c
	serial.send('9')
	time.sleep(0.5)
	c = serial.read()
	print "third = ",
	print c


# -------------------------


screen.drawContainers(cam1.getImage(), 3)
screen.drawContainers(cam2.getImage(), 2)
first = brain.firstPick()  #
brain.set_last_color(first[1])
screen.printMatrix()
screen.popContainer(first) #
screen.printMatrix()
serial.send(serial.convert(first))#
print "c_serial =",
print serial.convert(first)
print "First Stack sent"

first_time = True

while True:
	c = serial.read()
	print "c = ",
	print c

	if c == 'R' or c == 'S':
		c = 'R1' if c == 'R' else 'R2'

	brain.set_last_color(c)

	if(first_time and c == 'G'):
		print "SIMIOOOOOS"
		serial.send('S')
		serial.read()
		cam1.shoot()
		cam2.shoot()
		screen.drawContainers(cam1.getImage(), 1)
		screen.drawContainers(cam2.getImage(), 0)
		second = brain.secondPick()
		screen.popContainer(second)
		serial.send(serial.convert(second))
		first_time = False
	else:
		res = brain.solve()
		screen.popContainer(res)
		serial.send(serial.convert(res))

	
	screen.printMatrix()

# def main():
# 	global first_time
# 	c = serial.read()

# 	if c == 'R' or c == 'S':
# 		c = 'R1' if c == 'R' else 'R2'

# 	brain.set_last_color(c)

# 	if(first_time and c == 'G'):
# 		serial.send('S')
# 		serial.read()
# 		cam1.shoot()
# 		cam2.shoot()
# 		screen.drawContainers(cam1.getImage(), 1)
# 		screen.drawContainers(cam2.getImage(), 0)
# 		second = brain.secondPick()
# 		screen.popContainer(second[0])
# 		serial.send(serial.convert(second))
# 		brain.setPriority()
# 		first_time = False
# 	else:
# 		res = brain.solve()
# 		screen.popContainer(res[0])
# 		serial.send(serial.convert(res))
# 	print "loop sent"

# 	screen.run(5,main)

# screen.run(0, main)
# screen.mainloop()

cam1.release()
cam2.release()
cv2.destroyAllWindows()
