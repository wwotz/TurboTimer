#include "../include/tt.h"

tt_vec2_t tt_vec2_create(float x, float y)
{
        return (tt_vec2_t) { x, y };
}

float tt_vec2_length(tt_vec2_t vec)
{
        return sqrtf((vec.x * vec.x) + (vec.y * vec.y));
}

tt_vec2_t tt_vec2_add(tt_vec2_t first, tt_vec2_t second)
{
        return tt_vec2_create(first.x + second.x,
                                first.y + second.y);
}

tt_vec2_t tt_vec2_sub(tt_vec2_t first, tt_vec2_t second)
{
        return tt_vec2_create(first.x - second.x,
                                first.y - second.y);
}

tt_vec2_t tt_vec2_mul(tt_vec2_t first, tt_vec2_t second)
{
        return tt_vec2_create(first.x * second.x,
                                first.y * second.y);
}

tt_vec2_t tt_vec2_div(tt_vec2_t first, tt_vec2_t second)
{
        return tt_vec2_create(first.x / second.x,
                                first.y / second.y);
}

float tt_vec2_dot(tt_vec2_t first, tt_vec2_t second)
{
        return first.x * second.x + first.y * second.y;
}

float tt_vec2_cross(tt_vec2_t first, tt_vec2_t second)
{
        return first.x * second.y - first.y * second.x;
}

tt_vec2_t tt_vec2_normalise(tt_vec2_t vec)
{
        float length = tt_vec2_length(vec);
        return (tt_vec2_t) { vec.x / length, vec.y / length };
}

tt_vec3_t tt_vec3_create(float x, float y, float z)
{
        return (tt_vec3_t) { x, y, z };
}

float tt_vec3_length(tt_vec3_t vec)
{
        return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

tt_vec3_t tt_vec3_add(tt_vec3_t first, tt_vec3_t second)
{
        return tt_vec3_create(first.x + second.x,
                                first.y + second.y,
                                first.z + second.z);
}

tt_vec3_t tt_vec3_sub(tt_vec3_t first, tt_vec3_t second)
{
        return tt_vec3_create(first.x - second.x,
                                first.y - second.y,
                                first.z - second.z);
}

tt_vec3_t tt_vec3_mul(tt_vec3_t first, tt_vec3_t second)
{
        return tt_vec3_create(first.x * second.x,
                                first.y * second.y,
                                first.z * second.z);
}

tt_vec3_t tt_vec3_div(tt_vec3_t first, tt_vec3_t second)
{
        return tt_vec3_create(first.x / second.x,
                                first.y / second.y,
                                first.z / second.z);
}

float tt_vec3_dot(tt_vec3_t first, tt_vec3_t second)
{
        return first.x * second.x + first.y * second.y + first.z * second.z;
}

tt_vec3_t tt_vec3_cross(tt_vec3_t first, tt_vec3_t second)
{
        return tt_vec3_create(first.y * second.z - first.z * second.y,
                                first.z * second.x - first.x * second.z,
                                first.x * second.y - first.y * second.x);
}

tt_vec3_t tt_vec3_normalise(tt_vec3_t vec)
{
        float length = tt_vec3_length(vec);
        return (tt_vec3_t) { vec.x / length, vec.y / length, vec.z / length };
}

tt_vec4_t tt_vec4_create(float x, float y, float z, float w)
{
        return (tt_vec4_t) { x, y, z, w };
}

float tt_vec4_length(tt_vec4_t vec)
{
        return sqrtf(vec.x * vec.x + vec.y * vec.y
                     + vec.z * vec.z + vec.w * vec.w);
}

tt_vec4_t tt_vec4_add(tt_vec4_t first, tt_vec4_t second)
{
        return tt_vec4_create(first.x + second.x,
                                first.y + second.y,
                                first.z + second.z,
                                first.w + second.w);
}

tt_vec4_t tt_vec4_sub(tt_vec4_t first, tt_vec4_t second)
{
        return tt_vec4_create(first.x - second.x,
                                first.y - second.y,
                                first.z - second.z,
                                first.w - second.w);
}

tt_vec4_t tt_vec4_mul(tt_vec4_t first, tt_vec4_t second)
{
        return tt_vec4_create(first.x * second.x,
                                first.y * second.y,
                                first.z * second.z,
                                first.w * second.w);
}

tt_vec4_t tt_vec4_div(tt_vec4_t first, tt_vec4_t second)
{
        return tt_vec4_create(first.x / second.x,
                                first.y / second.y,
                                first.z / second.z,
                                first.w / second.w);
}

float tt_vec4_dot(tt_vec4_t first, tt_vec4_t second)
{
        return first.x * second.x + first.y * second.y
                + first.z * second.z + first.w * second.w;
}

tt_vec4_t tt_vec4_normalise(tt_vec4_t vec)
{
        float length = tt_vec4_length(vec);
        return (tt_vec4_t) {
                vec.x / length,
                vec.y / length,
                vec.z / length,
                vec.w / length
        };
}

int tt_matrix_multiply(float l[16], float r[16])
{
        float final[16];
        for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                        float sum = 0.0;
                        for (int k = 0; k < 4; k++)
                                sum += l[j * 4 + k] * r[i * 4 + k];
                        final[j * 4 + i] = sum;
                }
        }

        tt_matrix_copy(l, final);
        return 0;
}

