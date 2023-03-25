#include "../include/tt.h"

/* returns a memory allocate tt_buffer_t structure,
   returning NULL on error */
tt_buffer_t *tt_buffer_create(const char *data)
{
        tt_buffer_t *buffer = malloc(sizeof(*buffer));
        if (!buffer) {
                TT_PUSH_ERROR("failed to allocate memory for buffer!");
                return NULL;
        }

        if (tt_buffer_init(buffer, data) != TT_BUFFER_NO_ERROR) {
                tt_buffer_free(buffer);
                free(buffer);
                return NULL;
        }

        return buffer;
}

int tt_buffer_init(tt_buffer_t *buffer, const char *data)
{
        if (!buffer) {
                TT_PUSH_ERROR("buffer was null!");
                return TT_BUFFER_NULL_ERROR;
        }

        buffer->size = 0;
        buffer->capacity = TT_BUFFER_INITIAL_CAPACITY;
        buffer->data = calloc(buffer->capacity, sizeof(*buffer->data));
        if (!buffer->data) {
                TT_PUSH_ERROR("failed to allocate space for data!");
                return TT_BUFFER_DATA_ERROR;
        }

        return tt_buffer_write(buffer, data);
}

int tt_buffer_resize(tt_buffer_t *buffer)
{
        char *new_data;
        size_t new_size;
        if (!buffer) {
                TT_PUSH_ERROR("buffer was null!");
                return TT_BUFFER_NULL_ERROR;
        }

        new_size = sizeof(*buffer->data) * buffer->capacity * 2;
        new_data = realloc(buffer->data, new_size);
        if (!new_data) {
                TT_PUSH_ERROR("failed to resize space for data!");
                return TT_BUFFER_RESIZE_ERROR;
        }

        buffer->capacity *= 2;
        buffer->data = new_data;
        return TT_BUFFER_NO_ERROR;
}

int tt_buffer_write(tt_buffer_t *buffer, const char *data)
{
        return tt_buffer_insert(buffer, data, 0);
}

int tt_buffer_insert(tt_buffer_t *buffer, const char *data, int offset)
{
        if (!buffer) {
                TT_PUSH_ERROR("buffer was null!");
                return TT_BUFFER_NULL_ERROR;
        }

        size_t data_len;
        int status;

        data_len = strlen(data);
        status = TT_BUFFER_NO_ERROR;
        while (buffer->capacity <= data_len + offset) {
                if ((status = tt_buffer_resize(buffer)) != TT_BUFFER_NO_ERROR)
                    return status;
        }

        memcpy(buffer->data + offset, data, data_len);
        buffer->size = data_len + offset;
        buffer->data[buffer->size] = '\0';
        return TT_BUFFER_NO_ERROR;
}

int tt_buffer_clear(tt_buffer_t *buffer)
{
        if (!buffer) {
                TT_PUSH_ERROR("buffer was null!");
                return TT_BUFFER_NULL_ERROR;
        }

        memset(buffer->data, 0, buffer->capacity * sizeof(*buffer->data));
        buffer->size = 0;
}

char tt_buffer_get_char(tt_buffer_t *buffer, int index)
{
        if (index < buffer->size && index >= 0)
                return buffer->data[index];
        return '\0';
}

void tt_buffer_free(tt_buffer_t *buffer)
{
        if (buffer) {
                if (buffer->data)
                        free(buffer->data);
                buffer->data = NULL;
                buffer->size = 0;
                buffer->capacity = 0;
        }
}
