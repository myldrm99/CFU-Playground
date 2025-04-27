#include "proj_menu.h"
#include <stdio.h>
#include "cfu.h"
#include "menu.h"

namespace {

// Test CFU Load and Read clearly
void do_test_cfu_load_read() {
  puts("\n[CFU Load/Read Test]");
  uint32_t test_value = 0xDEADBEEF;

  // Load the value to CFU
  cfu_op0(0x00, test_value, 0);

  // Read back the value (renamed variable to avoid shadowing)
  uint32_t cfu_result = cfu_op0(0x01, 0, 0);

  // Verify correctness (correct format specifiers)
  if (cfu_result == test_value)
      printf("PASS: CFU returned correct value 0x%08lX\n", (unsigned long)cfu_result);
  else
      printf("FAIL: Expected 0x%08lX, Got 0x%08lX\n",
             (unsigned long)test_value,
             (unsigned long)cfu_result);
}

// Define interactive menu
struct Menu MENU = {
  "CFU Test Menu",
  "cfu_test",
  {
      MENU_ITEM('1', "CFU Load/Read Test", do_test_cfu_load_read),
      MENU_END,
  },
};

};  // anonymous namespace

extern "C" void do_proj_menu() {
  menu_run(&MENU);
}
