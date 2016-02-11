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
int d_errno = 0;

typedef int Type;
typedef struct stack{
        int size;
        Type *elem;
        int head;
} D_stack;

D_stack *d_stack_create(int size){
	if ((size <= 0) || (size > MAX_SIZE)) {
		d_errno = WRONG_SIZE;
		return NULL;
	}
	D_stack *stack = (D_stack*)malloc(sizeof(D_stack));
	if (stack == NULL){
		d_errno = NO_MEM;
		return NULL;
	}
	stack->elem = (Type*)malloc(size * sizeof(Type));
	if (stack->elem == NULL){
		d_errno = NO_MEM;
		return NULL;
	}
	stack->size = size;
	stack->head = 0;
	return stack;
}

int d_stack_realloc(D_stack *stack, int size){
	if (stack == NULL){
		d_errno = NO_STACK;
		return 1;
	}
	if ((size > MAX_SIZE) || (size < stack->head)){
		d_errno = WRONG_SIZE;
		return 1;
	}
	stack->elem = (Type *)realloc(stack->elem, size * sizeof(Type));
	if (stack->elem == NULL){
		d_errno = NO_MEM;
		return 1;
	}
	stack->size = size;
	return 0;
}

int d_stack_delete(D_stack *stack){
        if (stack == NULL){
                d_errno = NO_STACK;
                return 1;
        }
	free(stack->elem);
	free(stack);
	return 0;
}

int d_stack_is_full(D_stack *stack){
	if (stack->head < stack->size)
		return 0;
	else
		return 1;	
}

int d_stack_is_empty(D_stack *stack){
	if (stack->head == 0)
		return 1;
	else
		return 0;
}

int d_stack_push(D_stack *stack, Type data){
	if (stack == NULL){
		d_errno = NO_STACK;
		return 1;
	}
        if (d_stack_is_full(stack)){
		d_errno = FULL;
		return 1;
	}
	*(stack->elem + stack->head) = data;
        stack->head++;
        return 0;
}

Type d_stack_pop(D_stack *stack){
	if (stack == NULL){
                d_errno = NO_STACK;
                return 1;
        }
        if (d_stack_is_empty(stack)){
		d_errno = EMPTY;
		return D_POP_ERR_RETVAL;
	}
	stack->head--;
        return *(stack->elem + stack->head + 1);
}

int d_stack_dump(D_stack *stack){
        if (stack == NULL){
		d_errno = NO_STACK;
                return 1;
        }
	int tmp = 0;
        while (tmp != stack->head){
                printf("%d ", *(stack->elem + tmp));
                tmp++;
        }
	if (d_stack_is_empty(stack))
		printf("\nStack is empty\n");
	else if (d_stack_is_full(stack))
		printf("\nStack is full\n");
	else 
		printf("\n");
	return 0;
}
