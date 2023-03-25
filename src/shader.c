#include "../include/tt.h"

static GLboolean tt_shader_had_error(GLuint shader)
{
        if (glIsShader(shader)) {
                GLint status;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
                return status == GL_FALSE;
        }
        return GL_FALSE;
}

static GLchar *tt_shader_get_error(GLuint shader)
{
        if (glIsShader(shader)) {
                GLchar *msg;
                GLsizei length, max_length;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

                msg = malloc(max_length);
                if (!msg) {
                        fprintf(stderr, "Failed to allocate memory for shader error message!");
                        return NULL;
                }

                glGetShaderInfoLog(shader, max_length, &length, msg);
                if (length <= 0) {
                        free(msg);
                        msg = NULL;
                }
                return msg;
        }
        return NULL;
}

static GLuint tt_program_had_error(GLuint program)
{
        if (glIsProgram(program)) {
                GLint status;
                glGetProgramiv(program, GL_LINK_STATUS, &status);
                return status == GL_FALSE;
        }
        return GL_FALSE;
}

static GLchar *tt_program_get_error(GLuint program)
{
        if (glIsProgram(program)) {
                GLchar *msg;
                GLsizei length, max_length;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

                msg = malloc(max_length);
                if (!msg) {
                        fprintf(stderr, "Failed to allocate memory for shader error message!");
                        return NULL;
                }

                glGetProgramInfoLog(program, max_length, &length, msg);
                if (length <= 0) {
                        free(msg);
                        msg = NULL;
                }
                return msg;
        }
        return NULL;
}

GLuint tt_shader_create(tt_shader_info_t sinfo)
{
        GLuint shader;
        char *data, *msg;

        shader = glCreateShader(sinfo.stype);
        switch (sinfo.dtype) {
        case TT_SHADER_FILE_TYPE:
                data = tt_utils_fmap(sinfo.data);
                if (!data) return 0;
                glShaderSource(shader, 1, (const GLchar **)&data, NULL);
                break;
        default:
        case TT_SHADER_SOURCE_TYPE:
                glShaderSource(shader, 1, (const GLchar **)&sinfo.data, NULL);
                break;
        }

        glCompileShader(shader);
        if (tt_shader_had_error(shader)) {
                msg = tt_shader_get_error(shader);
                glDeleteShader(shader);
                shader = 0;
                TT_PUSH_ERROR(msg);
                free(msg);
        }
        return shader;
}

GLuint tt_program_create(tt_program_info_t pinfo)
{
        GLuint program, vshader, fshader;
        char *msg;

        program = glCreateProgram();
        vshader = tt_shader_create(pinfo.vinfo);
        if (!vshader) {
                glDeleteProgram(program);
                return 0;
        }
        glAttachShader(program, vshader);
        glDeleteShader(vshader);

        fshader = tt_shader_create(pinfo.finfo);
        if (!fshader) {
                glDeleteProgram(program);
                return 0;
        }
        glAttachShader(program, fshader);
        glDeleteShader(fshader);

        glLinkProgram(program);
        if (tt_program_had_error(program)) {
                msg = tt_program_get_error(program);
                glDeleteProgram(program);
                program = 0;
                TT_PUSH_ERROR(msg);
                free(msg);
        }

        return program;
}
