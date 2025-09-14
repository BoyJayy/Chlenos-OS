#include <stdint.h>
#include "vga.h"
#include "portio.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"
#include "keyboard.h"
#include "console.h"
#include "ui.h"

static inline void cli(void){ __asm__ __volatile__("cli"); }
static inline void sti(void){ __asm__ __volatile__("sti"); }
static inline void hlt(void){ __asm__ __volatile__("hlt"); }

void kernel_main(void){
    cli();

    vga_init(15, 0);
    vga_clear();

    outb(0x21, 0xFF); outb(0xA1, 0xFF);
    idt_init();
    pic_remap();
    pit_init(100);
    kb_init();

    ui_draw_hub();

    sti();
    console_init();
    console_run();
    for(;;) hlt();
}

