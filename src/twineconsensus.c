#include "twineconsensus.h"
#include "config.h"

uint32_t twlib_test_version(uint32_t version) {
  return version <= (uint32_t) TWCON_IF_VER;
}
