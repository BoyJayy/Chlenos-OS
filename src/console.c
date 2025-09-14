#include <stdint.h>
#include "vga.h"
#include "pit.h"
#include "keyboard.h"
#include "ui.h"
#include "cpu.h"
// мне впадлу опись писать сами разберетесь
static int streq(const char* a, const char* b){
    while(*a && *b && *a==*b){ a++; b++; }
    return *a==0 && *b==0;
}

static int starts(const char* s, const char* p){
    while(*p && *s==*p){ s++; p++; }
    return *p==0;
}

static const char* lstrip(const char* s){
    while(*s==' ' || *s=='\t') s++;
    return s;
}

static void print_hex32(uint32_t x){
    static const char* H="0123456789ABCDEF";
    char s[11]; s[0]='0'; s[1]='x'; s[10]=0;
    for(int i=0;i<8;i++){ s[9-i]=H[x&0xF]; x>>=4; }
    vga_write(s);
}

static void print_dec32(uint32_t v){
    static const uint32_t pow10[10] = {
        1000000000u,100000000u,10000000u,1000000u,100000u,
        10000u,1000u,100u,10u,1u
    };
    int started = 0;
    for(int k=0;k<10;k++){
        uint32_t p = pow10[k];
        int digit = 0;
        while(v >= p){ v -= p; digit++; }
        if(digit || started || k==9){ vga_putc('0'+digit); started = 1; }
    }
}

static void prompt(void){ vga_write("\n> "); }

static void cmd_about(void){
    vga_write("Chlenos OS\n");
    vga_write("Author: boyjayy\n");
    vga_write("Build: " __DATE__ " " __TIME__ "\n");
    vga_write("Screen: ");
    print_dec32((uint32_t)vga_cols()); vga_write("x");
    print_dec32((uint32_t)vga_rows()); vga_write(" text mode\n");
    vga_write("Timer: PIT 100 Hz (10 ms per tick)\n");
    vga_write("Keyboard: PS/2\n");
    vga_write("Use 'sys' for CPU info\n");
}

static void cmd_ticks(void){
    uint32_t t = (uint32_t)pit_ticks();
    vga_write("ticks="); print_dec32(t); vga_putc('\n');
}
__attribute__((noinline,optimize("O0")))
static void split_ms(uint32_t in_ms, uint32_t* sec_out, uint32_t* ms_out){
    uint32_t ms = in_ms, sec = 0;
    while (ms >= 1000u) { ms -= 1000u; sec++; }
    *sec_out = sec; *ms_out = ms;
}

static void cmd_time(void){
    uint32_t t  = (uint32_t)pit_ticks();
    uint32_t ms = t * 10u;
    uint32_t sec, rem; split_ms(ms, &sec, &rem);

    vga_write("uptime: ");
    print_dec32(sec*1000u + rem); vga_write(" ms (");
    print_dec32(sec); vga_write(" s)\n");
}

static int dec_parse32(const char* s, uint32_t* out){
    uint32_t v=0; int any=0;
    s = lstrip(s);
    while(*s>='0' && *s<='9'){
        v = (v<<3) + (v<<1) + (uint32_t)(*s - '0'); // v*10 + digit
        s++; any=1;
    }
    s = lstrip(s);
    if(!any || *s) return 0;
    *out=v; return 1;
}

static void cmd_hex(const char* args){
    uint32_t v;
    if(!dec_parse32(args, &v)){ vga_write("usage: hex <decimal>\n"); return; }
    vga_write("hex="); print_hex32(v); vga_putc('\n');
}

static void decode_cpuver(uint32_t ver, uint32_t* fam, uint32_t* model, uint32_t* stepping){
    uint32_t stepping_id =  ver        & 0xF;
    uint32_t model_id    = (ver >> 4)  & 0xF;
    uint32_t family_id   = (ver >> 8)  & 0xF;
    uint32_t ext_model   = (ver >> 16) & 0xF;
    uint32_t ext_family  = (ver >> 20) & 0xFF;

    uint32_t real_family = family_id;
    if (family_id == 0xF) real_family += ext_family;
    uint32_t real_model = model_id;
    if (family_id == 0x6 || family_id == 0xF) real_model += (ext_model << 4);

    if(fam) *fam = real_family;
    if(model) *model = real_model;
    if(stepping) *stepping = stepping_id;
}

