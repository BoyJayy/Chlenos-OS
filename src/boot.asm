BITS 32

%define MB_MAGIC     0x1BADB002
%define MB_FLAGS     0x00000000
%define MB_CHECKSUM  -(MB_MAGIC + MB_FLAGS)

section .multiboot
align 4
    dd MB_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM

section .text
align 16
global _start
extern kernel_main

_start:
    mov esp, stack_top
    call kernel_main

.hang:
    hlt
    jmp .hang

section .bss
align 16
stack_bottom:
    resb 16384        
stack_top:

