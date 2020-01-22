import serial
import time

try:
    arduino = serial.Serial("/dev/ttyACM0", 9600, timeout=0, writeTimeout=0)
    time.sleep(3)
except serial.SerialException:
    print 'Bad port, check labeling'
    while True:
        pass


def send(char):
    arduino.write(char)

    while(arduino.inWaiting() <= 0):
        pass

    p = arduino.read()

    print "received from arduino: ",
    print p


while True:
    send("M")


