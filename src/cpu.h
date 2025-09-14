#ifndef CPU_H
#define CPU_H
#include <stdint.h>

void     cpu_get_vendor(char out[13]); 
uint32_t cpu_get_version();              
uint32_t cpu_get_features_ecx();      
uint32_t cpu_get_features_edx();         //

int      cpu_has_hypervisor();         
void     cpu_get_hv_vendor(char out[13]); 

#endif

