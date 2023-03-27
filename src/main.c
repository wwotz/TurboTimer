#include "../include/tt.h"

static char *time_to_str(float time, char *buf, size_t buf_len)
{
        int itime = (int) time;
        buf[2] = buf[5] = ':';
        for (int i = 2; i >= 0; i--) {
                int val = itime % 60;
                buf[i*3]     = (val / 10) + '0';
                buf[i*3 + 1] = (val % 10) + '0';
                itime /= 60;
        }
        buf[8] = '\0';
        return buf;
}

static float time = 0.0, prev_time = 0.0;
static int get_time = 0;

static void play_func(void)
{
        if (get_time)
                return;
        tt_timer_start();
        get_time = 1;
        prev_time += time;
        time = 0.0;
}

static void stop_func(void)
{
        if (get_time) {
                tt_timer_reset();
                get_time = 0;
                prev_time += time;
                time = 0.0;
        }
}

static void replay_func(void)
{
        tt_timer_reset();
        time = 0.0;
        prev_time = 0.0;
}

static float get_timef(void)
{
        if (get_time)
                time = tt_timer_get_time();
        return time + prev_time;
}

int main(int argc, char **argv)
{
        tt_window_init("TurboTimer", TT_WINDOW_X, TT_WINDOW_Y,
                       TT_WINDOW_WIDTH, TT_WINDOW_HEIGHT, TT_WINDOW_FLAGS);
        if (tt_debug_had_error()) {
                fprintf(stderr, "%s\n", tt_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        const char *glyph_sample = "0 1 2 3 4 5 6 7 8 9 :";
        tt_program_info_t text_pinfo = {
                { TT_SHADER_FILE_TYPE, GL_VERTEX_SHADER, "../shaders/text.vert" },
                { TT_SHADER_FILE_TYPE, GL_FRAGMENT_SHADER, "../shaders/text.frag" }
        };
        GLuint text_program = tt_program_create(text_pinfo);
        if (tt_debug_had_error()) {
                fprintf(stderr, "%s\n", tt_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        tt_program_info_t rect_pinfo = {
                { TT_SHADER_FILE_TYPE, GL_VERTEX_SHADER, "../shaders/rect.vert" },
                { TT_SHADER_FILE_TYPE, GL_FRAGMENT_SHADER, "../shaders/rect.frag" }
        };
        GLuint rect_program = tt_program_create(rect_pinfo);

        tt_program_info_t icon_pinfo = {
                { TT_SHADER_FILE_TYPE, GL_VERTEX_SHADER, "../shaders/icon.vert" },
                { TT_SHADER_FILE_TYPE, GL_FRAGMENT_SHADER, "../shaders/icon.frag" }
        };
        GLuint icon_program = tt_program_create(icon_pinfo);
        if (tt_debug_had_error()) {
                fprintf(stderr, "%s\n", tt_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        tt_texture_info_t play_tinfo = {
                .type = TT_TEXTURE_FILE,
                .path = "../images/play.png",
                NULL, 0, 0
        };
        GLuint play_icon = tt_texture_create(play_tinfo);
        if (tt_debug_had_error()) {
                fprintf(stderr, "%s\n", tt_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        tt_texture_info_t stop_tinfo = {
                .type = TT_TEXTURE_FILE,
                .path = "../images/stop.png",
                NULL, 0, 0
        };
        GLuint stop_icon = tt_texture_create(stop_tinfo);
        if (tt_debug_had_error()) {
                fprintf(stderr, "%s\n", tt_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        tt_texture_info_t replay_tinfo = {
                .type = TT_TEXTURE_FILE,
                .path = "../images/replay.png",
                NULL, 0, 0
        };
        GLuint replay_icon = tt_texture_create(replay_tinfo);
        if (tt_debug_had_error()) {
                fprintf(stderr, "%s\n", tt_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        tt_glyph_t text_glyph;
        tt_glyph_init(&text_glyph, "../fonts/Roboto-Regular.ttf",
                      glyph_sample, 64);
        if (tt_debug_had_error()) {
                fprintf(stderr, "%s\n", tt_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        tt_text_t text;
        tt_text_init(&text, "00:00:00", &text_glyph,
                     text_program, 0.0, 0.0, 0xffffffff);
        if (tt_debug_had_error()) {
                fprintf(stderr, "%s\n", tt_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        tt_button_t play_button, stop_button, replay_button;
        tt_button_init(&play_button, (tt_window_get_width()-196.0)/2.0, tt_window_get_height()-64.0, 64.0, 64.0, 64.0, 64.0,
                       rect_program, icon_program, play_icon, 0xffffffff, 0x00ff00ff);
        if (tt_debug_had_error()) {
                fprintf(stderr, "%s\n", tt_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        tt_button_init(&stop_button, (tt_window_get_width()-196.0)/2.0 + 64.0, tt_window_get_height()-64.0, 64.0, 64.0, 64.0, 64.0,
                       rect_program, icon_program, stop_icon, 0xffffffff, 0xff0000ff);
        if (tt_debug_had_error()) {
                fprintf(stderr, "%s\n", tt_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        tt_button_init(&replay_button, (tt_window_get_width()-196.0)/2.0 + 128.0, tt_window_get_height()-64.0, 64.0, 64.0, 64.0, 64.0,
                       rect_program, icon_program, replay_icon, 0xffffffff, 0x000000ff);
        if (tt_debug_had_error()) {
                fprintf(stderr, "%s\n", tt_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        const size_t timestr_len = 9;
        char timestr[9] = {0};

        tt_timer_start();
        while (tt_window_is_running()) {
                while (tt_window_poll_event()) {
                        tt_window_events();
                        tt_button_events(&play_button, play_func);
                        tt_button_events(&stop_button, stop_func);
                        tt_button_events(&replay_button, replay_func);
                }
                tt_window_clear_buffers(GL_COLOR_BUFFER_BIT);
                time_to_str(get_timef(), timestr, timestr_len);
                tt_text_write(&text, timestr);
                tt_text_render(&text, &text_glyph);
                tt_text_set_x(&text, (tt_window_get_width()-tt_text_get_width(&text, &text_glyph))/2.0);
                tt_text_set_y(&text, 10.0);
                tt_button_render(&play_button);
                tt_button_render(&stop_button);
                tt_button_render(&replay_button);
                tt_window_swap();
        }

        tt_text_free(&text);
        tt_glyph_free(&text_glyph);
        glDeleteProgram(text_program);
        tt_window_free();
        return 0;
}
