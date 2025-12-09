from instructions import instructions, registers

content : str ="""\
mov bx 5
push bx
pop ax
retn\
"""

lines : list = content.splitlines()

output : bytearray = bytearray()

class Operand:
    def __init__(self):
        self.type : str = ''
        self.value : bytearray = bytearray()
        self.min_bytes : int = 0
        self.offset : int = 0

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
        break

    ops : list[Operand] = []

    for tk in tokens[1:]:
        if tk: ops.append(gen_operand(tk))

    
    if len(ops) == 2: # 2 operands, like "mov ax, [sp+4]"
        pass
    elif len(ops) == 1: # 1 operand, like "push ax" or "call 0x1212"
        if ops[0].type == 'reg':
            instruction[0] += ops[0].value[1] # add reg directly to operand
        elif ops[0].type == 'imm':
            instruction.extend(ops[0].value)

    # otherwise, there are no operands, like "ret"

    output.extend(instruction)


for byte in output:
    print(hex(byte))