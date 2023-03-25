#include "../include/tt.h"

int main(int argc, char **argv)
{
        tt_window_init("TurboTimer", TT_WINDOW_X, TT_WINDOW_Y,
                       TT_WINDOW_WIDTH, TT_WINDOW_HEIGHT, TT_WINDOW_FLAGS);
        if (tt_debug_had_error()) {
                fprintf(stderr, "%s\n", tt_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        while (tt_window_is_running()) {
                while (tt_window_poll_event()) {
                        tt_window_events();
                }
                tt_window_clear_buffers(GL_COLOR_BUFFER_BIT);
                tt_window_swap();
        }

        tt_window_free();
        return 0;
}
