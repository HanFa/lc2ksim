#include "lc2k.h"

#include <cstdio>

using namespace lc2k;

uint32_t cpu::read_mem(uint32_t address)
{
	if (address < mem_size) return ram[address];
	else return 0; /* Outside the RAM, TODO: MMIO*/
}

void cpu::write_mem(uint32_t address, uint32_t value)
{
	if (address < mem_size) ram[address] = value;
	else return; /* Outside the RAM, TODO: MMIO */
}

bool cpu::exe_single_ins(uint32_t instruction)
{
	uint32_t opcode = (instruction >> 22) & 0x07;
	uint32_t reg0 = (instruction >> 19) & 0x07;
	uint32_t reg1 = (instruction >> 16) & 0x07;
	uint32_t reg2 = instruction & 0x07;
	uint32_t inter = (uint32_t)((int16_t)(instruction & 0xFFFF));
	switch (opcode)
	{
		case opcode_add:
			reg_array[reg2] = reg_array[reg0] + reg_array[reg1];
			return true;
		case opcode_nand:
			reg_array[reg2] = ~(reg_array[reg0] & reg_array[reg1]);
			return true;
		case opcode_lw:
			reg_array[reg1] = read_mem(reg_array[reg0] + inter);
			return true;
		case opcode_sw:
			write_mem(reg_array[reg0] + inter, reg_array[reg1]);
			return true;
		case opcode_beq:
			if (reg_array[reg0] == reg_array[reg1])
				pc = pc + inter;
			return true;
		case opcode_jalr:
			reg_array[reg1] = pc + 1;
			pc = reg_array[reg0] - 1;
			return true;
		case opcode_noop:
			return true;
		case opcode_halt:
			return false;
	}
}

void cpu::start_execute(uint32_t start_pos, uint32_t count)
{
	ins_exe_count = count;
	pc = start_pos;
	while(exe_single_ins(read_mem(pc)) && (!count || --ins_exe_count)) {
		pc++;
	}
	pc++;
}

void cpu::dump_core_hr(FILE *fp)
{
	std::fprintf(fp, "PC: %08x", pc);
	for (int i = 0; i<LC2K_REG_NUMBER; i++) {
		fprintf(fp, " R%d: %08x", i, reg_array[i]);
	}
	fprintf(fp, "\n");
	for (uint32_t i = 0; i<mem_size; i++) {
		fprintf(fp, "%08x: %08x\n", i, read_mem(i));
	}
}
