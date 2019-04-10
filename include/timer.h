#ifndef TB_TIMER_H
#define TB_TIMER_H

#include <time.h>

typedef clock_t tb_time_unit;

// Returns the elapsed time in milliseconds
tb_time_unit tb_current_time();

#endif // TB_TIMER_H
