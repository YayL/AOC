#include "pair.h"
#include <stdlib.h>

Pair * new_pair(_PAIR_NUMERIC_TYPE a, _PAIR_NUMERIC_TYPE b) {
    Pair * pair = malloc(sizeof(*pair));
    pair->a = a;
    pair->b = b;
    return pair;
}
