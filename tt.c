#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include <sys/time.h>
#include <assert.h>

#define LINEARLIB_IMPLEMENTATION
#define FTGL_IMPLEMENTATION
#include "font.h"

#define FONT_NAME "Alata-Regular.ttf"

#define FPS 60.0

#define WINDOW_NAME "TurboTimer"
#define WINDOW_W 800
#define WINDOW_H 400

static SDL_Window *window;
static SDL_GLContext *context;
static SDL_Event event;
static int running;

enum gl_buffers {
	BUFFER_VBO,
	BUFFER_EBO,
	BUFFER_VAO,
	BUFFER_COUNT
};
static GLuint buffers[BUFFER_COUNT];
vec4_t vertices[4] = {
	{ 0.0, 0.0, 0.0, 0.0 },
	{ 1.0, 0.0, 1.0, 0.0 },
	{ 1.0, 1.0, 1.0, 1.0 },
	{ 0.0, 1.0, 0.0, 1.0 }
};

const GLuint indices[6] = {
	0, 1, 2,
	2, 3, 0
};

static GLuint text_shader;

static ivec4_t
time_elapsed(Uint32 ms);

static struct {
	int get_time;
	Uint32 start;
	Uint32 prev;
	Uint32 curr;
} stopwatch = { 0, 0, 0, 0 };

static ivec4_t
stopwatch_elapsed(void);

static void
stopwatch_start(void);

static void
stopwatch_reset(void);

static void
stopwatch_pause(void);

static void
clip4f(GLfloat x, GLfloat y, GLfloat w, GLfloat h);

static ftgl_font_t *number_font_big;
static ftgl_font_t *number_font_little;

