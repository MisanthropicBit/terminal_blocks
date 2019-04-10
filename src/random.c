#include "random.h"
#include <stdlib.h>

int tb_random_number(int min, int max) {
    return rand() % (max - min + 1) + min;
}
