#ifndef TT_H_
#define TT_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

/* debug */
#define TT_DEBUG_ENABLED 1
#define TT_DEBUG_MSG_LENGTH 128
#define TT_DEBUG_STACK_CAPACITY 20

#define TT_PUSH_ERROR(msg) \
        do { tt_debug_stack_push(msg, __LINE__, __FILE__); } while (0)

extern int tt_debug_stack_push(const char *msg, int line, const char *file);
extern char *tt_debug_stack_pop(void);
extern int tt_debug_had_error(void);

/* buffer */
#define TT_BUFFER_INITIAL_CAPACITY 16

typedef struct tt_buffer_t {
        char *data;
        size_t size;
        size_t capacity;
} tt_buffer_t;

typedef enum {
        TT_BUFFER_NO_ERROR = 0,
        TT_BUFFER_NULL_ERROR,
        TT_BUFFER_DATA_ERROR,
        TT_BUFFER_RESIZE_ERROR,
        TT_BUFFER_ENUM_COUNT
} TT_BUFFER_ENUM;

extern tt_buffer_t *tt_buffer_create(const char *data);
extern int tt_buffer_init(tt_buffer_t *buffer, const char *data);
extern int tt_buffer_resize(tt_buffer_t *buffer);
extern int tt_buffer_write(tt_buffer_t *buffer, const char *data);
extern int tt_buffer_insert(tt_buffer_t *buffer, const char *data, int offset);
extern int tt_buffer_clear(tt_buffer_t *buffer);
extern char tt_buffer_get_char(tt_buffer_t *buffer, int index);
extern void tt_buffer_free(tt_buffer_t *buffer);

/* shader */
typedef enum TT_SHADER_TYPE {
        TT_SHADER_FILE_TYPE = 0,
        TT_SHADER_SOURCE_TYPE,
        TT_SHADER_TYPE_COUNT
} TT_SHADER_TYPE;

typedef struct tt_shader_info_t {
        TT_SHADER_TYPE dtype;
        GLenum stype;
        const char *data;
} tt_shader_info_t;

typedef struct tt_program_info_t {
        tt_shader_info_t vinfo;
        tt_shader_info_t finfo;
} tt_program_info_t;

extern GLuint tt_shader_create(tt_shader_info_t sinfo);
extern GLuint tt_program_create(tt_program_info_t pinfo);

/* texture */
typedef enum TT_TEXTURE_TYPE {
        TT_TEXTURE_FILE_TYPE = 0,
        TT_TEXTURE_PIXEL_TYPE,
        TT_TEXTURE_TYPE_COUNT
} TT_TEXTURE_TYPE;

typedef struct tt_texture_info_t {
        TT_TEXTURE_TYPE type;
        const char *path;
        const GLuint *pixels;
        GLfloat w, h;
} tt_texture_info_t;

extern GLuint tt_texture_create(tt_texture_info_t tinfo);

/* window */
#define TT_WINDOW_X (SDL_WINDOWPOS_UNDEFINED)
#define TT_WINDOW_Y (SDL_WINDOWPOS_UNDEFINED)
#define TT_WINDOW_WIDTH (400)
#define TT_WINDOW_HEIGHT (200)
#define TT_WINDOW_FLAGS (SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN)

typedef enum TT_WINDOW_ENUM {
        TT_WINDOW_NO_ERROR = 0,
        TT_WINDOW_SDL_ERROR,
        TT_WINDOW_IMG_ERROR,
        TT_WINDOW_TTF_ERROR,
        TT_WINDOW_WINDOW_ERROR,
        TT_WINDOW_CONTEXT_ERROR,
        TT_WINDOW_GLEW_ERROR,
        TT_WINDOW_ENUM_COUNT
} TT_WINDOW_ENUM;

extern GLuint tt_window_init(const char *name, GLuint x, GLuint y,
                      GLuint w, GLuint h, GLuint flags);
extern GLboolean tt_window_is_running(void);
extern int tt_window_poll_event(void);
extern int tt_window_event_type(void);
extern void tt_window_events(void);
extern void tt_window_clear_buffers(GLbitfield buffers);
extern void tt_window_clear_colour(GLuint colour);
extern void tt_window_swap(void);
extern void tt_window_free(void);

/* utils */
extern size_t tt_utils_fsize(FILE *fd);
extern char *tt_utils_fmap(const char *path);

#endif // TT_H_
