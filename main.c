#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "Processor/processor.h"

int k = 0;

int main(){
        int fd = open("./test.bin");
        Cpu cpu;
	printf("Cpu created");
        cpu_init(&cpu);
        cpu_flash(fd, &cpu);
        cpu_start(&cpu);
        cpu_dump(&cpu);
        return 0;
}

