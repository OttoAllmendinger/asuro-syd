#!/usr/bin/python

import threading
import numpy as N
import SydSerial
import logging


SIGNED = 0x80

def hexdump(string):
    for c in string:
        print "%02x " % c,
    print

class SydReceiver():
    def __init__(self, callback=None):
        self.con = SydSerial.getConnection()
        self.callback = callback or self.dump

    def dump(self, _id, nptype, npdata):
        print("%d (%s) %s" % (_id, nptype, npdata))

    def read_chunk(self):
        header = self.con.read(7)

        _mark = header[:4]
        _id, _type, _len = map(ord, header[4:])
        _signed = (_type&SIGNED)>0
        _bits = (_type&(0xff-SIGNED))

        try:
            assert _mark=='send'
            assert _id<255
            assert _bits in (1,2,3,4)
            assert _len<128
        except AssertionError:
            print map(lambda x: "%02x " % x, header)
            raise Exception("Read Error")

        _nptype = '%sint%d' % (('u' if not _signed else ''), _bits*8)

        data = self.con.read(_bits * _len)
        npdata = N.fromstring(data, _nptype)

        return _id, _nptype, npdata

    def receive(self):
        while True:
            c = self.con.read(1)
            while not c=='\n':
                c = self.con.read(1)

            try:
                data = self.read_chunk()
            except:
                data = None

            if data:
                try:
                    self.callback(*data)
                    pass
                except:
                    logging.exception("callback error")



if __name__=="__main__":
    r = SydReceiver()
    r.receive()
