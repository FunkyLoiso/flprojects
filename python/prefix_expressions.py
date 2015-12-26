from operator import add, mul, div
import sys


def operation(code):
  return {'+': add,
          '*': mul,
          '/': div}[code]


def evaluate(st):
  # print st
  st[-3] = operation(st[-3])(float(st[-2]), float(st[-1]))
  del st[-2:]


def is_number(token):
  return isinstance(token, float) or (token not in "+*/")


# test_cases = open(sys.argv[1], 'r')
test_cases = ["* + 2 3 4", "+ + 1 2 3", "+ * / 2 3 3 10"]

for expr in test_cases:
  numbers = 0
  stack = []
  for token in expr.split(' '):
    stack.append(token)
    if len(stack) > 2 and is_number(stack[-1]) and is_number(stack[-2]):
      evaluate(stack)
  assert len(stack) == 1
  print(int(stack[0]))
