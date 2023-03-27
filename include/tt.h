#ifndef TT_H_
#define TT_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include <sys/time.h>

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
extern int tt_buffer_append(tt_buffer_t *buffer, char c);
extern size_t tt_buffer_length(tt_buffer_t *buffer);
extern int tt_buffer_write(tt_buffer_t *buffer, const char *data);
extern int tt_buffer_insert(tt_buffer_t *buffer, const char *data, int offset);
extern int tt_buffer_clear(tt_buffer_t *buffer);
extern char tt_buffer_get_charat(tt_buffer_t *buffer, int index);
extern void tt_buffer_set_charat(tt_buffer_t *buffer, int index, char c);
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
        TT_TEXTURE_FILE = 0,
        TT_TEXTURE_PIXELS,
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

extern float tt_model[16], tt_projection[16], tt_view[16];

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
extern float tt_window_mouse_x(void);
extern float tt_window_mouse_y(void);
extern int tt_window_event_button(void);
extern void tt_window_events(void);
extern void tt_window_clear_buffers(GLbitfield buffers);
extern void tt_window_clear_colour(GLuint colour);
extern int tt_window_get_width(void);
extern int tt_window_get_height(void);
extern void tt_window_get_size(int *w, int *h);
extern void tt_window_swap(void);
extern void tt_window_free(void);

/* utils */
extern size_t tt_utils_fsize(FILE *fd);
extern char *tt_utils_fmap(const char *path);

/* colour */
typedef enum TT_COLOUR_MASK {
        TT_COLOUR_R = 0,
        TT_COLOUR_G,
        TT_COLOUR_B,
        TT_COLOUR_A,
        TT_COLOUR_MASK_COUNT
} TT_COLOUR_MASK;

extern GLfloat tt_colour_mask(GLuint colour, TT_COLOUR_MASK mask, GLboolean normalized);

/* linear */
typedef struct {
        float x, y;
} tt_vec2_t;

typedef struct {
        float x, y, z;
} tt_vec3_t;

typedef struct {
        float x, y, z, w;
} tt_vec4_t;

extern tt_vec2_t tt_vec2_create(float x, float y);
extern float  tt_vec2_length(tt_vec2_t vec);
extern tt_vec2_t tt_vec2_add(tt_vec2_t first, tt_vec2_t second);
extern tt_vec2_t tt_vec2_sub(tt_vec2_t first, tt_vec2_t second);
extern tt_vec2_t tt_vec2_mul(tt_vec2_t first, tt_vec2_t second);
extern tt_vec2_t tt_vec2_div(tt_vec2_t first, tt_vec2_t second);
extern float  tt_vec2_dot(tt_vec2_t first, tt_vec2_t second);
extern float  tt_vec2_cross(tt_vec2_t first, tt_vec2_t second);
extern tt_vec2_t tt_vec2_normalise(tt_vec2_t vec);

extern tt_vec3_t tt_vec3_create(float x, float y, float z);
extern float  tt_vec3_length(tt_vec3_t vec);
extern tt_vec3_t tt_vec3_add(tt_vec3_t first, tt_vec3_t second);
extern tt_vec3_t tt_vec3_sub(tt_vec3_t first, tt_vec3_t second);
extern tt_vec3_t tt_vec3_mul(tt_vec3_t first, tt_vec3_t second);
extern tt_vec3_t tt_vec3_div(tt_vec3_t first, tt_vec3_t second);
extern float  tt_vec3_dot(tt_vec3_t first, tt_vec3_t second);
extern tt_vec3_t tt_vec3_cross(tt_vec3_t first, tt_vec3_t second);
extern tt_vec3_t tt_vec3_normalise(tt_vec3_t vec);

extern tt_vec4_t tt_vec4_create(float x, float y, float z, float w);
extern float  tt_vec4_length(tt_vec4_t vec);
extern tt_vec4_t tt_vec4_add(tt_vec4_t first, tt_vec4_t second);
extern tt_vec4_t tt_vec4_sub(tt_vec4_t first, tt_vec4_t second);
extern tt_vec4_t tt_vec4_mul(tt_vec4_t first, tt_vec4_t second);
extern tt_vec4_t tt_vec4_div(tt_vec4_t first, tt_vec4_t second);
extern float  tt_vec4_dot(tt_vec4_t first, tt_vec4_t second);
extern tt_vec4_t tt_vec4_normalise(tt_vec4_t vec);

