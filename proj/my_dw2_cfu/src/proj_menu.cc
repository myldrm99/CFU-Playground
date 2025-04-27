#include "proj_menu.h"
#include <stdio.h>
#include "cfu.h"
#include "menu.h"

namespace {
// MAC+ReLU test
void do_test_mac_relu() {
  puts("\n[CFU MAC+ReLU Test]");
  cfu_op0(0x03, 0, 0); // Clear accumulator

  int32_t a = 100;
  int32_t b = -2;

  cfu_op0(0x02, a, b);  // accumulator += a * b = -200
  uint32_t cfu_result = cfu_op0(0x02, 0, 0); // ReLU(-200) = 0
  printf("MAC(%ld,%ld), ReLU result = %lu (expected 0)\n",
         (long)a, (long)b, (unsigned long)cfu_result);

  a = 10; b = 25;
  cfu_op0(0x02, a, b);  // accumulator += 250 = 50 total
  cfu_result = cfu_op0(0x02, 0, 0); // ReLU(50) = 50
  printf("MAC(%ld,%ld), ReLU result = %lu (expected 50)\n",
         (long)a, (long)b, (unsigned long)cfu_result);
}

// Define menu items clearly
struct Menu MENU = {
  "CFU Test Menu",
  "cfu_test",
  {
      MENU_ITEM('1', "CFU MAC+ReLU Test", do_test_mac_relu),
      MENU_END,
  },
};

};  // namespace

extern "C" void do_proj_menu() {
  menu_run(&MENU);
}
