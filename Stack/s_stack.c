#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SIZE 1024
#define S_POP_ERR_RETVAL -42

enum S_errors{
        FULL = 1,
        EMPTY,
	NO_STACK,
	INVAL
};
int s_errno = 0;

typedef uint16_t Type;
typedef struct stack{
	Type elem[SIZE];
	int head;
} S_stack;

int s_stack_init(S_stack *stack){
	if (stack == NULL){
		s_errno = NO_STACK;
		return 1;
	}
	stack->head = 0;
	return 0;
}

int s_stack_is_empty(S_stack *stack){
	if (stack->head == 0)
		return 1;
	else
		return 0;
}

int s_stack_is_full(S_stack *stack){
	if (stack->head == SIZE)
		return 1;
	else
		return 0;
}

Type s_stack_pop(S_stack *stack){
	if (stack == NULL){
                s_errno = NO_STACK;
                return S_POP_ERR_RETVAL;
        }
	Type tmp;
	if (s_stack_is_empty(stack)){
		s_errno = EMPTY;
		return S_POP_ERR_RETVAL;
	}
	tmp = stack->elem[stack->head - 1];
	stack->head--;
	return tmp;
}

int s_stack_push(S_stack *stack, Type data){
	if (stack == NULL){
                s_errno = NO_STACK;
                return 1;
        }
	if (s_stack_is_full(stack)){
		s_errno = FULL;
		return 1;
	}
	else {
		stack->elem[stack->head] = data;
		stack->head++;
	}
	return 0;
}

int s_stack_dump(S_stack *stack){
        if (stack == NULL){
                s_errno = NO_STACK;
                return 1;
        }
	int i = 0;
        for (i = 0; i < stack->head; i++){
                printf("%d ", stack->elem[i]); 
        } 
	if (s_stack_is_empty(stack))
                printf("\nStack is empty\n");
        else if (s_stack_is_full(stack))
                printf("\nStack is full\n");
        else
                printf("\n");
	return 0;
}

int s_stack_pointer(S_stack *stack){
	if (stack == NULL){
		s_errno = NO_STACK;
		return -1;
	}
	else {
		return stack->head;
	}
}

Type s_stack_peek(S_stack *stack){
	if (stack == NULL){
		s_errno = NO_STACK;
		return S_POP_ERR_RETVAL;
	}
	else if (s_stack_is_empty(stack)){
                s_errno = EMPTY;
                return S_POP_ERR_RETVAL;
        } 
	else {
		return stack->elem[stack->head - 1];
	}
}

int s_stack_set(S_stack *stack, Type new_head){
	if (stack == NULL){
		s_errno = NO_STACK;
		return 1;
	}
	else if	(new_head > SIZE){
		s_errno = INVAL;
		return 1;
	}
	else {
		stack->head = new_head;
	}
}