extern int  tt_matrix_multiply(float l[16], float r[16]);
extern void tt_matrix_copy(float to[16], float from[16]);
extern int  tt_matrix_identity(float m[16]);
extern int  tt_matrix_translate3f(float m[16], float dx, float dy, float dz);
extern int  tt_matrix_translate3fv(float m[16], tt_vec3_t vec);
extern int  tt_matrix_scale3f(float m[16], float w, float h, float d);
extern int  tt_matrix_scale3fv(float m[16], tt_vec3_t vec);
extern int  tt_matrix_rotate3f(float m[16], float x, float y, float z, float angle);
extern int  tt_matrix_rotate3fv(float m[16], tt_vec3_t, float angle);
extern int  tt_matrix_orthographic(float m[16], float t, float r, float b, float l, float n, float f);

/* rect */
#define TT_RECT_VERTEX_LOC 0
#define TT_RECT_TEXVERTEX_LOC 1

typedef enum TT_RECT_ENUM {
        TT_RECT_NO_ERROR = 0,
        TT_RECT_NULL_ERROR,
        TT_RECT_PROGRAM_ERROR,
        TT_RECT_TEXTURE_ERROR,
        TT_RECT_ENUM_COUNT
} TT_RECT_ENUM;

typedef struct tt_rect_t {
        GLuint vbo, ebo, vao, program, texture, colour;
        GLfloat x, y, w, h;
} tt_rect_t;

extern int tt_rect_init(tt_rect_t *rect, GLfloat x, GLfloat y, GLfloat w, GLfloat h,
                            GLuint program, GLuint texture, GLuint colour);
extern void tt_rect_clip4f(tt_rect_t *rect, GLfloat x,
                             GLfloat y, GLfloat w, GLfloat h);
extern void tt_rect_clip4fv(tt_rect_t *rect, tt_vec4_t clip_rect);
extern void tt_rect_set_x(tt_rect_t *rect, GLfloat x);
extern void tt_rect_set_y(tt_rect_t *rect, GLfloat y);
extern void tt_rect_set_width(tt_rect_t *rect, GLfloat w);
extern void tt_rect_set_height(tt_rect_t *rect, GLfloat h);
extern void tt_rect_set_colour(tt_rect_t *rect, GLuint colour);
extern GLfloat tt_rect_get_x(tt_rect_t *rect);
extern GLfloat tt_rect_get_y(tt_rect_t *rect);
extern GLfloat tt_rect_get_width(tt_rect_t *rect);
extern GLfloat tt_rect_get_height(tt_rect_t *rect);
extern void tt_rect_render(tt_rect_t *rect);
extern void tt_rect_free(tt_rect_t *rect);

/* text */
#define TT_METRICTABLE_SIZE 16

typedef enum TT_GLYPH_ENUM {
        TT_GLYPH_NO_ERROR = 0,
        TT_GLYPH_NULL_ERROR,
        TT_GLYPH_FONT_ERROR,
        TT_GLYPH_TEXT_ERROR,
        TT_GLYPH_TEXTURE_ERROR,
        TT_GLYPH_ENUM_COUNT
} TT_GLYPH_ENUM;

typedef enum TT_METRIC_ENUM {
        TT_METRIC_NO_ERROR = 0,
        TT_METRIC_NULL_ERROR,
        TT_METRIC_ENUM_COUNT
} TT_METRIC_ENUM;

typedef struct tt_metric_t {
        char key;
        int minx, maxx, miny, maxy, advance, offset;
        struct tt_metric_t *next;
} tt_metric_t;

typedef struct tt_glyph_t {
        GLuint texture;
        GLuint tw, th;
        GLuint w, h;
        TTF_Font *font;
        tt_metric_t *table[TT_METRICTABLE_SIZE];
} tt_glyph_t;

typedef enum {
        TT_TEXT_NO_ERROR = 0,
        TT_TEXT_NULL_ERROR,
        TT_TEXT_PROGRAM_ERROR,
        TT_TEXT_ENUM_COUNT
} TT_TEXT_ENUM;

