import RPi.GPIO as GPIO 
import cv2
import sys
sys.path.insert(0, '/home/pi/Documents/LARC2018/Rasp/lib/')
from cam import Cam
import time


cam1 = Cam(0)
cam2 = Cam(1)

try:
	for i in range(3):
		GPIO.output(8, GPIO.HIGH) 
		time.sleep(0.5) 
		GPIO.output(8, GPIO.LOW) 
		time.sleep(0.5) 
	print "starting..."
    cam1.shoot()    
	cam2.shoot()

except (KeyboardInterrupt, SystemExit):
	cam1.release()
	cam2.release()
	cv2.destroyAllWindows()
	print "\nProgram has ended!"
else:
	print "\nSomething went wrong..."