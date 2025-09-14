#pragma once
#include <stdint.h>

static inline void outb(uint16_t p, uint8_t v){
    __asm__ __volatile__("outb %0,%1"::"a"(v),"Nd"(p));
}
static inline uint8_t inb(uint16_t p){
    uint8_t r; __asm__ __volatile__("inb %1,%0":"=a"(r):"Nd"(p)); return r;
}
static inline void io_wait(void){
    __asm__ __volatile__("outb %%al,$0x80"::"a"(0));
}

