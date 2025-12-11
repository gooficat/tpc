; this is a strict syntax x64 assembly dialect designed for embedded settings
; there are no repeat mnemonics. if the opcode is different, it is a different instruction.
; as of right now, hexadecimal is unsupported
:start
xor ax, ax
mov_ax 0x1310
int 0x10
jmpn 0xff
.times 501, .db 0x67
.db 0x55
.db 0xaa