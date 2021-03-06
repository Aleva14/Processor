#include "processor.h"

static const char Reg[28][4] = {"Gly\0", "Ala\0", "Val\0", "Leu\0", "Ser\0", "Thr\0", "Asp\0", "Asn\0", "Glu\0", "Gln\0", "Lys\0", "Arg\0", "Cys\0", "Met\0", "Fen\0", "Tyr\0", "Trp\0", "Hys\0", "Pro\0", "Npc\0", "V_0\0", "V_1\0", "V_2\0", "V_3\0", "V_4\0", "V_5\0", "V_6\0", "V_7\0"};

int p_errno = 0;

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
	if (s_stack_push(&cpu->mem, num)){
		p_errno = STACK;	
	}
}

static inline cpu_pop(Cpu *cpu, int reg_num){
	reg tmp = s_stack_pop(&(cpu->mem));
	if (s_errno == EMPTY){
		p_errno = STACK;
	}
	else {
		cpu->r[reg_num] = tmp;
	}
}

static inline cpu_push_reg(Cpu *cpu, int reg_num){
	if (s_stack_push(&cpu->mem, cpu->r[reg_num])){
		p_errno = STACK;
	}	
}

static inline cpu_add(Cpu *cpu){
	sign_reg a = s_stack_pop(&cpu->mem);
	sign_reg b = s_stack_pop(&cpu->mem);
	if (s_errno == EMPTY){
                p_errno = STACK;
        }
        else {
                s_stack_push(&cpu->mem, a + b);
	}
}

static inline cpu_sub(Cpu *cpu){
        sign_reg a = s_stack_pop(&cpu->mem);
        sign_reg b = s_stack_pop(&cpu->mem);
        if (s_errno == EMPTY){
                p_errno = STACK;
        }
        else {
                s_stack_push(&cpu->mem, b - a);
	}
}

static inline cpu_mul(Cpu *cpu){
        int a = s_stack_pop(&cpu->mem);
        int b = s_stack_pop(&cpu->mem);
	int result = a * b;
	sign_reg higher_order = result >> 16;
	sign_reg lower_order = result & 0xffff;
	printf("Result %d, higher %d, lower %d\n", result, higher_order, lower_order);
        if (s_errno == EMPTY){
                p_errno = STACK;
        }
        else {
                s_stack_push(&cpu->mem, higher_order);
		s_stack_push(&cpu->mem, lower_order);
	}
 }

static inline cpu_div(Cpu *cpu){
        sign_reg a = s_stack_pop(&cpu->mem);
        sign_reg b = s_stack_pop(&cpu->mem);
        if (s_errno == EMPTY){
                p_errno = STACK;
        }
        else if (a != 0){
		sign_reg quotient = b / a;
		sign_reg remainder = b % a;
		s_stack_push(&cpu->mem, remainder);
		s_stack_push(&cpu->mem, quotient);
	}
	else {
		p_errno = DIV_BY_0;		
	}
}

static inline cpu_tr(Cpu *cpu, int destination){
	NPC = cpu->firmware[NPC + 1];
}

static inline cpu_trip(Cpu *cpu, int destination){
	sign_reg a = s_stack_pop(&cpu->mem);
	if (s_errno == EMPTY){
                p_errno = STACK;
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
	if (s_errno == EMPTY){
                p_errno = STACK;
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
                p_errno = STACK;
        }
        else if (a == 0){
                NPC = destination;
        }
	else {
		NPC_INC(2);
	}
}

static inline cpu_out(Cpu *cpu){
	sign_reg tmp = s_stack_pop(&cpu->mem);
	printf("\n%d", tmp);
}

static inline cpu_gsp(Cpu *cpu, int reg){
	cpu->r[reg] = s_stack_pointer(&cpu->mem);
}

static inline cpu_ssp(Cpu *cpu, int reg){
	if (s_stack_set_pointer(&cpu->mem, cpu->r[reg])){
		p_errno = STACK;		
	}
}

static inline cpu_read(Cpu *cpu, reg fd, reg size){
	reg stack_pointer = s_stack_pointer(&cpu->mem);
	if (stack_pointer + size > SIZE){
		size = SIZE - stack_pointer;
	}
	if (read(fd, &cpu->mem + stack_pointer, size) == -1){
		p_errno = INV_SYSCALL;		
	}
	s_stack_set_pointer(&cpu->mem, stack_pointer + size); 
}

static inline cpu_write(Cpu *cpu, reg fd, reg size){
	reg stack_pointer = s_stack_pointer(&cpu->mem);
	if (stack_pointer + size > SIZE){
                size = SIZE - stack_pointer;
        }
	if (write(fd, &cpu->mem + stack_pointer, size) == -1){
		p_errno = INV_SYSCALL;	
	}
	s_stack_set_pointer(&cpu->mem, stack_pointer + size);
} 

static inline cpu_exit(Cpu *cpu, int *ret_val){
	cpu->run = 0;
	*ret_val = cpu->r[Glu];
}

static inline cpu_syscall(Cpu *cpu, int *ret_val){
	printf("syscall");
	reg call_type = cpu->r[Tyr];
	reg arg_1 = cpu->r[Glu];
	reg arg_2 = cpu->r[Lys];
	switch (call_type){
		case sys_read:
			cpu_read(cpu, arg_1, arg_2);
			break;
		case sys_write:
			cpu_write(cpu, arg_1, arg_2);
			break;
		case sys_exit:
			cpu_exit(cpu, ret_val);
			break;
		default:
			p_errno = INV_SYSCALL;
			break;
	}
		
}


int cpu_start(Cpu *cpu){
	cpu->run = 1;
	int ret_val = 0;
	while ((NPC < cpu->prog_len) && cpu->run){
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
				cpu_tr(cpu, cpu->firmware[NPC + 1]);
				break;
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
				cpu_gsp(cpu, cpu->firmware[NPC + 1]);
				NPC_INC(2);
				break;
			case ssp:
				cpu_ssp(cpu, cpu->firmware[NPC + 1]);
				NPC_INC(2);
				break;
			case syscall:
				cpu_syscall(cpu, &ret_val);
				NPC_INC(1);
				break;
			case out:
				cpu_out(cpu);
				NPC_INC(1);
				break; 
			default:
				NPC_INC(1);

		}
	if (p_errno){
		printf("\nError at %d", NPC);
		return 1;
	}
	}
	cpu->run = 0;
	return ret_val;
}


int cpu_dump(Cpu *cpu){
	printf("\n\nCPU dump\n");
	int i = 0;
	printf("p_error = %d\n", p_errno);
	printf("s_errno = %d\n", s_errno);
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

/*int main(){
        int fd = open("./test.bin");
        Cpu cpu;
        cpu_init(&cpu);
        cpu_flash(fd, &cpu);
        cpu_start(&cpu);
        cpu_dump(&cpu);
        return 0;
}*/

