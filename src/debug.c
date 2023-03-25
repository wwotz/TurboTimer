#include "../include/tt.h"

static char *tt_debug_stack[TT_DEBUG_STACK_CAPACITY] = {0};
static size_t tt_debug_stack_ptr = 0;
static size_t tt_debug_stack_size = 0;

static int tt_debug_stack_empty(void)
{
        return tt_debug_stack_size == 0;
}

static int tt_debug_stack_full(void)
{
        return tt_debug_stack_size == TT_DEBUG_STACK_CAPACITY;
}

int tt_debug_stack_push(const char *msg, int line,
                                const char *file)
{
        if (!tt_debug_stack_full())
                tt_debug_stack_size++;

        if (tt_debug_stack[tt_debug_stack_ptr]) {
                free(tt_debug_stack[tt_debug_stack_ptr]);
                tt_debug_stack[tt_debug_stack_ptr] = NULL;
        }

        tt_debug_stack[tt_debug_stack_ptr] = malloc(TT_DEBUG_MSG_LENGTH);
        if (!tt_debug_stack[tt_debug_stack_ptr]) {
                fprintf(stderr, "Fatal: no memory for debugging, exiting...\n");
                exit(EXIT_FAILURE);
        }

        snprintf(tt_debug_stack[tt_debug_stack_ptr], TT_DEBUG_MSG_LENGTH,
                   "Error @ Line %d in %s: %s", line, file, msg);
        tt_debug_stack_ptr = (tt_debug_stack_ptr + 1) % TT_DEBUG_STACK_CAPACITY;
}

char *tt_debug_stack_pop(void)
{
        if (tt_debug_stack_empty())
                return "No Error";

        tt_debug_stack_ptr--;
        tt_debug_stack_size--;
        if (tt_debug_stack_ptr < 0)
                tt_debug_stack_ptr += TT_DEBUG_STACK_CAPACITY;
        return tt_debug_stack[tt_debug_stack_ptr];
}

int tt_debug_had_error(void)
{
        return !tt_debug_stack_empty();
}
