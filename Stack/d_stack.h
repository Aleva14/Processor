#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 65536 // 2^16
#define D_POP_ERR_RETVAL -42

enum D_errors{
	NO_MEM = 1,
	NO_STACK,
	WRONG_SIZE,
	FULL,
	EMPTY
};
extern int d_errno = 0;

typedef int Type;
typedef struct stack{
        int size;
        Type *elem;
        int head;
} D_stack;

extern D_stack *d_stack_create(int size);

extern int d_stack_realloc(D_stack *stack, int size);

extern void d_stack_delete(D_stack *stack);

extern int d_stack_push(D_stack *stack, Type data);

extern Type d_stack_pop(D_stack *stack);

extern int d_stack_dump(D_stack *stack)
