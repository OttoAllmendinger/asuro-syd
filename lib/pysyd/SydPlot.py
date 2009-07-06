from Tkinter import *
import numpy as N

class PlotBuffer:
    def __init__(self, length=128):
        self.length = length
        self.idx = 0
        self.array = N.zeros(self.length)

    def update(self, value):
        self.array[1:] = self.array[:-1]
        self.array[0] = value
        self.idx = min(self.idx+1, self.length)

    def __len__(self):
        return self.idx

class SydPlot(Canvas):
    COLORS = ["black", "red", "#080", "blue", "#800", "#880", "#a35", "#5f2"]

    def __init__(self, master, label, crange=[0, 1000], *args, **kwargs):
        Canvas.__init__(self, master, width=300, height=200, background="white", *args, **kwargs)
        self.label = label
        self.crange = crange
        self.buffers = {}

    def update(self):
        self.draw_background()
        self.crange[0]=min(b.array.min() for n,b in self.buffers.items()) - 20
        self.crange[1]=max(b.array.max() for n,b in self.buffers.items()) + 20
        for n, buf in self.buffers.items():
            self.draw_buffer(buf, n, color=self.COLORS[n])

    def draw_background(self):
        self.W, self.H = W, H = self.winfo_width(), self.winfo_height()
        self.delete(ALL)
        for y in range(20, H, 16):
            self.create_line(0, y, W, y, fill="#cccccc")

    def draw_buffer(self, buf, index=0, color="black"):
        W, H = self.W, self.H
        cmin, cmax = self.crange

        label = "%s: %d" % (self.label, buf.array[0])
        self.create_text((4, 4 + index*16), text=label, anchor=NW, fill=color)


        xcoords = (1.0-N.arange(buf.idx)/float(buf.length)) * W
        ycoords = (buf.array[:buf.idx]-cmin)/float(cmax-cmin) # normalize
        ycoords = H*(1-ycoords) # flip
        #print buf.idx, xcoords, ycoords
        if len(xcoords)>1:
            self.create_line(*zip(xcoords, ycoords), fill=color, smooth=True)

    def plot(self, data):
        for i,v in enumerate(data):
            if not self.buffers.has_key(i):
                self.buffers[i] = PlotBuffer()
            self.buffers[i].update(v)
        self.update()
