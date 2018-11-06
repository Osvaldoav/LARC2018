import RPi.GPIO as GPIO 
import cv2
import sys
sys.path.insert(0, '/home/pi/Documents/LARC2018/Rasp/lib/')
from algorithm import Algorithm
from cam import Cam
import time

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(8, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(11, GPIO.OUT, initial=GPIO.LOW)

cam1 = Cam(0)
cam2 = Cam(1)

try:
    brain = Algorithm()

	for i in range(3):
		GPIO.output(8, GPIO.HIGH) 
		time.sleep(0.5) 
		GPIO.output(8, GPIO.LOW) 
		time.sleep(0.5) 
	print "starting..."
    cam1.shoot()    
	cam2.shoot()
    brain.updateContainers(cam1.getImage(), 3, False)
	brain.updateContainers(cam2.getImage(), 2, True)
    brain.printMatrix()

except (KeyboardInterrupt, SystemExit):
	cam1.release()
	cam2.release()
	cv2.destroyAllWindows()
	print "\nProgram has ended!"
else:
	print "\nSomething went wrong..."