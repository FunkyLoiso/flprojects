numLetters = ['0', '1', 'ABC', 'DEF', 'GHI', 'JKL', 'MNO', 'PQRS', 'TUV', 'WXYZ']
#               0   1     2       3     4       5     6       7       8     9
def inc_string(numString, number):
  for i, (ch, dig) in enumerate(zip(numString, number)):
    if ch != numLetters[dig][-1]:
      numString[i] = chr(ord(ch) + 1)
      break;
    else:
      numString[i] = numLetters[dig][0]
  else:
    return False
    
  return True

def all_num_strings(number):
  numStr = [numLetters[dig][0] for dig in number]
  yield numStr
  while inc_string(numStr, number):
    yield numStr
  
if __name__ == '__main__':
  phone_number = [2, 5, 4, 6, 5, 8, 3]
  for i, phone_str in enumerate(all_num_strings(phone_number)):
    print ''.join(phone_str)
  print 'thant\'s all (%d variants)' % (i+1)
  