#include "asmgen.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instructions.h"

#define ARG_MAX 12
#define MAX_ARG 3

#define MASK(n) ((1 << (n)) - 1)
#define SET(n, v) ((v) & MASK(n))
#define UNSET(n, v) ((v) & (~MASK(n)))

struct arg
{
   uint16_t value;
   uint16_t offset;
   enum
   {
      ARG_REG,
      ARG_IMM,
      ARG_MEM
   } type;
   enum
   {
      BITS_8,
      BITS_16
   } min_bits;
};

struct opcall
{
   uint8_t opcode;
   uint8_t mod;
   struct arg args[2];
   uint8_t len;
};

uint8_t find_opcode(const char* mnemonic);
struct arg decide_arg(const char* input);

uint8_t find_register(const char* mnemonic);

void parse_line(const char* content, struct asm_unit* output);

void gen_asm(const char* content, struct asm_unit* output)
{
   parse_line(content, output);
}

void parse_line(const char* content, struct asm_unit* output)
{
   char toks[MAX_ARG][ARG_MAX];
   struct opcall op = {};
   op.len =
       (uint8_t)sscanf_s(content, "%s %s %s", toks[0], ARG_MAX, toks[1], ARG_MAX, toks[2], ARG_MAX);

   if (op.len > 1)
      op.args[0] = decide_arg(toks[1]);
   if (op.len > 2)
      op.args[1] = decide_arg(toks[2]);

   op.opcode = find_opcode(toks[0]);
   if (op.len == 2)
   {
      switch (op.args[0].type)
      {
      case ARG_REG:
         op.opcode += op.args[0].value;  // 1 reg arg
         break;
      case ARG_IMM:
         break;  // 1 imm arg. like "push 236"
      case ARG_MEM:
         break;  // arg like "pop @sp+4".
      default:
         break;
      }
   }
   if (op.len == 3)
   {
      if (op.args[0].type == ARG_REG)
      {
      }
      else
      {
         // should be a memory address otherwise
      }
   }

   output->data = realloc(output->data, ++output->len);
   output->data[output->len - 1] = op.opcode;
}

uint8_t find_opcode(const char* mnemonic)
{
   for (size_t i = 0; i < sizeof(instructions) / sizeof(struct instruction); i++)
   {
      if (!strcmp(mnemonic, instructions[i].name))
      {
         return instructions[i].opcode;
      }
   }
   return UINT8_MAX;
}

uint8_t find_register(const char* mnemonic)
{
   for (size_t i = 0; i < sizeof(registers) / sizeof(struct reg); i++)
   {
      if (!strcmp(mnemonic, registers[i].names[0]))  // idc about others rn, just word for now
      {
         return (uint8_t)i;
      }
   }
   return UINT8_MAX;
}

struct arg decide_arg(const char* input)
{
   struct arg out = {};
   if (input[0] == '@')
   {
      char rv[4] = {};
      sscanf_s(input, "@%[^+]+%hu", rv, 4, &out.offset);
      printf_s("address at %s + %hu\n", rv, out.offset);
      out.value = find_register(rv);
      out.type = ARG_MEM;
      out.min_bits = out.value <= UINT8_MAX && out.offset <= UINT8_MAX ? BITS_8 : BITS_16;
   }
   else if (isdigit(input[0]))
   {
      out.value = (uint16_t)atoi(input);
      printf_s("immediate of %hu\n", out.value);
      out.type = ARG_IMM;
      out.min_bits = out.value <= UINT8_MAX ? BITS_8 : BITS_16;
   }
   else  // no label check yet!
   {
      out.value = find_register(input);
      printf_s("register of code %hu\n", out.value);
      out.type = ARG_REG;
      out.min_bits = BITS_16;  // no 8 bit yet!
   }

   return out;
}