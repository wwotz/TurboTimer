#include "../include/tt.h"

tt_metric_t *tt_metric_create(char key, int minx, int maxx,
                              int miny, int maxy, int advance, int offset)
{
        tt_metric_t *metric;
        metric = malloc(sizeof(*metric));
        if (!metric) {
                TT_PUSH_ERROR("failed to allocate memory for metric structure!");
                return NULL;
        }

        metric->key = key;
        metric->next = NULL;
        metric->minx = minx;
        metric->maxx = maxx;
        metric->miny = miny;
        metric->maxy = maxy;
        metric->advance = advance;
        metric->offset = offset;
        return metric;

}

int tt_metric_init(tt_metric_t *metric, char key, int minx, int maxx,
                   int miny, int maxy, int advance, int offset)
{
        if (!metric) {
                TT_PUSH_ERROR("metric was null!");
                return TT_METRIC_NULL_ERROR;
        }

        metric->key = key;
        metric->next = NULL;
        metric->minx = minx;
        metric->maxx = maxx;
        metric->miny = miny;
        metric->maxy = maxy;
        metric->advance = advance;
        metric->offset = offset;
        return TT_METRIC_NO_ERROR;
}

tt_metric_t *tt_metric_insert(tt_metric_t *metric, char key, int minx, int maxx,
                              int miny, int maxy, int advance, int offset)
{
        tt_metric_t *node;
        if (!metric) {
                TT_PUSH_ERROR("metric was null!");
                return NULL;
        }

        node = tt_metric_create(key, minx, maxx, miny, maxy, advance, offset);
        if (!node) {
                TT_PUSH_ERROR("failed to insert into metric!");
                return metric;
        }

        node->next = metric;
        metric = node;
        return metric;
}

tt_metric_t *tt_metric_lookup(tt_metric_t *metric, char key)
{
        tt_metric_t *ptr;
        if (!metric) {
                TT_PUSH_ERROR("metric was null!");
                return NULL;
        }

        ptr = metric;
        while (ptr != NULL && ptr->key != key)
                ptr = ptr->next;
        return (ptr ? ptr : metric);
}

void tt_metric_free(tt_metric_t *metric)
{
        tt_metric_t *ptr, *next;
        if (metric) {
                ptr = metric;
                while (ptr != NULL) {
                        next = ptr->next;
                        free(ptr);
                        ptr = next;
                }
        }
}

static int tt_glyph_hash(char c)
{
        return c % TT_METRICTABLE_SIZE;
}

int tt_glyph_init(tt_glyph_t *glyph, const char *path,
                  const char *sample, GLuint ptsize)
{
        GLuint texture;
        SDL_Surface *src_surface, *dest_surface;
        SDL_Color colour;
        TTF_Font *font;

        if (!glyph) {
                TT_PUSH_ERROR("glyph was null!");
                return TT_GLYPH_NULL_ERROR;
        }

        font = TTF_OpenFont(path, ptsize);
        if (!font) {
                TT_PUSH_ERROR(TTF_GetError());
                return TT_GLYPH_FONT_ERROR;
        }

        colour.r = colour.g = colour.b = colour.a = 0xff;
        src_surface = TTF_RenderText_Blended(font, sample, colour);
        if (!src_surface) {
                TT_PUSH_ERROR(TTF_GetError());
                return TT_GLYPH_TEXT_ERROR;
        }

        GLuint w = 1, h = 1;
        while (w <= src_surface->w)
                w <<= 1;
        while (h <= src_surface->h)
                h <<= 1;
        dest_surface = SDL_CreateRGBSurface(0, w, h, 32, src_surface->format->Rmask,
                                            src_surface->format->Gmask,
                                            src_surface->format->Bmask,
                                            src_surface->format->Amask);
        SDL_BlitSurface(src_surface, NULL, dest_surface, NULL);

        tt_texture_info_t tinfo = {
                TT_TEXTURE_PIXELS,
                0,
                dest_surface->pixels,
                dest_surface->w,
                dest_surface->h,
        };
        texture = tt_texture_create(tinfo);

        if (!texture)
                return TT_GLYPH_TEXTURE_ERROR;

        glyph->texture = texture;
        glyph->tw = dest_surface->w;
        glyph->th = dest_surface->h;
        glyph->h = TTF_FontHeight(font);
        glyph->w = 0;

        for (int i = 0; i < TT_METRICTABLE_SIZE; i++)
                glyph->table[i] = 0;

        for (int i = 0; sample[i] != '\0'; i++) {
                int minx, maxx, miny, maxy, advance;
                int hash = tt_glyph_hash(sample[i]);
                TTF_GlyphMetrics(font, sample[i], &minx, &maxx, &miny,
                                 &maxy, &advance);
                if (glyph->table[hash]) {
                        glyph->table[hash] = tt_metric_insert(glyph->table[hash],
                                                              sample[i], minx, maxx, miny, maxy, advance, glyph->w);
                } else {
                        glyph->table[hash] = tt_metric_create(sample[i], minx, maxx, miny, maxy, advance, glyph->w);
                }
                glyph->w += advance;
        }

        glyph->font = font;

        SDL_FreeSurface(src_surface);
        SDL_FreeSurface(dest_surface);
        return TT_GLYPH_NO_ERROR;
}

