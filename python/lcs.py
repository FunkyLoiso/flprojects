import sys
import itertools

cache = {}

def lcs(s1, s2):
  if not s1 or not s2:
    return ''
  if (s1, s2) in cache:
    return cache[(s1, s2)]
  if s1[-1] == s2[-1]:
    result = lcs(s1[:-1], s2[:-1]) + s1[-1]
  else:
    r1 = lcs(s1[:-1], s2)
    r2 = lcs(s1, s2[:-1])
    result = r1 if len(r1) > len(r2) else r2
  cache[(s1, s2)] = result
  return result

test_cases = open(sys.argv[1], 'r')
for test in test_cases:
    if test:
        s1,s2 = test.split(';')
        lcs(s1, s2)
        cache.clear()
test_cases.close()
