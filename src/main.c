#include "../include/tt.h"

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

        tt_rect_t border_rect;
        tt_rect_init(&border_rect, 0.0, 0.0, 64.0, 64.0,
                     rect_program, 0, 0xaaaaaaff);
        if (tt_debug_had_error()) {
                fprintf(stderr, "%s\n", tt_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        tt_rect_t play_rect;
        tt_rect_init(&play_rect, (tt_window_get_width()-196.0)/2.0, tt_window_get_height()-64.0, 64.0, 64.0,
                            icon_program, play_icon, 0x00ff00ff);
        if (tt_debug_had_error()) {
                fprintf(stderr, "%s\n", tt_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        tt_rect_t stop_rect;
        tt_rect_init(&stop_rect, 64.0 + (tt_window_get_width()-196.0)/2.0, tt_window_get_height()-64.0, 64.0, 64.0,
                     icon_program, stop_icon, 0xff0000ff);
        if (tt_debug_had_error()) {

                fprintf(stderr, "%s\n", tt_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        tt_rect_t replay_rect;
        tt_rect_init(&replay_rect, 128.0 + (tt_window_get_width()-196.0)/2.0, tt_window_get_height()-64.0, 64.0, 64.0,
                     icon_program, replay_icon, 0x000000ff);
        if (tt_debug_had_error()) {
                fprintf(stderr, "%s\n", tt_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        while (tt_window_is_running()) {
                while (tt_window_poll_event()) {
                        tt_window_events();
                        switch (tt_window_event_type()) {
                                case SDL_MOUSEMOTION:
                        }
                }
                tt_window_clear_buffers(GL_COLOR_BUFFER_BIT);
                tt_text_render(&text, &text_glyph);
                tt_text_set_x(&text, (tt_window_get_width()-tt_text_get_width(&text, &text_glyph))/2.0);
                tt_text_set_y(&text, 10.0);
                tt_rect_set_x(&border_rect, tt_rect_get_x(&play_rect));
                tt_rect_set_y(&border_rect, tt_rect_get_y(&play_rect));
                tt_rect_render(&border_rect);
                tt_rect_render(&play_rect);
                tt_rect_set_x(&border_rect, tt_rect_get_x(&stop_rect));
                tt_rect_set_y(&border_rect, tt_rect_get_y(&stop_rect));
                tt_rect_render(&border_rect);
                tt_rect_render(&stop_rect);
                tt_rect_set_x(&border_rect, tt_rect_get_x(&replay_rect));
                tt_rect_set_y(&border_rect, tt_rect_get_y(&replay_rect));
                tt_rect_render(&border_rect);
                tt_rect_render(&replay_rect);
                tt_window_swap();
        }

        tt_text_free(&text);
        tt_glyph_free(&text_glyph);
        glDeleteProgram(text_program);
        tt_window_free();
        return 0;
}
