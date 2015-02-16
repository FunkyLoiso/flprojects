import sys
import random
import itertools
import copy
import operator

class Field:
  def __init__(self, size=4):
    self.size = size
    self.cells = [[0] * size for _ in range(size)]

  def __str__(self):
    out = ''
    for row in self.cells:
      for cell in row:
        out += ('%4d, ' % cell)
      out += '\b\b\n'
    return out
    
  def getRow(self, row):
    return self.cells[row]
    
  def setRow(self, row, rowArr):
    self.cells[row] = rowArr
  
  def getCol(self, col):
    return [ self.cells[row][col] for row in range(self.size) ]
    
  def setCol(self, col, colArr):
    for row in range(self.size):
      self.cells[row][col] = colArr[row]
      
  def getEmpty(self):
    return [(r, c) for r, c in itertools.product(range(self.size), range(self.size)) if self.cells[r][c] == 0]
    
class GC:
  UP, DOWN, LEFT, RIGHT = 0, 1, 2, 3
  
  def move(self, line, action):
    out = [c for c in line if c != 0]
    if action in (GC.LEFT, GC.UP):
      return out + [0] * (len(line) - len(out))
    else:
      return [0] * (len(line) - len(out)) + out
  
  def collapse(self, line, action):
    if action in(GC.LEFT, GC.UP):
      inc = 1
      rg = xrange(0, len(line)-1, inc)
    else:
      inc = -1
      rg = xrange(len(line)-1, 0, inc)

    pts = 0
    for i in rg:
      if line[i] == 0:
        continue
      if line[i] == line[i+inc]:
        newVal = line[i]*2
        line[i] = newVal
        line[i+inc] = 0
        pts += newVal

    return (line, pts)
    
  def addTile(self, field):
    emptyList = field.getEmpty()
    if emptyList:
      r, c = random.choice(emptyList)
      field.cells[r][c] = 2
      return True
    else:
      return False
    

  def doAction(self, field, action):
    if action in (GC.UP, GC.DOWN):
      get = field.getCol
      set = field.setCol
    else:
      get = field.getRow
      set = field.setRow
      
    moved = False
    score = 0
    
    for i in range(field.size):
      orig = get(i)
      premoved = self.move(orig, action)
      collapsed, pts = self.collapse(premoved, action)
      new = self.move(collapsed, action)
      if(orig != new):
        set(i, new)
        moved = True
        score += pts
        
    if moved:
      self.addTile(field)
      
    return score
    
class RandomBot:
  def __init__(self):
    self.gen = random.Random()
    self.gen.seed()
  
  def decide(self, field):
    return self.gen.choice([GC.UP, GC.DOWN, GC.LEFT, GC.RIGHT])
    
class MinimizeBot:
  def __init__(self):
    self.gen = random.Random()
    self.gen.seed()
    
  def decide(self, field):
    gc = GC()
    results = []
    for action in [GC.UP, GC.DOWN, GC.LEFT, GC.RIGHT]:
      fieldCopy = copy.deepcopy(field)
      gc.doAction(fieldCopy, action)
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
    gc = GC()
    results = []
    for action in [GC.UP, GC.DOWN, GC.LEFT, GC.RIGHT]:
      fieldCopy = copy.deepcopy(field)
      score = gc.doAction(fieldCopy, action)
      if(fieldCopy.cells != field.cells):
        results.append( (score, action) )
      
    f = operator.itemgetter(0)
    results = list(next(itertools.groupby(sorted(results, key=f, reverse=True), key=f))[1])
    return self.gen.choice(results)[1]
    
def testBot(count, bot):
  print 'goind to play %d games with bot %s' % (count, bot)
  gc = GC()
  results = []
  for game in range(count):
    score = 0
    f = Field()
    gc.addTile(f)
    gc.addTile(f)
    while f.getEmpty():
      action = bot.decide(copy.deepcopy(f))
      score += gc.doAction(f, action)
    results.append(score)
    sys.stdout.write('.')

  avg = sum(results) / float(len(results))
  print '\nplayed %d games with bot %s\navg score is %f\n' % (count, bot, avg)

def main():
  gameCount = 100
  random.seed(2)
  testBot(gameCount, RandomBot())
  random.seed(2)
  testBot(gameCount, MinimizeBot())
  random.seed(2)
  testBot(gameCount, MaxScoreBot())

if __name__ == "__main__":
    main()
  