from Tkinter import *
import numpy as N
from pysyd import SydDataReader

class ChartBuffer:
    def __init__(self, source, length=128):
        self.source = source
        self.length = length
        self.idx = 0
        self.array = N.zeros(self.length)

    def update(self):
        self.array[1:] = self.array[:-1]
        self.array[0] = self.source.value
        self.idx = min(self.idx+1, self.length)

    def get():
        return self._array

    def __len__(self):
        return self.idx


class ChartArraySource:
    def __init__(self, cb_name, label, array_index):
        self.label = label
        self.array_index = array_index
        self.value = 0
        SydDataReader.addCallback(cb_name, self.set_value)

    def set_value(self, data):
        self.value = data[self.array_index]

class ChartSource:
    def __init__(self, cb_name, label):
        self.label = label
        self.value = 0
        SydDataReader.addCallback(cb_name, self.set_value)

    def set_value(self, data):
        self.value = data

class SydChart(Canvas):
    COLORS = ["black", "red", "#080", "blue", "#800", "#880", "#a35", "#5f2"]

    def __init__(self, *args, **kwargs):
        Canvas.__init__(self, width=400, height=300, background="white", *args, **kwargs)
        self.buffers = []
        self.crange = [0, 1100]
        self.update()

    def set_range(self, crange):
        self.crange = crange

    def add_source(self, source):
        self.buffers.append(ChartBuffer(source))

    def update(self):
        self.draw_background()
        for n, buf in enumerate(self.buffers):
            buf.update()
            self.plot_buffer(buf, offset=n, color=self.COLORS[n])
        self.after(50, self.update)

    def draw_background(self):
        self.W, self.H = W, H = self.winfo_width(), self.winfo_height()
        self.delete(ALL)
        for y in range(20, H, 16):
            self.create_line(0, y, W, y, fill="#cccccc")

    def plot_buffer(self, buf, offset=0, color="black"):
        W, H = self.W, self.H
        cmin, cmax = self.crange

        label = "%s: %d" % (buf.source.label, buf.source.value)
        self.create_text((4, 4 + offset*16), text=label, anchor=NW, fill=color)

        xcoords = (1.0-N.arange(buf.idx)/float(buf.length)) * W
        ycoords = (buf.array[:buf.idx]-cmin)/float(cmax-cmin) # normalize
        ycoords = H*(1-ycoords) # flip
        #print buf.idx, xcoords, ycoords
        if len(xcoords)>1:
            self.create_line(*zip(xcoords, ycoords), fill=color, smooth=True)