static void print_yesno(int v){ vga_write(v ? "yes" : "no"); }

static void cmd_sys(void){
    char vend[13]; cpu_get_vendor(vend);
    uint32_t ver = cpu_get_version();
    uint32_t fam, model, step; decode_cpuver(ver, &fam, &model, &step);

    vga_write("CPU vendor: "); vga_write(vend); vga_putc('\n');
    vga_write("fam="); print_dec32(fam);
    vga_write(" model="); print_dec32(model);
    vga_write(" stepping="); print_dec32(step); vga_putc('\n');

    uint32_t edx = cpu_get_features_edx();
    uint32_t ecx = cpu_get_features_ecx();

    vga_write("Features: ");
    vga_write("TSC=");   print_yesno((edx>>4)&1);  vga_write("  ");
    vga_write("MSR=");   print_yesno((edx>>5)&1);  vga_write("  ");
    vga_write("CMOV=");  print_yesno((edx>>15)&1); vga_write("  ");
    vga_write("MMX=");   print_yesno((edx>>23)&1); vga_write("  ");
    vga_write("SSE=");   print_yesno((edx>>25)&1); vga_write("  ");
    vga_write("SSE2=");  print_yesno((edx>>26)&1); vga_write("  ");
    vga_write("SSE3=");  print_yesno((ecx>>0)&1);  vga_write("  ");
    vga_write("SSSE3="); print_yesno((ecx>>9)&1);  vga_write("  ");
    vga_write("SSE4.1=");print_yesno((ecx>>19)&1); vga_write("  ");
    vga_write("SSE4.2=");print_yesno((ecx>>20)&1); vga_putc('\n');

    vga_write("Hypervisor: ");
    if (cpu_has_hypervisor()){
        char hv[13]; cpu_get_hv_vendor(hv);
        vga_write("yes ("); vga_write(hv); vga_write(")\n");
    } else {
        vga_write("no\n");
    }
}

static void cmd_print(const char* args){
    const char* s = lstrip(args);
    if(*s == '\0'){ vga_write("usage: print <text>\n"); return; }
    vga_write(s); vga_putc('\n');
}


void console_clear(void){
    int top = ui_console_top();
    vga_setcolor(15,1);
    vga_clear_region(top, vga_rows()-1);
    vga_move(top, 0);
}


static void cmd_help(void){
    vga_write("commands:\n");
    vga_write("  help         - this help\n");
    vga_write("  clear        - clear console area (keep header)\n");
    vga_write("  ticks        - show PIT ticks (32-bit)\n");
    vga_write("  time         - show uptime in ms/sec\n");
    vga_write("  hex <num>    - print number in hex (decimal input)\n");
    vga_write("  print <text> - echo text\n");
    vga_write("  about        - about this OS\n");
    vga_write("  sys          - CPU & environment info\n");
}

void console_init(void){
    vga_move(ui_console_top(), 0);
    vga_write("\nType something:\n");
    prompt();
}

void console_run(void){
    char line[128]; int len=0;
    for(;;){
        if(!kb_haschar()){ __asm__ __volatile__("hlt"); continue; }
        char c = kb_getchar();

        if(c=='\r' || c=='\n'){
            vga_putc('\n'); line[len]='\0';

            if(len==0){  }
            else if(streq(line,"help"))      cmd_help();
            else if(streq(line,"clear"))     console_clear();
            else if(streq(line,"ticks"))     cmd_ticks();
            else if(streq(line,"time"))      cmd_time();
            else if(starts(line,"hex "))     cmd_hex(line+4);
            else if(starts(line,"print "))   cmd_print(line+6);
            else if(streq(line,"about"))     cmd_about();
            else if(streq(line,"sys") || streq(line,"sysinfo")) cmd_sys();
            else { vga_write("unknown: "); vga_write(line); vga_putc('\n'); }

            len=0; prompt(); continue;
        }

        if(c==8){ if(len>0){ len--; vga_write("\b \b"); } continue; }

        if((unsigned char)c>=32 && (unsigned char)c<127){
            if(len < (int)sizeof(line)-1){ line[len++]=c; vga_putc(c); }
        }
    }
}

