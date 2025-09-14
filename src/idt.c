#include <stdint.h>
#include "idt.h"
#include "isr.h" 

static struct idt_entry idt[256];
static struct idt_ptr   idtp;

void idt_set_gate(int n, uint32_t base, uint16_t sel, uint8_t flags){
    idt[n].base_lo = base & 0xFFFF;
    idt[n].sel     = sel;
    idt[n].always0 = 0;
    idt[n].flags   = flags;
    idt[n].base_hi = (base >> 16) & 0xFFFF;
}

void idt_init(void){
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (uint32_t)&idt;

    for (int i = 0; i < 256; i++) idt_set_gate(i, 0, 0, 0);
    uint16_t kcs;
    __asm__ __volatile__("mov %%cs,%0" : "=r"(kcs));

    idt_set_gate(0,  (uint32_t)isr0,  kcs, 0x8E);
    idt_set_gate(1,  (uint32_t)isr1,  kcs, 0x8E);
    idt_set_gate(2,  (uint32_t)isr2,  kcs, 0x8E);
    idt_set_gate(3,  (uint32_t)isr3,  kcs, 0x8E);
    idt_set_gate(4,  (uint32_t)isr4,  kcs, 0x8E);
    idt_set_gate(5,  (uint32_t)isr5,  kcs, 0x8E);
    idt_set_gate(6,  (uint32_t)isr6,  kcs, 0x8E);
    idt_set_gate(7,  (uint32_t)isr7,  kcs, 0x8E);
    idt_set_gate(8,  (uint32_t)isr8,  kcs, 0x8E);
    idt_set_gate(9,  (uint32_t)isr9,  kcs, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, kcs, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, kcs, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, kcs, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, kcs, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, kcs, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, kcs, 0x8E);
    idt_set_gate(16, (uint32_t)isr16, kcs, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, kcs, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, kcs, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, kcs, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, kcs, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, kcs, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, kcs, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, kcs, 0x8E);
    idt_set_gate(24, (uint32_t)isr24, kcs, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, kcs, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, kcs, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, kcs, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, kcs, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, kcs, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, kcs, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, kcs, 0x8E);

    idt_set_gate(32, (uint32_t)irq0,  kcs, 0x8E);
    idt_set_gate(33, (uint32_t)irq1,  kcs, 0x8E);

    idt_load(&idtp);
}

