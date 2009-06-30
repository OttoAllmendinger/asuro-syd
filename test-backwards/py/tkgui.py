from Tkinter import *
import threading
import time
import random
import numpy as N

from pysyd.util import proxy

from pysyd.SydChart import SydChart
from pysyd import SydDataReader

CRANGE = [0, 1024]


class SensorBuffer:
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

class SensorChart(Canvas):
    COLORS = ["black", "red", "#080", "blue", "#800", "#880", "#a35", "#5f2"]

    def __init__(self, *args, **kwargs):
        Canvas.__init__(self, width=400, height=300, background="white", *args, **kwargs)
        self.buffers = []
        self.crange = [0, 1100]
        self.update()

    def set_range(self, crange):
        self.crange = crange

    def add_source(self, source):
        self.buffers.append(SensorBuffer(source))

    def update(self):
        self.draw_background()
        for n, buf in enumerate(self.buffers):
            buf.update()
            self.plot_buffer(buf, offset=n, color=self.COLORS[n])
        self.after(100, self.update)

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


class ArraySource:
    def __init__(self, cb_name, label, array_index):
        self.cb_name = cb_name
        self.label = label
        self.array_index = array_index
        self.value = 0
        #SydDataReader.addCallback(name, proxy(ROOT, self.read))
        SydDataReader.addCallback(cb_name, self.set_value)

    def set_value(self, data):
        self.value = data[self.array_index]

class RandomSource():
    def __init__(self):
        self.label = "Random Source"
        t = threading.Thread(target=self.set_value)
        t.setDaemon(True)
        t.start()

    def set_value(self):
        while True:
            self.value = random.randint(0, 1000)
            time.sleep(0.1)

class SensorApp:
    def __init__(self, master):
        frame = Frame(master)

        #rnd_chart = SensorChart(frame)
        #rnd_chart.add_source(RandomSource())
        #rnd_chart.pack()

        odo_chart = SensorChart(frame)
        odo_chart.add_source(ArraySource("odo_data", "odo left", 0))
        odo_chart.add_source(ArraySource("odo_data", "odo right", 1))
        odo_chart.pack(fill=BOTH, expand=True, side=LEFT)

        line_chart = SensorChart(frame)
        line_chart.add_source(ArraySource("line_data", "line data right", 0))
        line_chart.add_source(ArraySource("line_data", "line data left", 1))
        line_chart.pack(fill=BOTH, expand=True, side=LEFT)

        switch_chart = SensorChart(frame)
        switch_chart.set_range((-130, +130))
        for i in range(6):
            switch_chart.add_source(ArraySource("sw_time", "switches %d" % i, i))

        switch_chart.pack(fill=BOTH, expand=True, side=LEFT)

        frame.pack()



root = Tk()
root.resizable(0, 0)
app = SensorApp(root)
root.mainloop()
