#include <stdio.h>

#include "twineconsensus.h"

int main(int argc, char **argv) {
  if (test(7) == 9) {
    printf("Shared library test success\n");
  } else {
    printf("Shared library test failure\n");
  }
  return 0;
}
