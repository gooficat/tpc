; this is a strict syntax x64 assembly dialect designed for embedded settings
; as of right now, hexadecimal is unsupported
; there are no indirect accesses yet
;
; label. it does nothing here but is nonetheless a demostration of syntax
:start
jmpn 4
.db 0x48
.db 0x44
.db 0x48
.db 0x44
; empty lines are not allowed
; there are no repeat mnemonics. if the opcode is different, it is a different instruction.
;
mov_ah 0x00
mov_al 0x07
;
int 0x10
mov_ah 0x13
mov_cx 0x0004
mov_bx 0x0000
mov_seg es, bx
mov_bp 0x0000
mov_dl 0
int 0x10
;
hlt
.times 480, .db 0
.db 0x55
.db 0xaa