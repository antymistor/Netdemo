from multiprocessing import Process, Queue,Value
import ctypes
import multiprocessing as mp
import threading
import time
import os

def foo(q,v):
    while True:
        # q.put('P'+str(v))
        # print("p"+q.get())
        with v.get_lock():
            v.value =v.value+5
        q.put("P"+str(v.value))
        time.sleep(1)


if __name__ == '__main__':
    i=0

    # ctx = mp.get_context('spawn')
    # q = ctx.Queue()
    # p = ctx.Process(target=foo, args=(q,))
    print(mp.cpu_count())
    q = Queue()
    v = Value('d',0)
    # v.value=0
    p=Process(target=foo, args=(q,v))
    p.start()
    while True:
        time.sleep(1)
        with v.get_lock():
            v.value = v.value -1
        # q.put('M' + str(v))
        # while(q.qsize()):
        print("m"+q.get())
        print(v.value)
    # p.join()