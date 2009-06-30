import sys
import string
import StringIO
import time
import struct
import threading
import pprint
import numpy as N

import logging

from os.path import *
from serial import Serial


TTY_DEVICE = expanduser("~/src/asuro/dev/tty")

VALID_CHARS = set(string.printable)
VALID_CHARS.remove("\n")

MEM = {}

class ReceiveError(Exception):
    pass

def nice(_str):
    def _ascii(char):
        if char in VALID_CHARS:
            return char
        else:
            return "."
    return "".join(map(_ascii, _str))

def hexdump(data):
    rest = True
    while data:
        line, rest = data[:16], data[16:]
        hex_line =  "".join("%02x " % ord(c) for c in line)
        asc_line =  nice(line)
        logging.debug("%s | %s" % (hex_line, asc_line))
        data = rest


class DataSource_Serial:
    def __init__(self, logging=False):
        self.con = Serial(TTY_DEVICE, 2400)
        self.logging = logging
        self.logfile = StringIO.StringIO()

    def read(self,length):
        data = self.con.read(length)
        self.logfile.write(data)
        return data

    def readline(self):
        global LOGFILE
        data = self.con.readline()
        if data=="START\n" and self.logging:
            self.logfile = self.create_logfile()
        self.logfile.write(data)
        return data

    def create_logfile(self):
        f = None
        i = 0
        while not f:
            fname = 'logs/ttydump-%d.dat' % i
            if exists(fname):
                i+=1
            else:
                f = file(fname, 'w')
        return f


class DataSource_File:
    def __init__(self, filename):
        self.file = file(filename)

    def read(self,length):
        return self.file.read(length)

    def readline(self):
        return self.file.readline()




def main():
    if len(sys.argv)==2:
        fname = sys.argv[1]
        print "using %s as data source" % fname
        source = DataSource_File(fname)
    else:
        source = DataSource_Serial()
        print "using %s as data source" % source

    def cb_debug(name, data):
        logging.debug("%s=%s" % (name, data))

    parse(source, cb_debug)


class SydDataReader:
    def __init__(self, dataSource=None):
        self.callbacks = {}
        if dataSource==None:
            dataSource = DataSource_Serial()
        self.dataSource = dataSource
        readThread = threading.Thread(target=self.read)
        readThread.setDaemon(True)
        readThread.start()

    def unpack(self, line):
        try:
            sig, name, fmt, dsize = line.split(":")
        except:
            logging.debug("couldn't parse header line")
            raise ReceiveError()

        # XXX: this should be done better
        fmt = fmt.replace("i", "h")

        data = None
        try:
            data = self.dataSource.read(int(dsize))
            unpacked = N.array(struct.unpack(fmt, data))
            return name, unpacked
        except:
            logging.exception("failed to read data")
            if data!=None:
                hexdump(data)
            raise ReceiveError()

    def apply_callbacks(self, var_name, data):
        for cb in self.callbacks.get(var_name, []):
            try:
                cb(data)
            except:
                logging.exception("failed on callback %s" % cb)

    def read(self):
        line = True
        while line:
            line = self.dataSource.readline()

            if line.startswith("SEND:"):
                try:
                    var_name, data = self.unpack(line)
                    self.apply_callbacks(var_name, data)
                except Exception:
                    print "[syd] %s\n" % nice(line.strip()),
                    logging.exception("fail")
                    pass



_INSTANCE = SydDataReader()

def addCallback(var_name, function):
    if var_name in _INSTANCE.callbacks:
        _INSTANCE.callbacks[var_name].append(function)
    else:
        _INSTANCE.callbacks[var_name] = [function]