int
main(int argc, char **argv)
{
	window = SDL_CreateWindow(WINDOW_NAME,
				  SDL_WINDOWPOS_UNDEFINED,
				  SDL_WINDOWPOS_UNDEFINED,
				  WINDOW_W, WINDOW_H,
				  SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(window);
	glewInit();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SDL_SetWindowAlwaysOnTop(window, SDL_TRUE);

	glViewport(0.0, 0.0, WINDOW_W, WINDOW_H);
	glClearColor(0.08235294117647059,
		     0.0196078431372549,
		     0.058823529411764705,
		     1.0);

	ll_matrix_mode(LL_MATRIX_MODEL);
	ll_matrix_identity();
	ll_matrix_mode(LL_MATRIX_PROJECTION);
	ll_matrix_orthographic(0.0, WINDOW_W, WINDOW_H, 0.0, 10.0, -10.0);

	if (ftgl_font_library_init() != FTGL_NO_ERROR) {
		fprintf(stderr, "Failed to initialize freetype library!\n");
		exit(EXIT_FAILURE);
	}

	number_font_big = ftgl_font_create();
	if (!number_font_big) {
		fprintf(stderr, "Failed to create font!\n");
		exit(EXIT_FAILURE);
	}

	number_font_little = ftgl_font_create();
	if (!number_font_little) {
		fprintf(stderr, "Failed to create font!\n");
		exit(EXIT_FAILURE);
	}

	if (ftgl_font_bind(number_font_big, FONT_NAME)
	    != FTGL_NO_ERROR) {
		fprintf(stderr, "Failed to bind: '%s'\n", FONT_NAME);
		exit(EXIT_FAILURE);
	}
	
	if (ftgl_font_set_size(number_font_big, 64) != FTGL_NO_ERROR) {
		fprintf(stderr, "Failed to set size of big font!\n");
		exit(EXIT_FAILURE);
	}

	if (ftgl_font_bind(number_font_little, FONT_NAME)
	    != FTGL_NO_ERROR) {
		fprintf(stderr, "Failed to bind: '%s'\n" FONT_NAME);
		exit(EXIT_FAILURE);
	}

	if (ftgl_font_set_size(number_font_little, 32) != FTGL_NO_ERROR) {
		fprintf(stderr, "Failed to set size of little font!\n");
		exit(EXIT_FAILURE);
	}

	char c;
	const char *font_sample = "0123456789.:s";
	for (size_t i = 0; (c = font_sample[i]) != '\0'; i++) {
		ftgl_glyph_t *glyph_big, *glyph_little;
		glyph_big = ftgl_font_load_codepoint(number_font_big, c);
		glyph_little = ftgl_font_load_codepoint(number_font_little, c);
		if (!glyph_big || !glyph_little) {
			fprintf(stderr, "Failed to load codepoint: %c\n", c);
			exit(EXIT_FAILURE);
		}
	}

	glGenVertexArrays(1, buffers+BUFFER_VAO);
	glBindVertexArray(buffers[BUFFER_VAO]);

	glGenBuffers(2, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[BUFFER_VAO]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
		     GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[BUFFER_EBO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
		     GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(*vertices),
			      (const GLvoid *) 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(*vertices),
			      (const GLvoid *) (2 * sizeof(vertices->data[0])));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	GLuint vshader, fshader;
	const char *vsource = "#version 330 core\n"
		"uniform mat4 model;"
		"uniform mat4 projection;"
		"layout (location = 0) in vec2 vertex;"
		"layout (location = 1) in vec2 t_vertex;"
		"out vec2 tex_vertex;"
		"void main()"
		"{"
		"tex_vertex = t_vertex;"
		"gl_Position = projection * model * vec4(vertex, 0.0, 1.0);"
		"}";
	
	const char *fsource = "#version 330 core\n"
		"uniform sampler2D texture_unit;"
		"uniform vec4 colour;"
		"in vec2 tex_vertex;"
		"void main()"
		"{"
		"float r = texture2D(texture_unit, tex_vertex).r;"
		"gl_FragColor = vec4(colour.rgb, colour.a * r);"
		"}";

	vshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vshader, 1, (const GLchar **) &vsource, NULL);
	glCompileShader(vshader);

	fshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fshader, 1, (const GLchar **) &fsource, NULL);
	glCompileShader(fshader);

	text_shader = glCreateProgram();
	glAttachShader(text_shader, vshader);
	glAttachShader(text_shader, fshader);
	glLinkProgram(text_shader);

	glUseProgram(text_shader);

	ll_matrix_mode(LL_MATRIX_PROJECTION);
	glUniformMatrix4fv(glGetUniformLocation(text_shader, "projection"), 1,
			   GL_FALSE, ll_matrix_get_copy().data);
	

	glBindTexture(GL_TEXTURE_2D, number_font_big->textures[0]);

#define TIME_FORMAT_LENGTH 64
	char time_format[TIME_FORMAT_LENGTH];
	memset(time_format, 0, sizeof(time_format));
	strcpy(time_format, "00:00:00.000");

#define TEX_COORD_MAP(x, y, w, h)			\
	(x / (float) FTGL_FONT_ATLAS_WIDTH),		\
		(y / (float) FTGL_FONT_ATLAS_HEIGHT),	\
		(w / (float) FTGL_FONT_ATLAS_WIDTH),	\
		(h / (float) FTGL_FONT_ATLAS_HEIGHT)	\

	running = 1;

	Uint32 prev_seconds = 0;
	int get_time = 0, animate = 0, animate_frame = 0;

	#define ANIMATE_FRAMES 11
	vec2_t animate_frames[ANIMATE_FRAMES];
	for (size_t i = 0; i < ANIMATE_FRAMES; i++) {
		float x = 200;
		float y = 200 + 20.0*sin((M_PI/(float) (ANIMATE_FRAMES - 1) * i));
		animate_frames[i] = ll_vec2_create2f( x, y );
	}

	Uint32 start_time, end_time;
	float elapsed_time = 0.0, allowed_time = 1.0 / (float) FPS;
	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = 0;
				break;
			}

			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_s:
					stopwatch_start();
					break;
				case SDLK_p:
					stopwatch_pause();
					break;
				case SDLK_r:
					stopwatch_reset();
					break;
				}
			}
		}

		start_time = SDL_GetTicks();
		
		
		glBindTexture(GL_TEXTURE_2D,
			      number_font_big->textures[0]);
		glClear(GL_COLOR_BUFFER_BIT);
		ll_matrix_mode(LL_MATRIX_MODEL);

		glUniform4f(glGetUniformLocation(text_shader, "colour"),
		    1.0, 1.0, 1.0, 1.0);

		ivec4_t elapsed = stopwatch_elapsed();
		if (elapsed.z != prev_seconds) {
			prev_seconds = elapsed.z;
			animate = 1;
		}
		
		snprintf(time_format, TIME_FORMAT_LENGTH,
			 "%.2d:%.2d:%.2d.%.3d",
			 elapsed.x, elapsed.y,
			 elapsed.z, elapsed.w);

		vec2_t pen;
		if (animate) {
			glUniform4f(glGetUniformLocation(text_shader, "colour"),
				    0.0, 1.0, 0.0, 1.0);
			if (animate_frame == ANIMATE_FRAMES - 1) {
				animate = 0;
				animate_frame = 0;
			}
			pen = animate_frames[animate_frame++];
		} else {
			pen = ll_vec2_create2f( 200, 200 );
		}
		
		char c;
		ftgl_font_t *sample_font = number_font_big;
		for (size_t i = 0; (c = time_format[i]) != '\0'; i++) {
			ftgl_glyph_t *glyph;
			if (time_format[i] == '.') {
				glBindTexture(GL_TEXTURE_2D,
					      number_font_little->textures[0]);
				sample_font = number_font_little;
				continue;
			}
			glyph = ftgl_font_find_glyph(sample_font, c);
			assert(glyph);
			
			ll_matrix_identity();
			ll_matrix_scale3f(glyph->w,
					  glyph->h, 1.0);
			ll_matrix_translate3f((int) (pen.x + glyph->offset_x),
					      (int) (pen.y - glyph->offset_y),
					      0.0);
			clip4f(TEX_COORD_MAP(glyph->x, glyph->y,
					     glyph->w, glyph->h));
			pen.x += (GLint) glyph->advance_x;
			glUniformMatrix4fv(glGetUniformLocation(text_shader, "model"), 1,
					   GL_FALSE, ll_matrix_get_copy().data);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
		}
		
		SDL_GL_SwapWindow(window);
		end_time = SDL_GetTicks();
		elapsed_time = (end_time - start_time) / 1000.0;
		elapsed_time = allowed_time - elapsed_time;
		if (elapsed_time > 0.0) {
			Uint32 elapsed_ms = (Uint32) (elapsed_time*1000.0);
			SDL_Delay(elapsed_ms);
		}
	}

	SDL_DestroyWindow(window);
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return 0;
}

