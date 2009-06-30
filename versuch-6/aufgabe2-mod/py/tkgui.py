from Tkinter import *
import threading
import random
import Queue
import numpy as N

from pysyd.util import proxy

from pysyd.SydChart import SydChart
from pysyd import SydDataReader

CRANGE = [500, 1500]

class LineMonitor(Frame):
    def __init__(self, master, *args, **kwargs):
        Frame.__init__(self, master, *args, **kwargs)

        self.line_buffer = N.zeros(64)
        self.line_pointer = 0


        self.chart = SydChart(
                self,
                height=300, width=500,
                bd=2, relief=SUNKEN)

        self.scale_min = Scale(self,
                from_=2000, to=0,
                label="min",
                command=self.cb_scale_min)

        self.scale_max = Scale(self,
                from_=2000, to=0,
                label="max",
                command=self.cb_scale_max)

        self.chart.pack(side=LEFT)

        self.scale_min.pack(side=LEFT, fill=BOTH, expand=True)
        self.scale_max.pack(side=LEFT, fill=BOTH, expand=True)

        self.scale_min.set(CRANGE[0])
        self.scale_max.set(CRANGE[1])

        SydDataReader.addCallback("line_data", proxy(self, self.plot_line_data))

    def cb_scale_min(self, value):
        value = int(value)
        CRANGE[0] = value
        self.scale_max.set(str(max(value, int(self.scale_max.get()))))
        self.update_charts()

    def cb_scale_max(self, value):
        value = int(value)
        CRANGE[1] = value
        self.scale_min.set(str(min(value, int(self.scale_min.get()))))
        self.update_charts()


    def plot_line_data(self, data):
        if self.line_pointer>(len(self.line_buffer)-1):
            self.line_buffer *= 0
            self.line_pointer = 0

        self.line_buffer[self.line_buffer][0] = data[0]
        self.line_buffer[self.line_pointer][1] = data[1]

        self.line_pointer += 1

        self.update_charts()

    def update_charts(self):
        if self.line_pointer>1:
            dataset = (
                ("line data", self.line_buffer[:self.line_pointer], len(self.line_buffer)),
            )
            self.chart.plot(dataset, CRANGE)


class App:
    def __init__(self, master):
        monitor = LineMonitor(master)
        monitor.pack()



root = Tk()
root.resizable(0, 0)
app = App(root)
root.mainloop()
