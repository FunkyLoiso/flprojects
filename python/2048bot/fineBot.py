import random
import itertools
import copy
import operator
import numpy

import gameController as GC
from gameController import Actions

def fieldStdDev(field):
  return numpy.std( [v for v in (row for row in field.cells)] )

class FineBot:
  def __init__(self):
    self.gen = random.Random()
    self.gen.seed()
    
  def decide(self, field):
    results = []
    for action in [Actions.UP, Actions.DOWN, Actions.LEFT, Actions.RIGHT]:
      fieldCopy = copy.deepcopy(field)
      emptyCount = len(fieldCopy.getEmpty)
      GC.doAction(fieldCopy, action, add=False)
      if(fieldCopy.cells != field.cells):
        results.append( ( fieldStdDev(fieldCopy), action) )
      
    f = operator.itemgetter(0)
    print results
    results = list(next(itertools.groupby(sorted(results, key=f, reverse=False), key=f))[1])
    print self.gen.choice(results)[1]
    return self.gen.choice(results)[1]
    