import sys
import string

from os.path import *
from serial import Serial



TTY_DEVICE = expanduser("~/src/asuro/dev/tty")

CON = Serial(TTY_DEVICE, 2400)

line_length = 0
while True:
    char = CON.read()
    if char in string.printable:
        sys.stdout.write(char)
    else:
        sys.stdout.write("%02x " % ord(CON.read()))
        sys.stdout.flush()
        line_length+=1
        if line_length>15:
            sys.stdout.write("\n")
            line_length=0
