#include <stdio.h>
#include <stdlib.h>
#include "../Stack/s_stack.h"
#include <stdint.h>
#include <string.h>

#define REG_NUM 20
#define PROG_MEM_SIZE 65536 // 2 ^ 16
#define NPC_INC(x) cpu->r[Npc] += (x) 
#define NPC cpu->r[Npc]

typedef uint16_t reg;
typedef int16_t sign_reg;

enum Register{
        Gly, Ala, Val, Leu, Ser, Thr, Asp, Asn, Glu, Gln, Lys, Arg, Cys, Met, Fen, Tyr, Trp, Hys, Pro, Npc, v_0, v_1, v_2, v_3, v_4, v_5, v_6, v_7};

enum Command {push_num, push_reg, pop, in, out, tr, triz, trip, trin, add, mul, sub, divide, gsp, ssp, syscall};

enum Syscall {
	sys_read,
	sys_write,
	sys_exit
};

enum P_errors{
        INV_PROC = 1,
        NO_PROG,
        EOF_FMEM,
        DIV_BY_0,
        STACK,
	INV_SYSCALL
	
};

extern int p_errno;

typedef struct cpu{
        S_stack mem;
        reg firmware[PROG_MEM_SIZE];
        reg r[REG_NUM];
	int run;
        int prog_len;
} Cpu;

int cpu_init(Cpu *cpu);

int cpu_flash(int fd, Cpu *cpu);

int cpu_start(Cpu *cpu);

int cpu_dump(Cpu *cpu);
