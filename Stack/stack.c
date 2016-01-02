#include <stdio.h>
#include <stdlib.h>

#define SIZE 1024 
#define FULL 0
#define EMPTY 0

typedef int Type;

typedef struct stack{
	Type elem[SIZE + 1];
	int head;
} Stack;

void stack_init(Stack *stack){
	stack->head = 0;
}

void stack_type(Stack *stack){
	int i = 0;
	printf("Stack: ");
	for (i = 0; i < stack->head; i++){
		printf("%d ", stack->elem[i]); 
	}	
	printf("\n");
}

int stack_is_empty(Stack *stack){
	if (stack->head == 0){
		return 1;
	}
	return 0;
}

int stack_is_full(Stack *stack){
	if (stack->head == SIZE) {
		return 1;
	}
	return 0;
}

Type stack_pop(Stack *stack){
	Type tmp;
	if (stack_is_empty(stack)){
		fprintf(stderr, "Stack is empty\n");
		return EMPTY;
	}
	tmp = stack->elem[stack->head - 1];
	stack->head--;
	return tmp;
}

int stack_push(Stack *stack, Type data){
	if (stack_is_full(stack)){
		fprintf(stderr, "Stack is full\n");
		return FULL;
	}
	else {
		stack->elem[stack->head] = data;
		stack->head++;
	}
	return 0;
}

int main(){
	return 0;
}
