import random
import itertools
import copy
import operator

import gameController as GC
from gameController import Actions

class RandomBot:
  def __init__(self):
    self.gen = random.Random()
    self.gen.seed()
  
  def decide(self, field):
    return self.gen.choice([Actions.UP, Actions.DOWN, Actions.LEFT, Actions.RIGHT])
    
class MinimizeBot:
  def __init__(self):
    self.gen = random.Random()
    self.gen.seed()
    
  def decide(self, field):
    results = []
    for action in [Actions.UP, Actions.DOWN, Actions.LEFT, Actions.RIGHT]:
      fieldCopy = copy.deepcopy(field)
      GC.doAction(fieldCopy, action)
      if(fieldCopy.cells != field.cells):
        results.append( (len(fieldCopy.getEmpty()), action) )
      
    f = operator.itemgetter(0)
    results = list(next(itertools.groupby(sorted(results, key=f, reverse=True), key=f))[1])
    return self.gen.choice(results)[1]
    
class MaxScoreBot:
  def __init__(self):
    self.gen = random.Random()
    self.gen.seed()
    
  def decide(self, field):
    results = []
    for action in [Actions.UP, Actions.DOWN, Actions.LEFT, Actions.RIGHT]:
      fieldCopy = copy.deepcopy(field)
      score = GC.doAction(fieldCopy, action)
      if(fieldCopy.cells != field.cells):
        results.append( (score, action) )
      
    f = operator.itemgetter(0)
    results = list(next(itertools.groupby(sorted(results, key=f, reverse=True), key=f))[1])
    return self.gen.choice(results)[1]