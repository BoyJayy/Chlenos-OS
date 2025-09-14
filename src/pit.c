#include "pit.h"
#include "portio.h"
#include "pic.h"

static volatile uint64_t ticks = 0;

void pit_init(uint32_t freq){
    if (freq == 0) freq = 100;
    uint32_t div = 1193182 / freq;
    outb(0x43, 0x36);
    outb(0x40, div & 0xFF);
    outb(0x40, (div >> 8) & 0xFF);
}

void pit_on_tick(void){
    ticks++;
    (void)ticks;
    pic_eoi(0);
}

uint64_t pit_ticks(void){ return ticks; }

