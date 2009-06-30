import sys
import string

import time

from os.path import *
from serial import Serial


TICKS=10000


TTY_DEVICE = expanduser("~/src/asuro/dev/tty")

CON = Serial(TTY_DEVICE, 2400)

line_length = 0
ts = time.time()

STAMPS = {}

while True:
    line = CON.readline()
    mark = line.strip()
    ts = STAMPS.get(mark, None)
    if not ts==None:
        print "diff %s: %.5f" % (mark, time.time()-ts)
    STAMPS[mark]=time.time()
