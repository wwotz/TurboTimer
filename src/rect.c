#include "../include/tt.h"

static GLfloat vertices[16] = {
        0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
};

static GLuint indices[6] = {
        0, 1, 2,
        2, 3, 0
};

int tt_rect_init(tt_rect_t *rect, GLfloat x, GLfloat y, GLfloat w, GLfloat h,
                 GLuint program, GLuint texture, GLuint colour)
{
        if (!rect) {
                TT_PUSH_ERROR("rect was null!");
                return TT_RECT_NULL_ERROR;
        }

        rect->x = x;
        rect->y = y;
        rect->w = w;
        rect->h = h;
        rect->colour = colour;

        rect->program = program;
        if (!rect->program) {
                if (!tt_debug_had_error())
                        TT_PUSH_ERROR("OpenGL program was not provided!");
                return TT_RECT_PROGRAM_ERROR;
        }

        rect->texture = texture;

        glUseProgram(rect->program);
        glGenVertexArrays(1, &rect->vao);
        glBindVertexArray(rect->vao);

        glGenBuffers(1, &rect->vbo);
        glBindBuffer(GL_ARRAY_BUFFER, rect->vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

        glGenBuffers(1, &rect->ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rect->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(TT_RECT_VERTEX_LOC, 2, GL_FLOAT, GL_FALSE,
                              sizeof(*vertices) * 4, (GLvoid *)0);
        glVertexAttribPointer(TT_RECT_TEXVERTEX_LOC, 2, GL_FLOAT, GL_FALSE,
                              sizeof(*vertices) * 4, (GLvoid *)(sizeof(*vertices) * 2));

        glEnableVertexAttribArray(TT_RECT_VERTEX_LOC);
        glEnableVertexAttribArray(TT_RECT_TEXVERTEX_LOC);

        glBindVertexArray(0);
        glUseProgram(0);
        return TT_RECT_NO_ERROR;
}

void tt_rect_clip4f(tt_rect_t *rect, GLfloat x,
                    GLfloat y, GLfloat w, GLfloat h)
{
        if (rect) {
                vertices[2]  = x;
                vertices[3]  = y;
                vertices[6]  = w;
                vertices[7]  = y;
                vertices[10] = w;
                vertices[11] = h;
                vertices[14] = x;
                vertices[15] = h;

                glBindVertexArray(rect->vao);
                glBindBuffer(GL_ARRAY_BUFFER, rect->vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
                glBindVertexArray(0);

                vertices[2]  = 0.0f;
                vertices[3]  = 0.0f;
                vertices[6]  = 1.0f;
                vertices[7]  = 0.0f;
                vertices[10] = 1.0f;
                vertices[11] = 1.0f;
                vertices[14] = 0.0f;
                vertices[15] = 1.0f;
        }
}

void tt_rect_clip4fv(tt_rect_t *rect, tt_vec4_t clip_rect)
{
        tt_rect_clip4f(rect, clip_rect.x, clip_rect.y, clip_rect.z, clip_rect.w);
}

void tt_rect_set_x(tt_rect_t *rect, GLfloat x)
{
        if (rect)
                rect->x = x;
}

void tt_rect_set_y(tt_rect_t *rect, GLfloat y)
{
        if (rect)
                rect->y = y;
}

void tt_rect_set_width(tt_rect_t *rect, GLfloat w)
{
        if (rect)
                rect->w = w;
}

void tt_rect_set_height(tt_rect_t *rect, GLfloat h)
{
        if (rect)
                rect->h = h;
}

GLfloat tt_rect_get_x(tt_rect_t *rect)
{
        if (rect)
                return rect->x;
        return 0.0;
}

GLfloat tt_rect_get_y(tt_rect_t *rect)
{
        if (rect)
                return rect->y;
        return 0.0;
}

GLfloat tt_rect_get_width(tt_rect_t *rect)
{
        if (rect)
                return rect->w;
        return 0.0;
}

GLfloat tt_rect_get_height(tt_rect_t *rect)
{
        if (rect)
                return rect->h;
        return 0.0;
}

void tt_rect_render(tt_rect_t *rect)
{
        if (rect) {
                glUseProgram(rect->program);

                glBindTexture(GL_TEXTURE_2D, rect->texture);

                tt_matrix_identity(tt_model);
                tt_matrix_scale3f(tt_model, rect->w, rect->h, 1.0);
                tt_matrix_translate3f(tt_model, rect->x, rect->y, 0.0);

                glUniformMatrix4fv(glGetUniformLocation(rect->program, "model"), 1, GL_FALSE, tt_model);
                glUniformMatrix4fv(glGetUniformLocation(rect->program, "view"), 1, GL_FALSE, tt_view);
                glUniformMatrix4fv(glGetUniformLocation(rect->program, "projection"), 1, GL_FALSE, tt_projection);

                glUniform4f(glGetUniformLocation(rect->program, "colour"),
                            tt_colour_mask(rect->colour, TT_COLOUR_R, GL_TRUE),
                            tt_colour_mask(rect->colour, TT_COLOUR_G, GL_TRUE),
                            tt_colour_mask(rect->colour, TT_COLOUR_B, GL_TRUE),
                            tt_colour_mask(rect->colour, TT_COLOUR_A, GL_TRUE));

                glBindVertexArray(rect->vao);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
                glBindTexture(GL_TEXTURE_2D, 0);
                glUseProgram(0);
        }
}

void tt_rect_free(tt_rect_t *rect)
{
        if (rect) {
                glDeleteVertexArrays(1, &rect->vao);
                glDeleteBuffers(1, &rect->vbo);
                glDeleteBuffers(1, &rect->ebo);
                rect->vbo = 0;
                rect->ebo = 0;
                rect->vao = 0;
                rect->x = 0;
                rect->y = 0;
                rect->w = 0;
                rect->h = 0;
        }
}
