#include "ui.h"
#include "vga.h"

static int g_console_top = 18;   
int ui_console_top(void) { return g_console_top; }

static void draw_centered(const char* s, int row) {
    int cols = vga_cols();
    int len = 0; for (const char* p=s; *p; ++p) len++;
    int col = (cols - len) / 2; if (col < 0) col = 0;
    vga_move(row, col); vga_write(s);
}

static const char* const LOGO[] = {
" .o88b. db   db db      d88888b d8b   db  .d88b.  .d8888.       .d88b.  .d8888. ",
"d8P  Y8 88   88 88      88'     888o  88 .8P  Y8. 88'  YP      .8P  Y8. 88'  YP ",
"8P      88ooo88 88      88ooooo 88V8o 88 88    88 `8bo.        88    88 `8bo.   ",
"8b      88~~~88 88      88~~~~~ 88 V8o88 88    88   `Y8b.      88    88   `Y8b. ",
"Y8b  d8 88   88 88booo. 88.     88  V888 `8b  d8' db   8D      `8b  d8' db   8D ",
" `Y88P' YP   YP Y88888P Y88888P VP   V8P  `Y88P'  `8888Y'       `Y88P'  `8888Y' "
};

static void draw_logo(int top) {
    int n = (int)(sizeof(LOGO)/sizeof(LOGO[0]));
    vga_setcolor(11, 1);
    for (int i=0; i<n && (top+i) < vga_rows(); ++i) {
        draw_centered(LOGO[i], top + i);
    }
}

void ui_draw_hub(void) {
    vga_setcolor(15, 1);
    vga_clear();


    int logo_top = 3;
    draw_logo(logo_top);

    int title_row = logo_top + (int)(sizeof(LOGO)/sizeof(LOGO[0])) + 1;
    if (title_row < vga_rows()){
        vga_setcolor(14,1);
        draw_centered("Chlenos OS hub", title_row);
    }

    int menu_row = title_row + 2;
    if (menu_row < vga_rows()){
        vga_setcolor(11,1);
        vga_move(menu_row, 2);
        vga_write("[1] Console   [2] About   [3] Help");
    }

    int console_row = menu_row + 2;
    if (console_row >= vga_rows()) console_row = vga_rows() - 1;
    vga_setcolor(15,1);
    vga_move(console_row, 0);
    vga_write("Console:\n");
    g_console_top = console_row + 1; 
    vga_move(g_console_top, 0);
    vga_set_scroll_top(g_console_top);
}

