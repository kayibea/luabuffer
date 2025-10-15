#pragma once

#include <stdlib.h>

#define FREE(ptr) \
  do {            \
    free(ptr);    \
    (ptr) = NULL; \
  } while (0)
