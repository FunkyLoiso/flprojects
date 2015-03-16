import sys
from ast import literal_eval as make_tuple

test_cases = open(sys.argv[1], 'r')

for test in test_cases:
  if test.strip():
    houses = [make_tuple(tup_string.strip()) for tup_string in test.strip().split(';')]
    heights = [0] * max( h[2] for h in houses )
    
    for h in houses:
      for i in range(h[0], h[2]):
        if heights[i] < h[1]:
          heights[i] = h[1]
    
    last_height = 0
    for i, h in enumerate(heights):
      if h != last_height:
        print i, h,
        last_height = h
    print len(heights), 0

test_cases.close()