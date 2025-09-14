#include <stdint.h>
#include "isr.h"
#include "vga.h"
#include "portio.h"
#include "pic.h"
#include "pit.h"
#include "keyboard.h"

static void print_hex(uint32_t x) {
    static const char* H = "0123456789ABCDEF";
    char buf[11]; buf[0]='0'; buf[1]='x'; buf[10]='\0';
    for (int i=0;i<8;i++){ buf[9-i]=H[x&0xF]; x>>=4; }
    vga_write(buf);
}
static void dump_bytes(const uint8_t* p, int n){
    for (int i=0;i<n;i++){
        uint8_t b = p[i];
        static const char* H="0123456789ABCDEF";
        char s[3]; s[0]=H[(b>>4)&0xF]; s[1]=H[b&0xF]; s[2]=0;
        vga_write(s);
        vga_putc(i+1==n?'\n':' ');
    }
}

void isr_handler(regs_t* r){
    vga_write("\n[EXC] int="); print_hex(r->int_no);
    vga_write(" err="); print_hex(r->err_code);
    vga_write(" eip="); print_hex(r->eip);
    vga_write(" cs="); print_hex(r->cs);
    vga_write(" efl="); print_hex(r->eflags);
    vga_write("\nbytes: ");
    dump_bytes((const uint8_t*)r->eip, 8);
    vga_write("HALT\n");
    for(;;) __asm__ __volatile__("hlt");
}

void irq_handler(regs_t* r){
    switch (r->int_no) {
        case 32: pit_on_tick(); break;  
        case 33: kb_on_irq();   break;  
        default: break;
    }
    if (r->int_no >= 40) outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

