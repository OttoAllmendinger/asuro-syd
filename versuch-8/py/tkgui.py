from Tkinter import *
import threading
import time
import random
import numpy as N

from pysyd.util import proxy

from pysyd.SydChart import *
from pysyd import SydDataReader

CRANGE = [0, 1024]



class SensorApp:
    def __init__(self, master):
        frame = Frame(master)

        line_chart = SydChart(frame)
        line_chart.set_range((0, 1100))
        line_chart.add_source(ChartArraySource("line_data", "line_data[0]", 0))
        line_chart.add_source(ChartArraySource("line_data", "line_data[1]", 1))
        line_chart.pack(fill=BOTH, expand=True, side=LEFT)

        diff_chart = SydChart(frame)
        diff_chart.set_range((-300, 300))
        diff_chart.add_source(ChartSource("diff", "diff"))
        diff_chart.pack(fill=BOTH, expand=True, side=LEFT)

        frame.pack()


root = Tk()
root.resizable(0, 0)
app = SensorApp(root)
root.mainloop()
