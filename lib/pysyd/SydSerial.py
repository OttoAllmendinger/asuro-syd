#!/usr/bin/python

from serial import Serial
import glob

import logging

logging.basicConfig(level=1)

_CON = None

def getConnection():
    global _CON

    if not _CON:
        ttys = glob.glob("/dev/ttyUSB*")
        if len(ttys)>0:
            tty = ttys[0]
            logging.info("using %s" % tty)
            _CON = Serial(tty, 2400)
        else:
            raise Exception("No TTYs available")

    return _CON
