#include <stdint.h>
#include "keyboard.h"
#include "portio.h"
#include "pic.h"

static const char keymap[128] = {
    0,27,'1','2','3','4','5','6','7','8','9','0','-','=',8,'\t',
    'q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
    'z','x','c','v','b','n','m',',','.','/',0,'*',0,' ',
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

#define KBUF_SZ 256
static volatile char kbuf[KBUF_SZ];
static volatile uint16_t khead=0, ktail=0;

void kb_init(void){}

static void kpush(char c){
    uint16_t n = (uint16_t)((khead+1) % KBUF_SZ);
    if(n != ktail){ kbuf[khead]=c; khead=n; }
}

int kb_haschar(void){
    return khead != ktail;
}

char kb_getchar(void){
    while(khead==ktail) { __asm__ __volatile__("hlt"); }
    char c = kbuf[ktail];
    ktail = (uint16_t)((ktail+1) % KBUF_SZ);
    return c;
}

void kb_on_irq(void){
    uint8_t sc = inb(0x60);
    if((sc & 0x80)==0){ 
        char c = keymap[sc];
        if(c=='\r') c='\n';
        if(c) kpush(c);
    }
    pic_eoi(1);
}

