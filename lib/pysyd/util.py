import Queue
import logging

class QueueProxy:
    def __init__(self, widget, target):
        self.queue = Queue.Queue()
        self.widget = widget
        self.target = target
        self.read_queue()

    def write(self, *data):
        self.queue.put(data)

    def read_queue(self):
        try:
            while True:
                args = self.queue.get_nowait()
                try:
                    self.target(*args)
                except:
                    logging.exception("failure")
                self.widget.update_idletasks()
        except Queue.Empty:
            pass
        self.widget.after(5, self.read_queue)

    def __str__(self):
        return "proxy(%s)" % self.target

    def __repr__(self):
        return str(self)

def proxy(widget, target):
    return QueueProxy(widget, target).write
