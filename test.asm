; this is a strict syntax x64 assembly dialect designed for embedded settings
; there are no repeat mnemonics. if the opcode is different, it is a different instruction.
; as of right now, hexadecimal is unsupported
xor ax, ax
mov_ax 4105
int 16
jmpb 0
; fill is a special instruction, which repeats the first argument for the number of times specified in the second one.
fill 0, 500
fill 170, 1
fill 85, 1