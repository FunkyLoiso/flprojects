import copy

class Node:
  def __init__(self, v, p = None, l = None, r = None):
    self.v = v
    self.p = p
    self.l = l
    self.r = r

  def __str__(self):
    l = str(self.l) if self.l else ''
    r = str(self.r) if self.r else ''
    return '%s(%s,%s)' % (str(self.v), l, r)
    
  def root(self):
    while self.p:
      self = self.p
    return self
    
    
def gen_trees(node, count):
  if(count == 0):
    print str(node.root())
  else:
    node.l = Node(count, node)
    gen_trees(node.l, count-1)
        
    node.l = None
    node.r = Node(count, node)
    gen_trees(node.r, count-1)
    
    node.r = None
    if(count > 1):
      node.l = Node(count, node)
      node.r = Node(count-1, node)
      gen_trees(node.l, count-2)
      if(count > 2):
        gen_trees(node.r, count-2)
          
gen_trees(Node('root'), 2)