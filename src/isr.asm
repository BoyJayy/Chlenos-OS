BITS 32
section .text

global  isr0,  isr1,  isr2,  isr3,  isr4,  isr5,  isr6,  isr7
global  isr8,  isr9,  isr10, isr11, isr12, isr13, isr14, isr15
global  isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23
global  isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31

global  irq0, irq1

global  idt_load

extern  isr_handler
extern  irq_handler

%macro PUSHALL 0
    pusha
    push ds
    push es
    push fs
    push gs
%endmacro

%macro POPALL 0
    pop gs
    pop fs
    pop es
    pop ds
    popa
%endmacro

%macro ISR_NOERR 1
global isr%1
isr%1:
    push dword 0      
    push dword %1        
    PUSHALL
    mov ax, ds
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax           
    call isr_handler
    add  esp, 4         
    POPALL
    add  esp, 8       
    iret
%endmacro

%macro ISR_ERR 1
global isr%1
isr%1:
    push dword %1        
    PUSHALL
    mov ax, ds
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    call isr_handler
    add  esp, 4
    POPALL
    add  esp, 8          
    iret
%endmacro

%macro IRQ 2
global irq%1
irq%1:
    push dword 0        
    push dword %2         
    PUSHALL
    mov ax, ds
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    call irq_handler
    add  esp, 4
    POPALL
    add  esp, 8
    iret
%endmacro

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR   8
ISR_NOERR 9
ISR_NOERR 10
ISR_NOERR 11
ISR_NOERR 12
ISR_NOERR 13
ISR_ERR   14
ISR_NOERR 15
ISR_NOERR 16
ISR_NOERR 17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31

IRQ 0,32
IRQ 1,33

idt_load:
    mov eax, [esp+4] 
    lidt [eax]
    ret

