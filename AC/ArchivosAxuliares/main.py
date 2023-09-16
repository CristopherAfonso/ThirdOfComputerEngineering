import sys
from Runtime import Runtime

def main(args) -> None:
  if (len(args) < 2):
    raise Exception(f"Invalid number of args. Given {len(args)}")
  
  with open(args[1], 'r') as f:
    lines = [l.rstrip() for i, l in enumerate(f) if i > 0]

  if len(args) > 2:
    with open(args[2], 'r') as f:
      mem = [l.rstrip() for l in f]
  else:
    mem = []

  rn = Runtime(mem)

  print(f'Number of cicles: {rn.run(lines, False)}')
  print(rn.mem[400:656])

if __name__ == '__main__':
  main(sys.argv)