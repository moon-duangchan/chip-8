#include <stdio.h>
#include <stdint.h>

int main()
{
  uint16_t opcode = 0x8422;
  uint8_t category = (opcode & 0xF000) >> 12;
  uint8_t X = (opcode & 0x0F00) >>8;
  uint8_t Y = (opcode & 0x00F0) >>4;
  uint8_t action = (opcode & 0x000F);

  printf("Opcode: 0x%04X\n", opcode);
  printf("Category: 0x%X\n", category);
  printf("Register X: 0x%X\n", X);
  printf("Register Y: 0x%X\n", Y);
  printf("Action: 0x%X\n", action);
  
  return 0;
}

