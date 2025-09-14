#include "cpu.h"

static void cpuid(uint32_t leaf, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d){
    uint32_t eax, ebx, ecx, edx;
    __asm__ __volatile__(
        "cpuid"
        : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
        : "a"(leaf), "c"(0)
    );
    if(a) *a = eax; if(b) *b = ebx; if(c) *c = ecx; if(d) *d = edx;
}

void cpu_get_vendor(char out[13]){
    uint32_t a,b,c,d; cpuid(0, &a,&b,&c,&d);
    out[0]=b; out[1]=b>>8; out[2]=b>>16; out[3]=b>>24;
    out[4]=d; out[5]=d>>8; out[6]=d>>16; out[7]=d>>24;
    out[8]=c; out[9]=c>>8; out[10]=c>>16; out[11]=c>>24;
    out[12]='\0';
}

uint32_t cpu_get_version(){ uint32_t a; cpuid(1,&a,0,0,0); return a; }
uint32_t cpu_get_features_ecx(){ uint32_t a,b,c,d; cpuid(1,&a,&b,&c,&d); return c; }
uint32_t cpu_get_features_edx(){ uint32_t a,b,c,d; cpuid(1,&a,&b,&c,&d); return d; }

int cpu_has_hypervisor(){ return (cpu_get_features_ecx() >> 31) & 1; }

void cpu_get_hv_vendor(char out[13]){
    uint32_t a,b,c,d; cpuid(0x40000000u, &a,&b,&c,&d);
    out[0]=b; out[1]=b>>8; out[2]=b>>16; out[3]=b>>24;
    out[4]=c; out[5]=c>>8; out[6]=c>>16; out[7]=c>>24;
    out[8]=d; out[9]=d>>8; out[10]=d>>16; out[11]=d>>24;
    out[12]='\0';
}

