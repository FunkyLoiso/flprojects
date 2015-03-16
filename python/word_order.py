#2000 and was not However, implemented 1998 it until;9 8 3 4 1 5 7 2

import sys
import itertools
from operator import itemgetter

def restore(words, indices):
  last_word = words[-1]
  words, indices = (list(x) for x in zip(*sorted(zip(words, indices), key = itemgetter(1))))
  if(indices[0] != 1):
    words.insert(0, last_word)
  else:
    for i in range(1, len(indices)):
      if(indices[i-1] != indices[i]-1):
        words.insert(i, last_word)
        break
    else:
      words.append(last_word)
      
  return ' '.join(words)

test_cases = open(sys.argv[1], 'r')
for test in test_cases:
  if test:
    s1,s2 = test.split(';')
    words = s1.split(' ')
    indices = [int(v) for v in s2.split(' ')]
    print restore(words, indices)
    
test_cases.close()


# s1,s2 = '2000 and was not However, implemented 1998 it until;9 8 3 4 1 5 7 2'.split(';')
# words = s1.split(' ')
# indices = [int(v) for v in s2.split(' ')]
# print restore(words, indices)