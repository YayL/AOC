#pragma once

#include "common.h"

#define _PAIR_NUMERIC_TYPE long long

typedef struct pair {
    _PAIR_NUMERIC_TYPE a, b;
} Pair;

Pair * new_pair(_PAIR_NUMERIC_TYPE a, _PAIR_NUMERIC_TYPE b);
