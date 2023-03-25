#include "../include/tt.h"

size_t tt_utils_fsize(FILE *fd)
{
        if (!fd) return 0;
        size_t fsize;
        fseek(fd, 0, SEEK_END);
        fsize = ftell(fd);
        fseek(fd, 0, SEEK_SET);
        return fsize;
}

char *tt_utils_fmap(const char *path)
{
        char *buffer;
        size_t buffer_len;
        FILE *fd;

        fd = fopen(path, "r");
        if (!fd) {
                TT_PUSH_ERROR("File does not exist!");
                return NULL;
        }

        buffer_len = tt_utils_fsize(fd);
        buffer = malloc(buffer_len + 1);
        if (!buffer) {
                TT_PUSH_ERROR("Failed to allocate memory for file!");
                return NULL;
        }

        size_t nread;
        while ((nread = fread(buffer, sizeof(*buffer), buffer_len, fd)) > 0)
                ;

        if (feof(fd)) {
                buffer[buffer_len] = '\0';
        } else if (ferror(fd)) {
                free(buffer);
                buffer = NULL;
        }

        fclose(fd);
        return buffer;
}
