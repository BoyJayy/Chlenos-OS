#pragma once
#include <stdint.h>
void pit_init(uint32_t freq);
uint64_t pit_ticks(void);
void pit_on_tick(void);

