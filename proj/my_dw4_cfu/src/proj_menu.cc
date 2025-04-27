#include "proj_menu.h"
#include <stdio.h>
#include "cfu.h"
#include "menu.h"

namespace {

void do_test_filter_read() {
  puts("\n[Filter Read Test]");

  // Load IFMAP with arbitrary values (should not affect filter read)
  cfu_op0(0x08, 0x01020304, 0x05060708);

  // Load FILTER with known values
  cfu_op0(0x10, 0xA1A2A3A4, 0xB1B2B3B4);

  // Read FILTER values (expected: B4 B2 A4 A2 = 0xB4B2A4A2)
  uint32_t result_cfu = cfu_op0(0x18, 0, 0);
  printf("Read FILTER result: 0x%08X\n", (unsigned int)result_cfu);
}

struct Menu MENU = {
  "CFU Test Menu",
  "cfu_test",
  {
    MENU_ITEM('1', "Test Filter Read", do_test_filter_read),
    MENU_END,
  },
};

};  // anonymous namespace

extern "C" void do_proj_menu() {
  menu_run(&MENU);
}
