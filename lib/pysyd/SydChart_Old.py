from Tkinter import *
import numpy as N

class SydChart_Old(Canvas):
    COLORS = ["black", "red", "green", "blue", "#800"]

    def __init__(self, *args, **kwargs):
        Canvas.__init__(self, background="white", *args, **kwargs)

    def set_range(self, crange):
        self.crange = crange

    def plot(self, datasets):
        self.draw_background()
        for n, (label, dataset, length) in enumerate(datasets):
            color = self.COLORS[n]
            self.plot_dataset(n, label, dataset, length, crange)

    def draw_background(self):
        W, H = self.winfo_width(), self.winfo_height()
        self.delete(ALL)
        for y in range(20, H, 16):
            self.create_line(0, y, W, y, fill="#cccccc")

    def plot_dataset(self, n, label, data, length, crange):
        W, H = self.winfo_width(), self.winfo_height()
        color = self.COLORS[n]
        data = N.array(data)


        if length==None:
            length = len(data)

        if len(data)>0:
            label += " " + str(data[-1])
            self.create_text((4, 4 + n*16), text=label, anchor=NW, fill=color)
            cmin, cmax = crange
            cmin = data.min() if cmin==None else cmin
            cmax = data.max() if cmax==None else cmax
            xcoords = N.linspace(0, W, length)
            ycoords = (data-cmin)/float(cmax-cmin) # normalize
            ycoords = H*(1-ycoords) # flip
            self.create_line(*zip(xcoords, ycoords), fill=color, smooth=True)

