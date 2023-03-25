#include "../include/tt.h"

GLfloat tt_colour_mask(GLuint colour, TT_COLOUR_MASK mask, GLboolean normalized)
{
        GLfloat col = 0.0;
        switch (mask) {
        case TT_COLOUR_R:
                col = (colour & 0xff000000) >> 24;
                return (normalized ? col / 255.0 : col);
        case TT_COLOUR_G:
                col = (colour & 0x00ff0000) >> 16;
                return (normalized ? col / 255.0 : col);
        case TT_COLOUR_B:
                col = (colour & 0x0000ff00) >> 8;
                return (normalized ? col / 255.0 : col);
        case TT_COLOUR_A:
        default:
                col = (colour & 0x000000ff) >> 0;
                return (normalized ? col / 255.0 : col);
        }
}
