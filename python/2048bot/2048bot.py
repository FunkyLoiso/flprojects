import sys
import random
import copy

import gameController as GC
from gameController import Field
from badBots import RandomBot, MinimizeBot, MaxScoreBot
    
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

  avg = sum(results) / float(len(results))  
  print '\nplayed %d games with bot %s\n(min, avg, max) is (%d, %f, %d)\n' % (count, bot, min(results), avg, max(results))

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
  