void tt_matrix_copy(float to[16], float from[16])
{
        for (int i = 0; i < 16; i++)
                to[i] = from[i];
}

int tt_matrix_identity(float m[16])
{
        for (int i = 0; i < 16; i++)
                m[i] = 0;
        m[0]  = 1;
        m[5]  = 1;
        m[10] = 1;
        m[15] = 1;
        return 0;
}

int tt_matrix_translate3f(float m[16], float dx, float dy, float dz)
{
        float other[16] = {
                1.0, 0.0, 0.0, dx,
                0.0, 1.0, 0.0, dy,
                0.0, 0.0, 1.0, dz,
                0.0, 0.0, 0.0, 1.0,
        };

        return tt_matrix_multiply(m, other);
}

int tt_matrix_translate3fv(float m[16], tt_vec3_t vec)
{
        return tt_matrix_translate3f(m, vec.x, vec.y, vec.z);
}

int tt_matrix_scale3f(float m[16], float w, float h, float d)
{
        float other[16] = {
                w, 0, 0, 0,
                0, h, 0, 0,
                0, 0, d, 0,
                0, 0, 0, 1,
        };

        return tt_matrix_multiply(m, other);
}

int tt_matrix_scale3fv(float m[16], tt_vec3_t vec)
{
        return tt_matrix_scale3f(m, vec.x, vec.y, vec.z);
}

int tt_matrix_rotate3f(float m[16], float x, float y, float z, float theta)
{
        float other[16] = {
                cos(theta) + x*x*(1-cos(theta)),   x*y*(1-cos(theta)) - z*sin(theta), x*z*(1-cos(theta)) + y*sin(theta), 0.0,
                y*x*(1-cos(theta)) + z*sin(theta), cos(theta) + y*y*(1-cos(theta)),   y*z*(1-cos(theta)) - x*sin(theta), 0.0,
                z*x*(1-cos(theta)) - y*sin(theta), y*z*(1-cos(theta)) + x*sin(theta), cos(theta) + x*x*(1-cos(theta)),   0.0,
                0.0,                               0.0,                               0.0,                               1.0,
        };

        return tt_matrix_multiply(m, other);
}

int tt_matrix_rotate3fv(float m[16], tt_vec3_t vec, float theta)
{
        return tt_matrix_rotate3f(m, vec.x, vec.y, vec.z, theta);
}

int tt_matrix_orthographic(float m[16], float t, float r, float b, float l, float n, float f)
{
        float other[16] = {
                2.0/(r-l), 0.0,       0.0,        -(r+l)/(r-l),
                0.0,       2.0/(t-b), 0.0,        -(t+b)/(t-b),
                0.0,       0.0,       -2.0/(f-n), -(f+n)/(f-n),
                0.0,       0.0,       0.0,        1.0
        };

        tt_matrix_identity(m);
        return tt_matrix_multiply(m, other);
}
