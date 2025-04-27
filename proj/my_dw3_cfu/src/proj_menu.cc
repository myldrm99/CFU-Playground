#include "proj_menu.h"
#include <stdio.h>
#include "cfu.h"
#include "menu.h"

namespace {

void test_read_ifmap() {
  puts("\n[Read IFMAP Test]");
  cfu_op0(0x10, 0x04030201, 0x08070605);
  for(int i=0; i<8; i++)
    printf("IFMAP[%d] = %d\n", i, (int8_t)cfu_op0(0x14, i, 0));
}

void test_read_filter() {
  puts("\n[Read FILTER Test]");
  cfu_op0(0x11, 0x04030201, 0x08070605);
  for(int i=0; i<8; i++)
    printf("FILTER[%d] = %d\n", i, (int8_t)cfu_op0(0x15, i, 0));
}

void test_mac_relu() {
  puts("\n[MAC and RELU Test]");
  cfu_op0(0x10, 0xFFFFFFFE, 0xFFFFFFFF); // IFMAP [-2,-1,...]
  cfu_op0(0x11, 0x01010101, 0x01010101); // FILTER [1,...]
  cfu_op0(0x16,0,0);
  int32_t mac_res = cfu_op0(0x12,0,0);
  printf("MAC result = %ld\n", (long int)mac_res);  // explicitly %ld
  int32_t relu_res = cfu_op0(0x13,0,0);
  printf("RELU result = %ld\n", (long int)relu_res); // explicitly %ld
}

struct Menu MENU = {
  "CFU Complete Test Menu", "cfu_test", {
    MENU_ITEM('1', "Read IFMAP", test_read_ifmap),
    MENU_ITEM('2', "Read FILTER", test_read_filter),
    MENU_ITEM('3', "MAC and RELU Test", test_mac_relu),
    MENU_END,
  },
};

}; 

extern "C" void do_proj_menu() { menu_run(&MENU); }
