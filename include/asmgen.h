#pragma once

#include <stdint.h>

struct asm_unit
{
   uint8_t* data;
   size_t len;
};

void gen_asm(const char* content, struct asm_unit* output);