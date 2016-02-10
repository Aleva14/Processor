#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 65536 // 2^16
#define D_POP_ERR_RETVAL -42

enum D_errors{
	NO_MEM = 1,
	WRONG_SIZE,
	FULL,
	EMPTY
};
int d_errno = 0;

typedef int Type;
typedef struct stack{
        int size;
        Type *elem;
        Type *head;
} D_stack;

D_stack *d_stack_create(int size){
	if ((size == 0) || (size > MAX_SIZE)) {
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
	stack->head = stack->elem;
	return stack;
}

int d_stack_realloc(D_stack *stack){
	if (stack->size * 2 > MAX_SIZE){
		d_errno = WRONG_SIZE;
		return 1;
	}
	stack->elem = (Type *)realloc(stack->elem, stack->size * 2 * sizeof(Type));
	if (stack->elem == NULL){
		d_errno = NO_MEM;
		return 1;
	}
	stack->size *= 2;
	return 0;
}

void d_stack_delete(D_stack *stack){
        free(stack->elem);
	free(stack);
}

int d_stack_is_full(D_stack *stack){
	if (stack->head - stack->elem < stack->size)
		return 0;
	else
		return 1;	
}

int d_stack_is_empty(D_stack *stack){
	if (stack->head == stack->elem)
		return 1;
	else
		return 0;
}


int d_stack_push(D_stack *stack, Type data){
        if (d_stack_is_full(stack)){
		d_errno = FULL;
		return 1;
	}
	*stack->head = data;
        stack->head++;
        return 0;
}

Type d_stack_pop(D_stack *stack){
        if (d_stack_is_empty(stack)){
		d_errno = EMPTY;
		return D_POP_ERR_RETVAL;
	}
	stack->head--;
        return *(stack->head + 1);
}


int d_stack_dump(D_stack *stack){
        Type *tmp = stack->elem;
        while (tmp != stack->head){
                printf("%d ", *tmp);
                tmp++;
        }
	if (d_stack_is_empty(stack))
		printf("\nStack is empty\n");
	else if (d_stack_is_full(stack))
		printf("\nStack is full\n");
	else 
		printf("\n");
}
