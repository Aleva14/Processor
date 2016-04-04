#include <stdio.h>
#include <stdlib.h>
#include "Stack/s_stack.h"
#include <stdint.h>
#include <string.h>

#define REG_NUM 20
#define PROG_MEM_SIZE 65536 // 2 ^ 16
#define NPC_INC(x) cpu->r[Npc] += (x) 
#define NPC cpu->r[Npc]

typedef uint16_t reg;
typedef int16_t sign_reg;

enum Register{
	Gly, Ala, Val, Leu, Ser, Thr, Asp, Asn, Glu, Gln, Lys, Arg, Cys, Met, Fen, Tyr, Trp, Hys, Pro, Npc};

enum Command {push_num, push_reg, pop, in, out, tr, triz, trip, trin, add, mul, sub, divide, gsp, asp, rsp};

char Reg[20][4] = {"Gly\0", "Ala\0", "Val\0", "Leu\0", "Ser\0", "Thr\0", "Asp\0", "Asn\0", "Glu\0", "Gln\0", "Lys\0", "Arg\0", "Cys\0", "Met\0", "Fen\0", "Tyr\0", "Trp\0", "Hys\0", "Pro\0","Npc\0"};

enum P_errors{ 
        INV_PROC = 1,
	NO_PROG,
	EOF_FMEM,
	DIV_BY_0,
	EMPTY_STACK
	NO_STACK
};

int p_errno = 0;

typedef struct cpu{
	S_stack mem;
	reg firmware[PROG_MEM_SIZE];
	reg r[REG_NUM];
	int prog_len;
} Cpu;

int cpu_dump(Cpu *cpu);

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
	cpu->prog_len = t / 2;
	return 0;		
}

static inline cpu_push_num(Cpu *cpu, reg num){
	printf("push_num %d\n", num);
	s_stack_push(&cpu->mem, num);
}

static inline cpu_pop(Cpu *cpu, int reg_num){
	reg tmp = s_stack_pop(&(cpu->mem));
	printf("pop num %d\n", tmp);
	if (s_errno == EMPTY){
		p_errno = EMPTY_STACK;
	}
	else {
		cpu->r[reg_num] = tmp;
	}
}

static inline cpu_push_reg(Cpu *cpu, int reg_num){
	s_stack_push(&cpu->mem, cpu->r[reg_num]);	
}

static inline cpu_add(Cpu *cpu){
	int a = s_stack_pop(&cpu->mem);
	int b = s_stack_pop(&cpu->mem);
	if (s_errno == EMPTY){
                p_errno = EMPTY_STACK;
        }
        else {
                s_stack_push(&cpu->mem, a + b);
	}
}

static inline cpu_sub(Cpu *cpu){
        int a = s_stack_pop(&cpu->mem);
        int b = s_stack_pop(&cpu->mem);
        if (s_errno == EMPTY){
                p_errno = EMPTY_STACK;
        }
        else {
                s_stack_push(&cpu->mem, b - a);
	}
}

static inline cpu_mul(Cpu *cpu){
        int a = s_stack_pop(&cpu->mem);
        int b = s_stack_pop(&cpu->mem);
        if (s_errno == EMPTY){
                p_errno = EMPTY_STACK;
        }
        else {
                s_stack_push(&cpu->mem, a * b);
	}
}

static inline cpu_div(Cpu *cpu){
        int a = s_stack_pop(&cpu->mem);
        int b = s_stack_pop(&cpu->mem);
        if (s_errno == EMPTY){
                p_errno = EMPTY_STACK;
        }
        else if (a != 0){
		s_stack_push(&cpu->mem, b / a);
	}
	else {
		p_errno = DIV_BY_0;		
	}
}

static inline cpu_trip(Cpu *cpu, int destination){
	sign_reg a = s_stack_pop(&cpu->mem);
	if (s_errno == EMPTY){
                p_errno = EMPTY_STACK;
        }
	else if (a > 0){
		NPC = destination;
	}
	else {
		NPC_INC(2);
	}
	
}

static inline cpu_trin(Cpu *cpu, int destination){
        sign_reg a = s_stack_pop(&cpu->mem);
        printf("Gly == %d", a);
	if (s_errno == EMPTY){
                p_errno = EMPTY_STACK;
        }
        else if (a < 0){
                NPC = destination;
        }
	else {
		NPC_INC(2);
	}

}

static inline cpu_triz(Cpu *cpu, int destination){
        sign_reg a = s_stack_pop(&cpu->mem);
        if (s_errno == EMPTY){
                p_errno = EMPTY_STACK;
        }
        else if (a == 0){
                NPC = destination;
        }
	else {
		NPC_INC(2);
	}
}

int cpu_start(Cpu *cpu){
	while (NPC < cpu->prog_len){
		switch (cpu->firmware[NPC]){
			case push_num:
				cpu_push_num(cpu, cpu->firmware[NPC + 1]);
				NPC_INC(2);
				break;
			case pop:
				cpu_pop(cpu, cpu->firmware[NPC + 1]);
                            	if (p_errno){
					break;
				}
				NPC_INC(2);
				break;
			case push_reg:
				cpu_push_reg(cpu, cpu->firmware[NPC + 1]);
				NPC_INC(2);
				break;
			case add:
				cpu_add(cpu);
				if (p_errno){
                                        break;
                                }
				NPC_INC(1);
				break;
			case mul:
                                cpu_mul(cpu);
                                if (p_errno){
                                        break;
                                }
				NPC_INC(1);
                                break;
			case sub:
                                cpu_sub(cpu);
                                if (p_errno){
                                        break;
                                }
				NPC_INC(1);
                                break;
			case divide:
                                cpu_div(cpu);
                                if (p_errno){
                                        break;
                                }
				NPC_INC(1);
                                break;
			case tr:
				cpu->r[Npc] = cpu->firmware[NPC + 1];
			case trip:
				cpu_trip(cpu, cpu->firmware[NPC + 1]);
				break;
			case trin:
                                cpu_trin(cpu, cpu->firmware[NPC + 1]);
                                break;
			case triz:
                                cpu_triz(cpu, cpu->firmware[NPC + 1]);
                                break;
			case gsp:
				int sp = s_stack_peek(&cpu->mem);
				if (sp == -1){
					p_errno = NO_STACK;
					break;
				}
				else {
					s_stack_push(&cpu->mem, sp);
				}
			default:
				NPC_INC(1);

		}
	if (p_errno){
		printf("Error at %d", NPC);
		return 1;
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
		int tmp = cpu->r[i];
	}
	printf("\nFirmware\n");
	for(i = 0; i < cpu->prog_len; i += 1){
		printf("%04x ", cpu->firmware[i]);
	}
	printf("\nStack\n");
	s_stack_dump(&cpu->mem);	
}

int main(){
	int fd = open("./test.bin");
	Cpu cpu;
	cpu_init(&cpu);
	cpu_flash(fd, &cpu);
	cpu_start(&cpu);
	cpu_dump(&cpu);
	return 0;	 
}
