main: main.c Processor/libprocessor.a Stack/libstack.a
	gcc main.c -o main -L./Processor -lprocessor -L./Stack -lstack
Processor/libprocessor.a: Processor/processor.o
	ar crs Processor/libprocessor.a Processor/processor.o
Processor/processor.o: Stack/libstack.a Processor/processor.c
	gcc -c Processor/processor.c -o Processor/processor.o -L./Stack -lstack
Stack/libstack.a: Stack/s_stack.o 
	ar crs Stack/libstack.a Stack/s_stack.o
Stack/s_stack.o: Stack/s_stack.c
	gcc -c Stack/s_stack.c
