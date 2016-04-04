#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SIZE 1024
#define S_POP_ERR_RETVAL -42

enum S_errors{
        FULL = 1,
        EMPTY,
	NO_STACK
};
extern int s_errno;

typedef uint16_t Type;
typedef struct stack{
	Type elem[SIZE];
	int head;
} S_stack;

extern int s_stack_init(S_stack *stack);

extern Type s_stack_pop(S_stack *stack);

extern int s_stack_push(S_stack *stack, Type data);

extern int s_stack_dump(S_stack *stack);

extern Type s_stack_peek(S_stack *stack);