static ivec4_t
time_elapsed(Uint32 ms)
{
	ivec4_t retvec;
	const Uint32 segments[4] = {
		3600000, 60000,
		1000, 1
	};
	
	for (size_t i = 0; i < 4; i++) {
		retvec.data[i] = ms / segments[i];
		ms -= retvec.data[i] * segments[i];
	}

	return retvec;
}

static ivec4_t
stopwatch_elapsed(void)
{
	if (stopwatch.get_time) {
		stopwatch.curr = SDL_GetTicks();
		return time_elapsed(stopwatch.prev
				    + (stopwatch.curr - stopwatch.start));
	} else {
		return time_elapsed(stopwatch.prev);
	}
}

static void
stopwatch_start(void)
{
	stopwatch.start = stopwatch.get_time ? stopwatch.start : SDL_GetTicks();
	stopwatch.get_time = 1;
}

static void
stopwatch_reset(void)
{
	stopwatch.start = 0;
	stopwatch.prev = 0;
	stopwatch.curr = 0;
	stopwatch.get_time = 0;
}

static void
stopwatch_pause(void)
{
	stopwatch.prev += (stopwatch.curr - stopwatch.start);
	stopwatch.curr = 0;
	stopwatch.start = 0;
	stopwatch.get_time = 0;
}

static void
clip4f(GLfloat x, GLfloat y, GLfloat w, GLfloat h)
{
	float s0 = x;
	float s1 = x + w;
	float t0 = y;
	float t1 = y + h;
	vertices[0] = (vec4_t) {{ 0.0, 0.0, s0, t0 }};
	vertices[1] = (vec4_t) {{ 1.0, 0.0, s1, t0 }};
	vertices[2] = (vec4_t) {{ 1.0, 1.0, s1, t1 }};
	vertices[3] = (vec4_t) {{ 0.0, 1.0, s0, t1 }};
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices),
			vertices);
}
