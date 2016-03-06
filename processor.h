#include <stdio.h>
#include <stdlib.h>
#include "Stack/s_stack.h"
#include <stdint.h>
#include <string.h>

#define REG_NUM 20
#define PROG_MEM_SIZE 65536 // 2 ^ 16

typedef uint8_t byte;
typedef uint16_t reg;

enum register{
	Gly,Ala,Val,Leu,Ser,Thr,Asp,Asn,Glu,Gln,Lys,Arg,Cys,Met,Fen,Tyr,Trp,Hys,Pro,Npc
};

typedef struct cpu{
	S_stack mem;
	byte firmware[PROG_MEM_SIZE];
	reg r[REG_NUM];
} Cpu;

int cpu_init(Cpu *cpu);

int cpu_flash(int fd, Cpu *cpu);

int cpu_start(Cpu *cpu);

int cpu_dump(Cpu *cpu);
