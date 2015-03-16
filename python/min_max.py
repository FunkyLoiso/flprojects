import math

seq = [1, 4, -6, 8, 3, -1, 0, 0, 13]

less_counter = 0

def counting_less(l, r):
  global less_counter
  less_counter += 1
  return l < r
  
def abs(val):
  return int(math.sqrt(val * val))
  
# impossible for val = 0!
def signum(val):
  return abs(val) / val
  
def mod(val):
  return val*2+1

def demod(val):
  return (val-1)/2

def min_max(seq):
  max_dst_sq = 0;
  res = [mod(seq[0]), 42, mod(seq[0])]
  for val in seq[1:]:
    dst = res[0] - mod(val)
    dst_sq = dst * dst
    if counting_less(max_dst_sq, dst_sq):
      sgn = signum(dst)
      res[sgn+1] = res[0]
      res[-sgn+1] = mod(val)
  return (demod(res[0]), demod(res[2]))
  
print seq
print 'my ', min_max(seq)
print 'comps', less_counter
print 'std', (min(seq), max(seq))

import random
