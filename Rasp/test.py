import cv2
import sys
sys.path.insert(0, '/home/pi/Documents/LARC2018/Rasp/lib/')
from serial_comunication import Serial
from cam import Cam
from tkinter import Screen
import time

# screen.drawText()

# brain = Algorithm(screen.getParams())

serial = Serial()
print "starting"
serial.start()   #
serial.read()   #

while True:
	serial.send('B')
	# time.sleep(1)
	c = serial.read()
	print "first = ",
	print c
	serial.send('j')
	# time.sleep(1)
	c = serial.read()
	print "second = ",
	print c
	serial.send('9')
	# time.sleep(1)
	c = serial.read()
	print "third = ",
	print c