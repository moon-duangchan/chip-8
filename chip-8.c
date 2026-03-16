#include <stdio.h>
#include <stdint.h>

uint16_t stack[16]={0};
uint8_t sp=0;
uint8_t V[16]={0}; // rigster
uint8_t MEMORY[4096] ={0}; // rom
uint16_t I =0;
int load_rom()
{
  FILE *file =fopen("test_math.ch8","rb");

      if (file == NULL)
      {
        printf("Error: file not open\n");
        return 1;
      }
  // F read here
        size_t n = fread(&MEMORY[512],sizeof(unsigned char),4096,file);
        for (int i=0;i<4096;i++)
        {
          printf("%02X ",MEMORY[i]);
         if ((i+1)%16==0)printf("\n");
        }

}


int opcode()
{
  load_rom();

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


    // Decode 
   uint16_t opcode =(MEMORY[pc] <<8)|MEMORY[pc+1];
   //printf("opcode :  0x%04X\n",opcode);
   uint8_t category = (opcode & 0xF000)>>12;
   uint8_t x        = (opcode & 0x0F00)>>8;
   uint8_t y        = (opcode & 0x00F0)>>4;
   uint8_t n        = (opcode & 0x000F);
   uint8_t nn       = (opcode & 0x00FF);
   uint16_t nnn     = (opcode & 0x0FFF);


    printf("Category: 0x%X, Opcode: 0x%04X\n", category, opcode);
    switch(category)
     {
       case 0x0:
         printf("Case 0x0: SYS (NNN=%03X)\n", nnn);
         break;
       case 0x1:
         pc = nnn;
         printf("Case 0x1: JP to 0x%03X\n", nnn);
         continue;
       case 0x3:
         printf("Case 0x3: SE V[%X], 0x%02X\n", x, nn);
         if(V[x]== nn)
         {
           pc+=2;
           printf("  -> Skip (V[%X]=%02X == %02X)\n", x, V[x], nn);
         }
         break;
       case 0x2:
         printf("Case 0x2: CALL 0x%03X\n", nnn);
         stack[sp] = pc;
         sp++;
         pc = nnn;
         continue;
       case 0x4:
         printf("Case 0x4: SNE V[%X], 0x%02X\n", x, nn);
         if(V[x]!= nn)
         {
           pc+=2;
         }
         break;
       case 0x5:
         printf("Case 0x5: SE V[%X], V[%X]\n", x, y);
         if(V[x]== V[y])
         {
           pc+2;
         }
         break;
       case 0x6:
         printf("Case 0x6: LD V[%X], 0x%02X\n", x, nn);
         V[x] = nn;
         break;
       case 0x7:
         printf("Case 0x7: ADD V[%X], 0x%02X\n", x, nn);
         V[x] +=nn;
         break;
       case 0x8:
         printf("Case 0x8: ALU op 0x%X (x=%X, y=%X)\n", n, x, y);
         switch(n)
         {
           case 0x0:
             printf("  0x0: LD V[%X], V[%X]\n", x, y);
             V[x]=V[y];
           case 0x1:
             printf("  0x1: OR V[%X], V[%X]\n", x, y);
             V[x]=V[x]|V[y];
           case 0x2:
             printf("  0x2: AND V[%X], V[%X]\n", x, y);
             V[x]=V[x]&V[y];
           case 0x3:
             printf("  0x3: XOR V[%X], V[%X]\n", x, y);
             V[x]=V[x]^V[y];
           case 0x4:
             printf("  0x4: ADD V[%X], V[%X]\n", x, y);
             uint16_t sum =V[x]+V[y];
             V[x] = sum & 0xFF;
             if(sum > 255)
             {
               V[0xF]=1;
             }else 
             {
               V[0xF]=0;
             }
              break;
           case 0x5:
             printf("  0x5: SUB V[%X], V[%X]\n", x, y);
             uint8_t flag=(V[x]>=V[y] ? 1:0);
             V[x] =V[x] -V[y];
             V[0xF]=flag;
             break;
           case 0x6:
             printf("  0x6: SHR V[%X]\n", x);
             uint8_t lsb = V[x] & 0x01;
             V[x]>>=1;
             V[0xF] = lsb;
             break;
           case 0x7:
             printf("  0x7: SUBN V[%X], V[%X]\n", x, y);
                         if(V[y]>=V[x])
             {
               V[0xF]=1;
             }else
             {
               V[0xF]=0;
             }
               V[x]= V[y]-V[x];

           case 0xE:
             printf("  0xE: SHL V[%X]\n", x);
             uint8_t msb=(V[x] & 0x80)>>7;
             V[x]<<=1;
             V[0xF]=msb;
             break;
           default:
             printf("Unknown 0x8 sub-op: 0x%X\n", n);
             break; 
          }
         break;
       case 0xA:
         I = nnn;
         printf("Case 0xA: LD I, 0x%03X :I is now 0x%03X\n ",nnn,I);
         break;
       case 0xB:
         printf("Case 0xB: JP V0, 0x%03X\n", nnn);
         break;
       case 0xC:
         printf("Case 0xC: RND V[%X], 0x%02X\n", x, nn);
         break;
       case 0xD:
         printf("Case 0xD: DRW V[%X], V[%X], 0x%X\n", x, y, n);
         break;
       case 0xE:
         printf("Case 0xE: SKP/SKNP V[%X]\n", x);
         break;
       case 0xF:
         printf("Case 0xF: misc (NN=%02X)\n", nn);
         break;
       default:
         printf("Unknown category: 0x%X\n", category);
    }

   pc+=2;


  }

}



int main()
{
  opcode();

}
