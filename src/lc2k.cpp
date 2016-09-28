#include "lc2k.h"

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
				pc = pc + 1 + inter;
			return true;
		case opcode_jalr:
			reg_array[reg1] = pc + 1;
			pc = reg_array[reg0];
			return true;
		case opcode_noop:
			return true;
		case opcode_halt:
			return false;
	}
}

void cpu::start_execute(uint32_t start_pos)
{
	pc = start_pos;
	while(exe_single_ins(read_mem(pc)))
		pc++;
	pc++;
}
