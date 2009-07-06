from Tkinter import *
import threading
import time
import random
import numpy as N

from pysyd.util import *
from pysyd.SydPlot import *
from pysyd.SydReceiver import *

DEBUG = 0x80

class BufferWriter:
    def write(self, _id, _type, data):
        pass

def get_range(_id, _type):
    if _type=='int8':
        crange = [-127, 127]
    elif _type=='int16':
        crange = [-1024, 1024]
    elif _type=="uint8":
        crange = [0, 256]
    elif _type== "uint16":
        crange = [0, 2048]
    return crange

class SensorApp:
    def __init__(self, master):
        br = BufferWriter()

        self.frame = Frame(master, width=100, height=60)
        self.plots = {}
        self.frame.pack()

        t = threading.Thread(target=self.update_buffers)
        t.setDaemon(True)
        t.start()

    def update_buffers(self):
        receiver = SydReceiver(callback=proxy(self.frame, self.write))
        receiver.receive()

    def write(self, _id, _type, _data):
        if (_id & DEBUG):
            self.log(_id, _type, _data)
        else:
            self.plot(_id, _type, _data)

    def log(self, _id, _type, _data):
        _id = _id - DEBUG
        logging.info("%s, %s: %s" % (_id, _type, _data))

    def plot(self, _id, _type, _data):
        COLS = 4
        if not self.plots.has_key(_id):
            n_plot = len(self.plots)
            label = "Sensor %d" % _id
            plot = SydPlot(self.frame, label, get_range(_id, _type))
            self.plots[_id] = plot
            plot.grid(row=int(n_plot/float(COLS)), column=int(n_plot % COLS))
        self.plots[_id].plot(_data)


root = Tk()
root.resizable(0, 0)
app = SensorApp(root)
root.mainloop()
