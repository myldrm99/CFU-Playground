# CFU Playground

### Setup CFU Playground

* I followed the CFU-Playground GitHub installation guide.
* Amaranth was not installed.
* I installed Vivado 2020.1.
* Modified proj/proj.mk:
```sh
export TARGET ?= digilent_nexys4ddr
```
* Also check "https://hackmd.io/@pei1005/B1QBvAaTn"

### Make Your Project

```sh
cp -r proj/proj_template_v proj/my_first_cfu
cd proj/my_first_cfu
```

### Measuring how much MAC and DRAM space of MNV2 model use.

* Modify CFU-Playground/common/src/tflite.cc

* Add codes below:
 
```sh
printf("DRAM: %d bytes\n", interpreter->arena_used_bytes());
```

* in here:

```sh
printf("Input: %d bytes, %d dims:", input->bytes, dims->size);
for (int ii = 0; ii < dims->size; ++ii) {
    printf(" %d", dims->data[ii]);
}
puts("\n");
printf("DRAM: %d bytes\n", interpreter->arena_used_bytes());
tflite_postload();
```

### Measuring the cycles of multiply-and-accumulate(MAC) operation required for a model

* We can use the functions in CFU-Playground/common/src/perf.h to count the cycles of MAC operations.

1. Create files in CFU-Playground/common/src/models to record cycles.

* create a file named: "my_cycles.cc"

```sh
long long unsigned my_cycles = 0;

long long unsigned get_my_cycles(){
    return my_cycles;
}

void reset_my_cycles(){
    my_cycles = 0;
}
```

* create a file named: my_cycles.h

```sh
long long unsigned get_my_cycles();
void reset_my_cycles();
```
2. Inside your project folder run the following:

```sh
$ mkdir -p src/tensorflow/lite/kernels/internal/reference/integer_ops/
$ cp \
  ../../third_party/tflite-micro/tensorflow/lite/kernels/internal/reference/conv.h \
  src/tensorflow/lite/kernels/internal/reference/conv.h
```

* This will create a copy of the convolution source code in your project directory. 
* At build time your copy of the source code will replace the regular implementation.

3. Modify conv.h
* Open the newly created copy at proj/my_first_cfu/src/tensorflow/lite/kernels/ internal/reference/conv.h. 
* Locate the innermost loop of the first function, 
* add #include "perf.h" , #include "models/my_cycles.h" and extern long long unsigned my_cycles; at the top of the file and then surround the inner loop with perf functions to count how many cycles this inner loop takes.

```sh
#include "perf.h"
#include "models/my_cycles.h"

extern long long unsigned my_cycles;

/* ... */
unsigned my_start = perf_get_mcycle();
for (int in_channel = 0; in_channel < filter_input_depth; ++in_channel) {
  float input_value = input_data[Offset(
      input_shape, batch, in_y, in_x, in_channel + group * filter_input_depth)];
  float filter_value = filter_data[Offset(
      filter_shape, out_channel, filter_y, filter_x, in_channel)];
acc += (input_value * filter_value);
}
unsigned my_finish = perf_get_mcycle();
my_cycles += (my_finish - my_start);
```
4. Modify CFU-Playground/common/src/models/mnv2/mnv2.cc .

* Add #include "models/my_cycles.h" at the top of the file. 
* Use the functions in models/my_cycles.h to print the total cycles of MAC operations.

```sh
#include "models/mnv2/mnv2.h"

#include <stdio.h>

#include "menu.h"
#include "models/mnv2/input_00001_18027.h"
#include "models/mnv2/input_00001_7281.h"
#include "models/mnv2/input_00001_7425.h"
#include "models/mnv2/input_00002_2532.h"
#include "models/mnv2/input_00002_25869.h"
#include "models/mnv2/input_00004_970.h"
#include "models/mnv2/model_mobilenetv2_160_035.h"
#include "tflite.h"
#include "models/my_cycles.h"

extern "C" {
#include "fb_util.h"
};
```

5. Run the project

* You must make clean first. 
* To enable performance counters you should use the command below.

```sh
make clean
make prog EXTRA_LITEX_ARGS="--cpu-variant=perf+cfu"
make load
```
* or, if you use Vivado and ARTY100T 

```sh
make clean
make prog TARGET=digilent_nexys4ddr USE_VIVADO=1 EXTRA_LITEX_ARGS="--cpu-variant=perf+cfu"
make make load TARGET=digilent_nexys4ddr
```
----------------------------------------------------------------------------------------------
# How TO Run Models
----------------------------------------------------------------------------------------------
### CFU with Renode
----------------------------------------------------------------------------------------------
```sh
cd CFU-Playground
cp -r proj/proj_template proj/"proj_name"
cd proj/"proj_name"
code Makefile
#choose model which you want to use
make renode
```
----------------------------------------------------------------------------------------------
### CFU with FPGA 
----------------------------------------------------------------------------------------------
```sh
cd CFU-Playground
cd proj/"proj_name"
make clean
make prog  USE_VIVADO=1
make load 
# nothing happens, type ENTER
litex> reboot <- type this or push reset button on board
# for exir panel: ctrl + alt +c //twice 
```
-----------------------------------------------------------------------------------