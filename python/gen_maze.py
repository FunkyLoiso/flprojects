from random import shuffle
import numpy
import sys

class Maze:
  wall = ord('*')
  floor = ord(' ')
  def __init__(self, width, height):
    self.lines = [bytearray([Maze.wall]*width) for _ in range(height)]
    self.height = height
    self.width = width
    
  def neighbour_walls(self, (x, y)):
    return [(x_, y_) for x_, y_
      in [(x, y-2), (x, y+2), (x-2, y), (x+2, y)]
      if  0 <= x_ < self.width and 
          0 <= y_ < self.height and
          self.is_wall((x_, y_))]
          
  def is_wall(self, (x, y)):
    return self.lines[y][x] is Maze.wall
    
  def set_floor(self, (x, y)):
    self.lines[y][x] = Maze.floor
      
def gen_maze(width, height, sx, fx):
  maze = Maze(width, height)
  maze.set_floor((sx, 1))
  stack = [(sx, 1)]
  
  while stack:
    cur = stack.pop()
    neighbours = maze.neighbour_walls(cur)
    shuffle(neighbours)
    for n in (n for n in neighbours if maze.is_wall(n)):
      mid = numpy.divide(numpy.add(cur, n), 2)
      maze.set_floor(mid)
      maze.set_floor(n)
      stack.append(n)
    
  maze.set_floor((sx, 0))
  maze.set_floor((fx, height-1))
  return maze
  
  
maze = gen_maze(101, 101, 1, 49)
for line in maze.lines:
  print line
