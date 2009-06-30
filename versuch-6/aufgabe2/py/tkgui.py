from Tkinter import *
import threading
import random
import Queue
import numpy as N

from pysyd.util import proxy

from pysyd.SydChart import SydChart
from pysyd import SydDataReader

CRANGE = [500, 1500]

class OdometryMonitor(Frame):
    def __init__(self, master, *args, **kwargs):
        Frame.__init__(self, master, *args, **kwargs)

        self.odo_buffer = N.zeros((64, 2))
        self.odo_buffer_avg = N.zeros((64, 2))
        self.odo_avg = [0,0]
        self.odo_pointer = 0


        self.chart_l = SydChart(
                self,
                height=400, width=400,
                bd=2, relief=SUNKEN)
        self.chart_l.pack(fill=BOTH, expand=True, side=LEFT)

        self.chart_r = SydChart(
                self,
                height=400, width=400,
                bd=2, relief=SUNKEN)
        self.chart_r.pack(fill=BOTH, expand=True, side=LEFT)

        self.scale_min = Scale(self,
                from_=2000, to=0,
                label="min",
                command=self.cb_scale_min)

        self.scale_max = Scale(self,
                from_=2000, to=0,
                label="max",
                command=self.cb_scale_max)

        self.scale_min.pack(side=LEFT, fill=BOTH, expand=True)
        self.scale_max.pack(side=LEFT, fill=BOTH, expand=True)

        self.scale_min.set(CRANGE[0])
        self.scale_max.set(CRANGE[1])

        SydDataReader.addCallback("odo_data", proxy(self, self.plot_od_data))
        SydDataReader.addCallback("avg", proxy(self, self.plot_avg))
        self.pack()

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


    def plot_od_data(self, data):
        if self.odo_pointer>(len(self.odo_buffer)-1):
            self.odo_buffer *= 0
            self.odo_pointer = 0

        self.odo_buffer[self.odo_pointer][0] = data[0]
        self.odo_buffer[self.odo_pointer][1] = data[1]

        self.odo_buffer_avg[self.odo_pointer][0] = self.odo_avg[0]
        self.odo_buffer_avg[self.odo_pointer][1] = self.odo_avg[1]

        self.odo_pointer += 1

        self.update_charts()

    def plot_avg(self, data):
        self.odo_avg = data

    def get_dataset(self, index):
        return (
                ("left odo data", self.odo_buffer[:self.odo_pointer,index], len(self.odo_buffer)),
                ("avg", self.odo_buffer_avg[:self.odo_pointer,index], len(self.odo_buffer))
        )

    def update_charts(self):
        if self.odo_pointer>2:
            dataset_l = self.get_dataset(0)
            dataset_r = self.get_dataset(1)
            self.chart_l.plot(dataset_l, CRANGE)
            self.chart_r.plot(dataset_r, CRANGE)


class OdometryApp:
    def __init__(self, master):
        monitor = OdometryMonitor(master)
        monitor.pack()



root = Tk()
root.resizable(0, 0)
app = OdometryApp(root)
root.mainloop()
