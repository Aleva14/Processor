#include <stdio.h>
#include <stdlib.h>
#include "Stack/s_stack.h"
#include <stdint.h>
#include <string.h>

#define REG_NUM 20
#define PROG_MEM_SIZE 65536 // 2 ^ 16

typedef uint8_t byte;
typedef uint16_t reg;

enum Register{
	Gly,Ala,Val,Leu,Ser,Thr,Asp,Asn,Glu,Gln,Lys,Arg,Cys,Met,Fen,Tyr,Trp,Hys,Pro,Npc
};

enum Command {push_num, push_reg, pop, in, out, tr,triz, trip, trin, add, mul, sub, divide};

char Reg[20][4] = {"Gly\0", "Ala\0", "Val\0", "Leu\0", "Ser\0", "Thr\0", "Asp\0", "Asn\0", "Glu\0", "Gln\0", "Lys\0", "Arg\0", "Cys\0", "Met\0", "Fen\0", "Tyr\0", "Trp\0", "Hys\0", "Pro\0","Npc\0"};

enum P_errors{ 
        INV_PROC = 1,
	NO_PROG,
	EOF_FMEM
};

int p_errno = 0;

typedef struct cpu{
	S_stack mem;
	byte firmware[PROG_MEM_SIZE];
	reg r[REG_NUM];
	int prog_len;
} Cpu;

int cpu_init(Cpu *cpu){
	if (s_stack_init(&cpu->mem)){
		p_errno = INV_PROC;
		return 1;	
	}
	memset(cpu->firmware, 0, PROG_MEM_SIZE);
	memset(cpu->r, 0, REG_NUM * 2);
	cpu->prog_len = 0;
	return 0;
}

int cpu_flash(int fd, Cpu *cpu){
	int t = 0;
	int p_len = lseek(fd, 0, SEEK_END);
	if (p_len == -1){
		p_errno = NO_PROG;
		return 1;
	}
	lseek(fd, 0, SEEK_SET);
	if ((t = read(fd, cpu->firmware, PROG_MEM_SIZE)) == -1){
		p_errno = NO_PROG;
		return 1;
	}
	else if (t < p_len){
		p_errno = EOF_FMEM;
	}
	cpu->prog_len = t;
	return 0;		
}

int cpu_start(Cpu *cpu){
	int i = 0;
	reg *cur_c;
	while (i < cpu->prog_len){
		cur_c = &(cpu->firmware[i]);
		switch (*cur_c){
			case push_num:
				s_stack_push(&cpu->mem, *(cur_c + 1));
				i += 2;
			default:
				i++;
		}
	}
	return 0;
}

int cpu_dump(Cpu *cpu){
	printf("\n\nCPU dump\n");
	int i = 0;
	printf("Registers\n");
	for (i = 0; i < REG_NUM; i++){
		printf("%-2d %s %d\n", i, Reg[i], cpu->r[i]);
	}
	printf("Firmware (bytes)\n");
	for (i = 0; i <	cpu->prog_len; i++){
		printf("%02x ", cpu->firmware[i]);
	}
	printf("\nFirmware (commands)\n");
	for(i = 0; i < cpu->prog_len; i += 2){
		reg *tmp;
		tmp = &(cpu->firmware[i]);
		printf("%04x ", *tmp);
	}
	printf("\nStack\n");
	s_stack_dump(&cpu->mem);	
}

int main(){
	int fd = open("./test.bin");
	Cpu cpu;
	cpu_init(&cpu);
	cpu_flash(fd, &cpu);
	printf("Errno %d", p_errno);
	cpu_start(&cpu);
	cpu_dump(&cpu);
	return 0;	 
}
