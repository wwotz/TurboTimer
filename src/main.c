#include "../include/tt.h"

int main(int argc, char **argv)
{
        tt_window_init("TurboTimer", TT_WINDOW_X, TT_WINDOW_Y,
                       TT_WINDOW_WIDTH, TT_WINDOW_HEIGHT, TT_WINDOW_FLAGS);
        if (tt_debug_had_error()) {
                fprintf(stderr, "%s\n", tt_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        tt_program_info_t rect_pinfo = {
                { TT_SHADER_FILE_TYPE, GL_VERTEX_SHADER, "../shaders/rect.vert" },
                { TT_SHADER_FILE_TYPE, GL_FRAGMENT_SHADER, "../shaders/rect.frag" }
        };
        GLuint rect_program = tt_program_create(rect_pinfo);
        if (tt_debug_had_error()) {
                fprintf(stderr, "%s\n", tt_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        tt_rect_t rect;
        tt_rect_init(&rect, 0.0, 0.0, 50.0, 50.0,
                     rect_program, 0, 0xff0000ff);
        if (tt_debug_had_error()) {
                fprintf(stderr, "%s\n", tt_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        while (tt_window_is_running()) {
                while (tt_window_poll_event()) {
                        tt_window_events();
                }
                tt_window_clear_buffers(GL_COLOR_BUFFER_BIT);
                tt_rect_render(&rect);
                tt_window_swap();
        }

        tt_rect_free(&rect);
        tt_window_free();
        return 0;
}
