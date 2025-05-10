print('starting')



x = 123	    # x occupies cell 0 in memory
p = &x	    # p is a pointer, containing address of x
print(p)	# prints 0

s = input('Enter an interger for x>')
*p = int(s)

if x == 0:
{
  print("x is 0")
}
elif x == 1:
{
  x = x * 100
  print('x was initially 1')
  pass
  print('now its 100!')
}
elif x>0:
{
  print(x)
  *p = 12345
  print(x)
}
else:
{
  pass
  print("after 3 checks, x is negative!")
}

y = 456	    # y will end up in cell 2 of memory
p2 = &y	    # p2 is a pointer containing address of y
print(p2) 	# prints 2
print(x)

print('done')
