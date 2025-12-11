; this is a strict syntax x64 assembly dialect designed for embedded settings
; there are no repeat mnemonics. if the opcode is different, it is a different instruction.
; as of right now, hexadecimal is unsupported
:start
xor ax, ax
mov_ax 0x0010
int 0x10
jmpn 0
