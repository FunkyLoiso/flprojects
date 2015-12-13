from operator import add, mul, div
import sys


def operation(code):
  return {'+': add,
          '*': mul,
          '/': div}[code]


def evaluate(st):
  # print st
  st[-3] = operation(st[-3])(int(st[-2]), int(st[-1]))
  del st[-2:]


def is_number(token):
  return isinstance(token, int) or (token not in "+*/")


test_cases = open(sys.argv[1], 'r')
# test_cases = ["* + 2 3 4", "+ + 1 2 3"]

for expr in test_cases:
  numbers = 0
  stack = []
  for token in expr.split(' '):
    stack.append(token)
    if len(stack) > 2 and is_number(stack[-1]) and is_number(stack[-2]):
      evaluate(stack)
  assert len(stack) == 1
  print(stack[0])
