#include "timer.h"

tb_time_unit tb_current_time() {
    return clock() / CLOCKS_PER_SEC * 1000;
}
