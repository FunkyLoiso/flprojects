import sys
import random
import copy
import numpy

import gameController as GC
from gameController import Field
from badBots import RandomBot, MinimizeBot, MaxScoreBot
from fineBot import FineBot
    
def testBot(count, bot):
  print 'goind to play %d games with bot %s' % (count, bot)
  results = []
  for game in range(count):
    score = 0
    f = Field()
    GC.addTile(f)
    GC.addTile(f)
    while f.getEmpty():
      action = bot.decide(copy.deepcopy(f))
      score += GC.doAction(f, action)
    results.append(score)
    sys.stdout.write('.')
 
  print '\nplayed %d games with bot %s\n(min, avg, max, stddev) is (%d, %f, %d, %s)\n' % (
    count, bot, numpy.amin(results), numpy.mean(results), numpy.amax(results), numpy.std(results))

def main():
  gameCount = 100
  # random.seed(2)
  # testBot(gameCount, RandomBot())
  # random.seed(2)
  # testBot(gameCount, MinimizeBot())
  # random.seed(2)
  # testBot(gameCount, MaxScoreBot())
  random.seed(2)
  testBot(gameCount, FineBot())

if __name__ == "__main__":
    main()
  