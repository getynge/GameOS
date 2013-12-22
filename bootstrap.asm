global _start

MBALIGN	EQU	1<<0
MEMINFO	EQU	1<<1
FLAGS	EQU	MBALIGN | MEMINFO
MAGIC	EQU	0x1badb002
CHECKSUM EQU -(MAGIC + FLAGS)

section .multiboot
align 4
	dd	MAGIC
	dd	FLAGS
	dd	CHECKSUM
	
section .bootstrap_stack
STACKSIZE equ 0x8000	;set up a 32k stack
	
section .text

_start:
	MOV	esp, stack + STACKSIZE
	HLT					;Will be removed shortly, only there for register debugging
	EXTERN  _kmain
	CALL	_kmain
	
_safety_loop:
	CLI
	HLT
	JMP	_safety_loop
	
section .bss
align 4
stack: resb STACKSIZE
magic: resd 1
mbd:   resd 1