from random import gauss
from datetime import datetime, timedelta
from functools import partial
import operator
import math
import sys
import os

def seq_gen(start, op=partial(operator.add, 1)):
  while True:
    yield start
    start = op(start)

def fn_gen(fn):
  while True:
    yield fn()

class cyclic_moving_body:
  def __init__(self):
    self.weight = 0.1
    self.time = datetime.utcnow()
    self.pos = 0
    self.speed = 0
    self.acc = 0

  def next(self):
    dt = timedelta(milliseconds=gauss(500, 300))
    self.time += dt  # update time
    dv = self.acc * dt.total_seconds()  # dv = a * t
    dp = self.speed + dv * dt.total_seconds() / 2 # dp = v + (a * t^2)/2 = v + dv * t / 2
    self.speed += dv  # update speed
    self.pos += dp  # update position

    acc_bias = 0.05 * math.sin(self.time.second / 10 * 2 * math.pi)  # interval is 10 seconds
    self.acc = gauss(0, 0.01) + acc_bias
    self.acc = math.copysign(self.acc, -self.pos)  # acc always try to bring pos to 0

    return '{0.time},{0.acc},{0.speed},{0.pos}'.format(self)

if __name__ == "__main__":
  if(len(sys.argv) < 3):
    print("Usage: {} <out_file> <approx_size>").format(os.path.basename(__file__))
  else:
    out_file = sys.argv[1]
    out_min_bytes = int(sys.argv[2])

    with open(out_file, 'w') as fd:
      seq_number = seq_gen(0)
      time = seq_gen(datetime.utcnow(), lambda v: v + timedelta(milliseconds=gauss(500, 300)))
      weight = fn_gen(partial(gauss, 12, 3))
      body_info = cyclic_moving_body()

      generators = [seq_number, time, weight, body_info]
      while fd.tell() < out_min_bytes:
        line = ''
        for g in generators:
          line += str(g.next()) + ','
        fd.write(line[:-1] + '\n')
        # print line[:-1]