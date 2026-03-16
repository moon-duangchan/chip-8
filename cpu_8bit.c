#include <stdio.h>
#include <stdint.h>

int main()
{
uint8_t memory[4096]={0};
FILE *file =fopen("dummy.ch8","rb");

if (file == NULL){
	printf("Error: file not open\n");
	return 1;
}
  // F read here 
	size_t n = fread(&memory[512],sizeof(unsigned char),4096,file);
	for (int i=0;i<4096;i++)
	{
	  printf("%02X ",memory[i]);
	 if ((i+1)%16==0)printf("\n");
	}


printf("\n");
printf("=======text in file =========\n");
	for (size_t i=0;i<n;i++)
	{
	 printf("%02X ", memory[i]);
	}
printf("\n");
  int pc = 512;
  int runing= 1; 
  while(runing)
  {
    if (pc>=4096)
    {
      break;
    }
  // uint8_t x = (memory[pc]  & 0xF0) >>4;
   //uint8_t y = (memory[pc+1]& 0x0F);
    // printf("X: 0x%04X  position: %d\n",x,memory[pc]);
   //printf("Y; 0x%04X  position: %d\n",x,memory[pc+1]);
  
   uint16_t opcode =(memory[pc] <<8)|memory[pc+1]; 
   printf("opcode :  0x%04X\n",opcode);
   uint8_t category = (opcode & 0xF000)>>12;
   uint8_t x        = (opcode & 0x0F00)>>8;
   uint8_t y        = (opcode & 0x00F0)>>4;
   uint8_t n        = (opcode & 0x000F);
   
   switch(category)
    {
      case 0x1:
        printf("Jump to 0x%03x\n",opcode & 0x0FFF);
        break;
      case 0xA:
        printf("Set I to 0x%03x\n",opcode & 0x0FFF);
        break;
      default:
        printf("Unknow opcode\n");
    }

   pc+=2;
    
    
  }
return 0;
}
