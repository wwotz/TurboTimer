#include "../include/tt.h"

static SDL_Window *tt_window = NULL;
static SDL_GLContext *tt_context = NULL;
static SDL_Event tt_event = {0};
static GLboolean tt_running = GL_FALSE;

TT_WINDOW_ENUM tt_window_init(const char *name, GLuint x, GLuint y,
                      GLuint w, GLuint h, GLuint flags)
{
        int img_flags, status;
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
                TT_PUSH_ERROR("Failed to initialise SDL!");
                return TT_WINDOW_SDL_ERROR;
        }

        if (TTF_Init() != 0) {
                TT_PUSH_ERROR("Failed to initialise TTF!");
                return TT_WINDOW_TTF_ERROR;
        }

        img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
        if (IMG_Init(img_flags) != img_flags) {
                TT_PUSH_ERROR("Failed to initialise IMG!");
                return TT_WINDOW_IMG_ERROR;
        }

        tt_window = SDL_CreateWindow(name, x, y, w, h, flags);
        if (!tt_window) {
                TT_PUSH_ERROR("Failed to initialise SDL window!");
                return TT_WINDOW_WINDOW_ERROR;
        }

        tt_context = SDL_GL_CreateContext(tt_window);
        if (!tt_context) {
                TT_PUSH_ERROR("Failed to initialise OpenGL context!");
                return TT_WINDOW_CONTEXT_ERROR;
        }

        status = glewInit();
        if (status != GLEW_OK) {
                TT_PUSH_ERROR("Failed to initialise glew!");
                return TT_WINDOW_GLEW_ERROR;
        }

        SDL_SetWindowAlwaysOnTop(tt_window, SDL_TRUE);
        tt_running = GL_TRUE;
        return TT_WINDOW_NO_ERROR;
}

GLboolean tt_window_is_running(void)
{
        return tt_running;
}

int tt_window_poll_event(void)
{
        return SDL_PollEvent(&tt_event);
}

int tt_window_event_type(void)
{
        return tt_event.type;
}

void tt_window_events(void)
{
        switch (tt_window_event_type()) {
        case SDL_QUIT:
                tt_running = GL_FALSE;
                break;
        }
}

void tt_window_clear_buffers(GLbitfield buffers)
{
        glClear(buffers);
}

void tt_window_clear_colour(GLuint colour)
{
        //TODO: implement colour functions
}

void tt_window_swap(void)
{
        SDL_GL_SwapWindow(tt_window);
}

GLuint tt_window_free(void)
{
        SDL_DestroyWindow(tt_window);
        SDL_GL_DeleteContext(tt_context);
        tt_window = NULL;
        tt_context = NULL;
        SDL_Quit();
        IMG_Quit();
        TTF_Quit();
}
