#include <stdio.h>
#include "asmgen.h"

int main()
{
   struct asm_unit output = {};
   gen_asm("add ax @ax+128", &output);
   for (size_t i = 0; i < output.len; i++)
   {
      printf("0X%02X\n", output.data[i]);
   }
   return 0;
}
