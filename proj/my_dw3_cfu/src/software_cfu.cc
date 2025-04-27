#include <stdint.h>

int8_t ifmap[8], filter[8];
int32_t accumulator = 0;

extern "C" uint32_t software_cfu(int funct3, int funct7, uint32_t rs1, uint32_t rs2) {
  switch (funct7) {
    case 0x10: 
      for(int i=0; i<4; i++) ifmap[i] = (rs1 >> (8*i));
      for(int i=0; i<4; i++) ifmap[i+4] = (rs2 >> (8*i));
      return 0;
    case 0x11: 
      for(int i=0; i<4; i++) filter[i] = (rs1 >> (8*i));
      for(int i=0; i<4; i++) filter[i+4] = (rs2 >> (8*i));
      return 0;
    case 0x12:
      for(int i=0; i<8; i++) accumulator += ifmap[i]*filter[i];
      return accumulator;
    case 0x13:
      accumulator = (accumulator > 0)? accumulator : 0;
      return accumulator;
    case 0x16:
      accumulator = 0;
      return 0;
    case 0x14:
      return (int32_t)ifmap[rs1 & 0x7];
    case 0x15:
      return (int32_t)filter[rs1 & 0x7];
    default: return 0;
  }
}
