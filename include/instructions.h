#pragma once
#include <stdint.h>

struct instruction
{
   const char* name;
   uint8_t opcode;
} instructions[] = {
    {"add", 0x00},  //
};

struct reg
{
   const char* names[4];
} registers[] = {
    {"ax"}  //
};