import gmpy2
def fib_seq():
  n1, n2 = 0, 1
  while True:
    yield n1
    (n1, n2) = (n2, n1+n2)
    
for i, fib_n in enumerate(fib_seq()):
  if not i%100000:
    print i, ': ', gmpy2.mpfr(fib_n)