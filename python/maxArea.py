import random

# 4                                                                                                                
# 3  *** **                                                                                                              
# 2  *** ****                                                                                                               
# 1 **** ****                                                                                                               
# 0 **********                                                                                                                
#   0123456789      
##############                                                                                                          
heights = [2, 4, 4, 4, 1, 4, 4, 3, 3, 1]

def printHeights(heights):
  print '123456789'
  for i, h in enumerate(heights):
    print '*' * h
  print '123456789'

def biggestArea(heights):
  maxArea = 0
  curSquareLen = [0] * max(heights)
  
  for columnH in heights:
    for squareH in xrange(columnH):
      # increment length for hights that are lower or equal
      curSquareLen[squareH] += 1
    for squareH in xrange(columnH, len(curSquareLen)):
      # for higher hights check area and zero length
      area = curSquareLen[squareH] * (squareH+1)
      if area > maxArea:
        maxArea = area
      curSquareLen[squareH] = 0
    # print 'Column height = %d, maxArea = %d\n\t square lengths: %s' % (columnH, maxArea, str(curSquareLen))
    
  # final area calculation
  for h, l in enumerate(curSquareLen):
    area = l * (h+1)
    if area > maxArea:
      maxArea = area
  return maxArea
  
if __name__ == '__main__':
  random.seed()
  heights = [random.randint(0, 7) for r in xrange(10)]
  # heights = [1, 2, 3, 4, 5, 6, 7, 8, 7 ,6 ,5 ,4 ,3 ,2 ,1]
  printHeights(heights)
  result = biggestArea(heights)
  print 'Biggest area = ', result