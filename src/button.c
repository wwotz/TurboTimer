#include "../include/tt.h"

tt_button_t *tt_button_create(GLfloat x, GLfloat y,
                              GLfloat btn_w, GLfloat btn_h,
                              GLfloat icon_w, GLfloat icon_h,
                              GLuint btn_program, GLuint icon_program,
                              GLuint icon_texture, GLuint btn_colour, GLuint icon_colour)
{
        tt_button_t *button;
        int status;

        button = malloc(sizeof(*button));
        if (!button) {
                TT_PUSH_ERROR("failed to allocate memory for button!");
                return NULL;
        }

        if ((status = tt_button_init(button, x, y, btn_w, btn_h, icon_w, icon_h,
                       btn_program, icon_program, icon_texture,
                       btn_colour, btn_colour)) != TT_BUTTON_NO_ERROR) {
                free(button);
                button = NULL;
        }
        return button;
}

int tt_button_init(tt_button_t *button, GLfloat x, GLfloat y, GLfloat btn_w, GLfloat btn_h,
                   GLfloat icon_w, GLfloat icon_h,
                   GLuint btn_program, GLuint icon_program,
                   GLuint icon_texture, GLuint btn_colour, GLuint icon_colour)
{
        int status;
        if (!button) {
                TT_PUSH_ERROR("buffer was null!");
                return TT_BUTTON_NULL_ERROR;
        }

        if ((status = tt_rect_init(&button->btn, x, y, btn_w, btn_h,
                                   btn_program, 0, btn_colour)) != TT_RECT_NO_ERROR)
                return status;

        if ((status = tt_rect_init(&button->icon, x+(btn_w-icon_w)/2.0, y+(btn_h-icon_h)/2.0,
                                   icon_w, icon_h, icon_program, icon_texture, icon_colour)) != TT_RECT_NO_ERROR)
                return status;
        return TT_BUTTON_NO_ERROR;
}

void tt_button_set_x(tt_button_t *button, GLfloat x)
{
        if (button)
                tt_rect_set_x(&button->btn, x);
}

void tt_button_set_y(tt_button_t *button, GLfloat y)
{
        if (button)
                tt_rect_set_y(&button->btn, y);
}

void tt_button_set_width(tt_button_t *button, GLfloat w)
{
        if (button)
                tt_rect_set_width(&button->btn, w);
}

void tt_button_set_height(tt_button_t *button, GLfloat h)
{
        if (button)
                tt_rect_set_height(&button->btn, h);
}

GLfloat tt_button_get_x(tt_button_t *button)
{
        if (button)
                return tt_rect_get_x(&button->btn);
        return 0.0;
}

GLfloat tt_button_get_y(tt_button_t *button)
{
        if (button)
                return tt_rect_get_y(&button->btn);
        return 0.0;
}

GLfloat tt_button_get_width(tt_button_t *button)
{
        if (button)
                return tt_rect_get_width(&button->btn);
        return 0.0;
}

GLfloat tt_button_get_height(tt_button_t *button)
{
        if (button)
                return tt_rect_get_height(&button->btn);
        return 0.0;
}

void tt_button_events(tt_button_t *button, void (*func)())
{
        float mx, my;
        switch (tt_window_event_type()) {
        case SDL_MOUSEMOTION:
                mx = tt_window_mouse_x();
                my = tt_window_mouse_y();
                if (mx >= tt_button_get_x(button)
                    && mx <= tt_button_get_x(button) + tt_button_get_width(button)
                    && my >= tt_button_get_y(button)
                    && my <= tt_button_get_y(button) + tt_button_get_height(button)) {
                        tt_rect_set_colour(&button->btn, 0x333333ff);
                } else {
                        tt_rect_set_colour(&button->btn, 0xffffffff);
                }
                break;
        case SDL_MOUSEBUTTONDOWN:
                switch (tt_window_event_button()) {
                case SDL_BUTTON_LEFT:
                        mx = tt_window_mouse_x();
                        my = tt_window_mouse_y();
                        if (mx >= tt_button_get_x(button)
                            && mx <= tt_button_get_x(button) + tt_button_get_width(button)
                            && my >= tt_button_get_y(button)
                            && my <= tt_button_get_y(button) + tt_button_get_height(button)) {
                                func();
                        } 
                        break;
                }
                break;
        }
}

void tt_button_render(tt_button_t *button)
{
        if (button) {
                tt_rect_render(&button->btn);
                tt_rect_render(&button->icon);
        }
}

void tt_button_free(tt_button_t *button)
{
        if (button) {
                tt_rect_free(&button->btn);
                tt_rect_free(&button->icon);
        }
}
