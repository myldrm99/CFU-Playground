#include <stdint.h>
#include "software_cfu.h"


int8_t ifmap[8];
int8_t filter[8];

extern "C" uint32_t software_cfu(int funct3, int funct7, uint32_t rs1, uint32_t rs2) {
  uint32_t fid = (funct7 << 3) | funct3;

  switch (fid) {
    case 0x08: // Load IFMAP
      for (int i = 0; i < 4; i++) ifmap[i] = ((int8_t)(rs1 >> (8 * i))) - 128;
      for (int i = 0; i < 4; i++) ifmap[i + 4] = ((int8_t)(rs2 >> (8 * i))) - 128;
      return 0;

    case 0x10: // Load FILTER
      for (int i = 0; i < 4; i++) filter[i] = (int8_t)(rs1 >> (8 * i));
      for (int i = 0; i < 4; i++) filter[i + 4] = (int8_t)(rs2 >> (8 * i));
      return 0;

    case 0x18: { // Read FILTER {filter[0], filter[2], filter[4], filter[6]}
      uint32_t out = 0;
      out |= ((uint8_t)filter[0]) << 0;
      out |= ((uint8_t)filter[2]) << 8;
      out |= ((uint8_t)filter[4]) << 16;
      out |= ((uint8_t)filter[6]) << 24;
      return out;
    }

    default:
      return 0;
  }
}
