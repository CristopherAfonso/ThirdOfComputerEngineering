from dataclasses import dataclass

@dataclass
class Runtime():
  mem = [0 for _ in range(1024)]
  MNEMONICS = {}
  labels = {}
  registers = {f'R{i}': 0 for i in range(64)}

  def __init__(self, mem: list[str]) -> None:
    self.MNEMONICS = {
      'ADDI': [self.__add, 1],
      'LW': [self.__lw, 4],
      'SW': [self.__sw, 4],
      'MULT': [self.__mult, 2],
      'ADD': [self.__add, 1],
      'SUB': [self.__sub, 1],
      'BNE': [self.__bne, 2],
      'BGT': [self.__bgt, 2],
    }

    for line in mem:
      dir, values = line.split('] ')
      dir = int(dir.removeprefix('['))
      values = values.split(' ')
      for i, val in enumerate(values):
        self.mem[dir + i] = int(val.split('.')[0])

  def __lw(self, inm, dir) -> int:
    return self.__mem[inm + dir]

  def __mult(self, num_1, num_2) -> int:
    return num_1 * num_2

  def __add(self, num_1, num_2) -> int:
    return num_1 + num_2

  def __sub(self, num_1, num_2) -> int:
    return num_1 - num_2

  def __sw(self, inm, dir, data) -> None:
    self.__mem[dir + inm] = data

  def __bne(self, num_1, num_2) -> bool:
    return num_1 != num_2
  
  def __bgt(self, num_1, num_2) -> bool:
    return num_1 > num_2

  def run(self, file: list[str], debug: bool = True) -> int:
    start = 0
    cicles = 0
    j = 0

    while True:
      jump = False
      for i in range(start, len(file)):
        line = file[i].strip()

        if (opcode := line.split(' ')[0]) in self.MNEMONICS:
          j += 1
          cicles += self.MNEMONICS[opcode][1]
          op_1, op_2 = line.split(' ')[1:3]
          op_3 = None if len(line.split(' ')) <= 3 else line.split(' ')[3]
          if debug: print(self.MNEMONICS[opcode][1], opcode, op_1, op_2, op_3)

          if opcode == 'BNE' or opcode == 'BGT':
            if self.MNEMONICS[opcode][0](self.registers[op_1], self.registers[op_2]):
              start = self.labels[op_3]
              jump = True
              break
          elif opcode == 'LW':
            inm = int(op_2.split('(')[0])
            reg = op_2.split('(')[1].removesuffix(')')
            self.registers[op_1] = self.mem[inm + self.registers[reg]]
          elif opcode == 'SW':
            inm = int(op_2.split('(')[0])
            reg = op_2.split('(')[1].removesuffix(')')
            self.mem[inm + self.registers[reg]] = self.registers[op_1]
          else:
            if not op_3:
              raise Exception(f"El operando 3 no puede ser nulo\n  Line {i}: {line}")
            
            if opcode == 'ADDI':
              self.registers[op_1] = self.MNEMONICS[opcode][0](
                self.registers[op_2],
                int(op_3.removeprefix('#'))
              )
            else:
              self.registers[op_1] = self.MNEMONICS[opcode][0](
                self.registers[op_2],
                self.registers[op_3]
              )
        elif line and not line.strip().startswith('//'):
          self.labels[line.strip().removesuffix(':')] = i + 1

      if not jump:
        return cicles
