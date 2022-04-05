# generate random string to provide data to display
import serial
from time import sleep
from random import random as rnd
port = "/dev/ttyUSB0"

rf = serial.Serial(port, baudrate=9600)
while True:
    fake = str(int(rnd()*10e12)) + '\r'
    print(fake)
    rf.write(fake.encode())
    sleep(2)