typedef struct tt_text_t {
        tt_buffer_t buffer;
        tt_rect_t rect;
} tt_text_t;

extern tt_metric_t *tt_metric_create(char key, int minx, int maxx,
                                         int miny, int maxy, int advance, int offset);
extern int tt_metric_init(tt_metric_t *metric, char key, int minx, int maxx,
                            int miny, int maxy, int advance, int offset);
extern tt_metric_t *tt_metric_insert(tt_metric_t *metric, char key, int minx, int maxx,
                                         int miny, int maxy, int advance, int offset);
extern tt_metric_t *tt_metric_lookup(tt_metric_t *metric, char key);
extern void tt_metric_free(tt_metric_t *metric);
extern int tt_glyph_init(tt_glyph_t *glyph, const char *path,
                                      const char *sample, GLuint ptsize);
extern void tt_glyph_free(tt_glyph_t *glyph);

extern tt_text_t *tt_text_create(const char *data, tt_glyph_t *glyph, GLuint program,
                                     GLfloat x, GLfloat y, GLuint colour);
extern int tt_text_init(tt_text_t *text,  const char *data, tt_glyph_t *glyph,
                          GLuint program, GLfloat x, GLfloat y, GLuint colour);
extern int tt_text_resize(tt_text_t *text);
extern int tt_text_append(tt_text_t *text, char c);
extern int tt_text_write(tt_text_t *text, const char *data);
extern int tt_text_clear(tt_text_t *text);
extern GLfloat tt_text_get_height(tt_text_t *text, tt_glyph_t *glyph);
extern GLfloat tt_text_get_width(tt_text_t *text, tt_glyph_t *glyph);
extern GLfloat tt_text_get_x(tt_text_t *text);
extern GLfloat tt_text_get_y(tt_text_t *text);
extern size_t tt_text_get_length(tt_text_t *text);
extern char tt_text_get_charat(tt_text_t *text, int index);
extern void tt_text_set_charat(tt_text_t *text, int index, char c);
extern void tt_text_set_x(tt_text_t *text, GLfloat x);
extern void tt_text_set_y(tt_text_t *text, GLfloat y);
extern void tt_text_render(tt_text_t *text, tt_glyph_t *glyph);
extern void tt_text_free(tt_text_t *text);

/* timer */
extern void tt_timer_start();
extern void tt_timer_reset();
extern float tt_timer_get_time();

/* button */
typedef struct tt_button_t {
        tt_rect_t btn;
        tt_rect_t icon;
} tt_button_t;

typedef enum TT_BUTTON_ENUM {
        TT_BUTTON_NO_ERROR = 0,
        TT_BUTTON_MEMORY_ERROR,
        TT_BUTTON_NULL_ERROR,
} TT_BUTTON_ENUM;


extern tt_button_t *tt_button_create(GLfloat x, GLfloat y, GLfloat btn_w, GLfloat btn_h,
                                     GLfloat icon_w, GLfloat icon_h,
                                     GLuint btn_program, GLuint icon_program,
                                     GLuint icon_texture, GLuint btn_colour, GLuint icon_colour);
extern int tt_button_init(tt_button_t *button, GLfloat x, GLfloat y, GLfloat btn_w, GLfloat btn_h,
                          GLfloat icon_w, GLfloat icon_h,
                          GLuint btn_program, GLuint icon_program,
                          GLuint icon_texture, GLuint btn_colour, GLuint icon_colour);
extern void tt_button_set_x(tt_button_t *button, GLfloat x);
extern void tt_button_set_y(tt_button_t *button, GLfloat y);
extern void tt_button_set_width(tt_button_t *button, GLfloat w);
extern void tt_button_set_height(tt_button_t *button, GLfloat h);
extern GLfloat tt_button_get_x(tt_button_t *button);
extern GLfloat tt_button_get_y(tt_button_t *button);
extern GLfloat tt_button_get_width(tt_button_t *button);
extern GLfloat tt_button_get_height(tt_button_t *button);
extern void tt_button_events(tt_button_t *button, void (*func)(void));
extern void tt_button_render(tt_button_t *button);
extern void tt_button_free(tt_button_t *button);

#endif // TT_H_
