#ifndef VGA_H
#define VGA_H
#include <stdint.h>

void vga_init(uint8_t fg, uint8_t bg);
void vga_clear(void);
void vga_putc(char c);
void vga_write(const char* s);
void vga_setcolor(uint8_t fg, uint8_t bg);
void vga_newline(void);
void vga_move(int row,int col);
int  vga_cols(void);
int  vga_rows(void);

void vga_clear_region(int top, int bottom);
void vga_set_scroll_top(int top);

#endif

