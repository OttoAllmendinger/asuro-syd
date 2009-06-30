import parse
from pylab import *

SPEEDS = { 'left': [], 'right': [] }
def cb_speeds(name, data):
    if name=="samples_l":
        SPEEDS['left'].extend(list(data))
    elif name=="samples_r":
        SPEEDS['right'].extend(list(data))

parse.parse(parse.DataSource_File("logs/ttydump-17.dat"), cb_speeds)

plot(SPEEDS['left'], linewidth=1.0)
plot(SPEEDS['right'], linewidth=1.0)

show()