void tt_glyph_free(tt_glyph_t *glyph)
{
        if (glyph) {
                glDeleteTextures(1, &glyph->texture);
                glyph->texture = 0;
                glyph->w = 0;
                glyph->h = 0;
                TTF_CloseFont(glyph->font);
                glyph->font = NULL;
        }
}

tt_text_t *tt_text_create(const char *data, tt_glyph_t *glyph, GLuint program,
                          GLfloat x, GLfloat y, GLuint colour)
{
        int status;
        tt_text_t *text;

        if (!program || !glIsProgram(program))
                return NULL;

        text = malloc(sizeof(*text));
        if (!text) {
                TT_PUSH_ERROR("failed to allocate memory for text structure!");
                return NULL;
        }

        if ((status = tt_buffer_init(&text->buffer, data)) != TT_BUFFER_NO_ERROR) {
                tt_text_free(text);
                free(text);
                return NULL;
        }

        if ((status = tt_rect_init(&text->rect, x, y, 0.0, 0.0, program, glyph->texture, 0x000000ff))
            != TT_RECT_NO_ERROR) {
                tt_text_free(text);
                free(text);
                return NULL;
        }

        return text;
}

int tt_text_init(tt_text_t *text, const char *data, tt_glyph_t *glyph,
                 GLuint program, GLfloat x, GLfloat y, GLuint colour)
{
        int status;

        if (!program || !glIsProgram(program))
                return TT_TEXT_PROGRAM_ERROR;

        if (!text) {
                TT_PUSH_ERROR("text structure was null!");
                return TT_TEXT_NULL_ERROR;
        }

        if ((status = tt_buffer_init(&text->buffer, data)) != TT_BUFFER_NO_ERROR)
                return status;

        if ((status = tt_rect_init(&text->rect, x, y, 0.0, 0.0, program, glyph->texture,
                                     colour)) != TT_RECT_NO_ERROR) {
                tt_text_free(text);
                return status;
        }
        return TT_TEXT_NO_ERROR;
}

int tt_text_resize(tt_text_t *text)
{
        return tt_buffer_resize(&text->buffer);
}

int tt_text_append(tt_text_t *text, char c)
{
        return tt_buffer_append(&text->buffer, c);
}

int tt_text_write(tt_text_t *text, const char *data)
{
        return tt_buffer_write(&text->buffer, data);
}

int tt_text_clear(tt_text_t *text)
{
        return tt_buffer_clear(&text->buffer);
}

GLfloat tt_text_get_height(tt_text_t *text, tt_glyph_t *glyph)
{
        return (float) glyph->h;
}

GLfloat tt_text_get_width(tt_text_t *text, tt_glyph_t *glyph)
{
        GLfloat width = 0;
        char c;
        tt_metric_t *found;

        if (text && glyph) {
                for (int i = 0; (c = text->buffer.data[i]) != '\0'; i++) {
                        found = tt_metric_lookup(glyph->table[tt_glyph_hash(c)], c);
                        if (!found) return 0;
                        width += found->advance;
                }
        }

        return (float) width;
}

GLfloat tt_text_get_x(tt_text_t *text)
{
        return tt_rect_get_x(&text->rect);
}

GLfloat tt_text_get_y(tt_text_t *text)
{
        return tt_rect_get_height(&text->rect);
}

size_t tt_text_get_length(tt_text_t *text)
{
        if (text) {
                return tt_buffer_length(&text->buffer);
        }
        return 0;
}

char tt_text_get_charat(tt_text_t *text, int index)
{
        if (text) {
                return tt_buffer_get_charat(&text->buffer, index);
        }
        return '\0';
}

void tt_text_set_charat(tt_text_t *text, int index, char c)
{
        if (text)
                tt_buffer_set_charat(&text->buffer, index, c);
}

void tt_text_set_x(tt_text_t *text, GLfloat x)
{
        if (text) {
                tt_rect_set_x(&text->rect, x);
        }
}

void tt_text_set_y(tt_text_t *text, GLfloat y)
{
        if (text) {
                tt_rect_set_y(&text->rect, y);
        }
}

void tt_text_render(tt_text_t *text, tt_glyph_t *glyph)
{
        float sav_x, sav_y;
        char c;
        tt_metric_t *found;
        if (text && glyph) {
                sav_x = text->rect.x, sav_y = text->rect.y;
                for (int i = 0; (c = text->buffer.data[i]) != '\0' && c != '\n'; i++) {
                        found = tt_metric_lookup(glyph->table[tt_glyph_hash(c)], c);
                        if (found) {
                                tt_rect_clip4f(&text->rect,
                                                 (GLfloat) (found->offset)/glyph->tw,
                                                 (GLfloat) 0.0,
                                                 (GLfloat) (found->offset + found->advance)/glyph->tw,
                                                 (GLfloat) glyph->h/glyph->th);
                                tt_rect_set_width(&text->rect, found->advance);
                                tt_rect_set_height(&text->rect, glyph->h);
                                tt_rect_render(&text->rect);
                                tt_text_set_x(text, tt_text_get_x(text) + found->advance);
                        }
                }
                text->rect.x = sav_x;
                text->rect.y = sav_y;
        }
}

void tt_text_free(tt_text_t *text)
{
        if (text) {
                tt_buffer_free(&text->buffer);
                tt_rect_free(&text->rect);
        }
}
