import sys
import string
import StringIO
import time
import struct

import logging

from os.path import *
from serial import Serial


TTY_DEVICE = expanduser("~/src/asuro/dev/tty")

VALID_CHARS = set(string.printable)
VALID_CHARS.remove("\n")

MEM = {}

def nice(_str):
    def _ascii(char):
        if char in VALID_CHARS:
            return char
        else:
            return "."
    return "".join(map(_ascii, _str))

class ReceiveError(Exception):
    pass

class DataSource_Serial:
    def __init__(self):
        self.con = Serial(TTY_DEVICE, 2400)
        self.logfile = StringIO.StringIO()

    def read(self,length):
        data = self.con.read(length)
        self.logfile.write(data)
        return data

    def readline(self):
        global LOGFILE
        data = self.con.readline()
        if data=="START\n":
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


def hexdump(data):
    rest = True
    while data:
        line, rest = data[:16], data[16:]
        hex_line =  "".join("%02x " % ord(c) for c in line)
        asc_line =  nice(line)
        logging.debug("%s | %s" % (hex_line, asc_line))
        data = rest


def rcv_data(source, line):

    try:
        sig, name, fmt, dsize = line.split(":")
    except:
        logging.debug("couldn't parse header line")
        raise ReceiveError()

    try:
        data = source.read(int(dsize))
        unpacked = struct.unpack(fmt, data)
        return name, unpacked
        #MEM[name]=unpacked
        #logging "%s=%s" % (name, unpacked)
    except:
        #print "couldn't parse %s as %s" % (name, fmt)
        hexdump(data)
        raise ReceiveError()


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


def parse(source, callback):
    line = True
    while line:
        line = source.readline()
        print "[syd] %s\n" % nice(line.strip()),

        if line.startswith("SEND:"):
            try:
                name, data = rcv_data(source, line)
                callback(name, data)
            except ReceiveError:
                logging.exception("")
                pass



if __name__=="__main__":
    main()
    #test()
