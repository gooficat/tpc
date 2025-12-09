from instructions import instructions, registers

content : str ="""add ax 5"""

lines : list = content.splitlines()

output : bytearray = bytearray()

class Operand:
    def __init__(self):
        self.type : str = ''
        self.value : bytearray = bytearray()
        self.min_bytes : int = 0

def gen_operand(token : str) -> Operand:
    out : Operand = Operand()

    for reg in registers:
        if token == reg[0]:
            out.type += "reg"
            out.value.append(reg[1])
            out.min_bytes = reg[2]
            return out
    
    if token.isdigit():
        out.type = "imm"
        value : int = int(token)
        out.value.extend(value.to_bytes(2, 'little'))
        if value < 256:
            out.min_bytes = 1
        else:
            out.min_bytes = 2


    return out

for line in lines:
    tokens : list[str] = line.split()
    instruction : bytearray = bytearray()

    for instr in instructions:
        if tokens[0] == instr[0]: instruction.append(instr[1])

    match(len(tokens)):
        case 3:
            break
        case 2:
            operand : Operand = gen_operand(tokens[1])
            if (operand.type == 'reg'):
                instruction.extend(operand.value)
            break
        case 1:
            ops : list[Operand] = [
                gen_operand(tokens[1]),
                gen_operand(tokens[2])
            ]
            if (ops[0].type == 'reg'):
                pass
            break


    output.extend(instruction)


for byte in output:
    print(hex(byte))