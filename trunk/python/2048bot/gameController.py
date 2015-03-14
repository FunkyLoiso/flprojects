import random
import itertools

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
  
class Actions:  
  UP, DOWN, LEFT, RIGHT = 0, 1, 2, 3
 
# game controller functions 
def __move(line, action):
  out = [c for c in line if c != 0]
  if action in (Actions.LEFT, Actions.UP):
    return out + [0] * (len(line) - len(out))
  else:
    return [0] * (len(line) - len(out)) + out

def __collapse(line, action):
  if action in(Actions.LEFT, Actions.UP):
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
  
def addTile(field):
  emptyList = field.getEmpty()
  if emptyList:
    r, c = random.choice(emptyList)
    field.cells[r][c] = 2 if random.random() < 0.9 else 4;
    return True
  else:
    return False
  

def doAction(field, action, add=True):
  if action in (Actions.UP, Actions.DOWN):
    get = field.getCol
    set = field.setCol
  else:
    get = field.getRow
    set = field.setRow
    
  moved = False
  score = 0
  
  for i in range(field.size):
    orig = get(i)
    premoved = __move(orig, action)
    collapsed, pts = __collapse(premoved, action)
    new = __move(collapsed, action)
    if(orig != new):
      set(i, new)
      moved = True
      score += pts
      
  if moved and add:
    addTile(field)
    
  return score