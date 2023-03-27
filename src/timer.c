#include "../include/tt.h"

static struct timeval tt_start_time, tt_curr_time;

void tt_timer_start()
{
        tt_timer_reset();
}

void tt_timer_reset()
{
        gettimeofday(&tt_start_time, NULL);
        gettimeofday(&tt_curr_time, NULL);
}

float tt_timer_get_time()
{
        float time;
        gettimeofday(&tt_curr_time, NULL);
        time = (tt_curr_time.tv_sec - tt_start_time.tv_sec)
                + (tt_curr_time.tv_usec - tt_start_time.tv_usec)/1000000.0;
        return time;
}
