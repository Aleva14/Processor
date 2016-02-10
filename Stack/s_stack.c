#include <stdio.h>
#include <stdlib.h>

#define SIZE 1024
#define S_POP_ERR_RETVAL -42

enum S_errors{
        FULL = 1,
        EMPTY
};
int s_errno = 0;

typedef int Type;
typedef struct stack{
	Type elem[SIZE];
	int head;
} S_stack;

void s_stack_init(S_stack *stack){
	stack->head = 0;
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

void s_stack_dump(S_stack *stack){
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
}

