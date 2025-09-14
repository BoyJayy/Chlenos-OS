#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdint.h>

void kb_init(void);
void kb_on_irq(void);
int  kb_haschar(void);
char kb_getchar(void);

#endif

