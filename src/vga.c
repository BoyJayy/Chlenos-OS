#include "vga.h"

#define VGA_MEM  ((volatile uint16_t*)0xB8000)
#define VGA_COLS 80
#define VGA_ROWS 25

static uint8_t fgc, bgc;
static int cx, cy;
static int scroll_top = 0; 

static inline uint8_t color(void){ return (fgc | (bgc << 4)); }
static inline uint16_t entry(char c){ return (uint16_t)c | ((uint16_t)color() << 8); }

static void do_scroll(void){
    if (cy < VGA_ROWS) return;

    int top = scroll_top;
    if (top < 0) top = 0;
    if (top >= VGA_ROWS) top = VGA_ROWS - 1;
    for (int y = top + 1; y < VGA_ROWS; y++){
        for (int x = 0; x < VGA_COLS; x++){
            VGA_MEM[(y-1)*VGA_COLS + x] = VGA_MEM[y*VGA_COLS + x];
        }
    }
    for (int x = 0; x < VGA_COLS; x++){
        VGA_MEM[(VGA_ROWS-1)*VGA_COLS + x] = entry(' ');
    }
    cy = VGA_ROWS - 1;
    cx = 0;
}

void vga_init(uint8_t fg, uint8_t bg){
    fgc = fg; bgc = bg; cx = 0; cy = 0; scroll_top = 0;
    for (int y = 0; y < VGA_ROWS; y++)
        for (int x = 0; x < VGA_COLS; x++)
            VGA_MEM[y*VGA_COLS + x] = entry(' ');
}

void vga_clear(void){
    cx = 0; cy = 0;
    for (int y = 0; y < VGA_ROWS; y++)
        for (int x = 0; x < VGA_COLS; x++)
            VGA_MEM[y*VGA_COLS + x] = entry(' ');
}

void vga_clear_region(int top, int bottom){
    if (top < 0) top = 0;
    if (bottom >= VGA_ROWS) bottom = VGA_ROWS - 1;
    if (bottom < top) return;

    for (int y = top; y <= bottom; y++){
        for (int x = 0; x < VGA_COLS; x++)
            VGA_MEM[y*VGA_COLS + x] = entry(' ');
    }
}

void vga_setcolor(uint8_t fg, uint8_t bg){ fgc = fg; bgc = bg; }

void vga_newline(void){ cx = 0; cy++; do_scroll(); }

void vga_putc(char c){
    if (c == '\n'){ vga_newline(); return; }
    if (c == '\r'){ cx = 0; return; }
    if (c == '\b'){ 
        if (cx > 0){ cx--; VGA_MEM[cy*VGA_COLS + cx] = entry(' '); }
        return;
    }
    if (cx >= VGA_COLS){ cx = 0; cy++; do_scroll(); }
    VGA_MEM[cy*VGA_COLS + cx] = entry(c);
    cx++;
    if (cx >= VGA_COLS){ cx = 0; cy++; do_scroll(); }
}

void vga_write(const char* s){ while (*s) vga_putc(*s++); }

void vga_move(int row, int col){
    if (row < 0) row = 0;
    if (col < 0) col = 0;
    if (row >= VGA_ROWS) row = VGA_ROWS - 1;
    if (col >= VGA_COLS) col = VGA_COLS - 1;
    cy = row; cx = col;
}

int vga_cols(void){ return VGA_COLS; }
int vga_rows(void){ return VGA_ROWS; }

void vga_set_scroll_top(int top){
    if (top < 0) top = 0;
    if (top >= VGA_ROWS) top = VGA_ROWS - 1;
    scroll_top = top;
}

