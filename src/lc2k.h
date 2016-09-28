#ifndef _LC2K_H
#define _LC2K_H

#include <stdint.h>

#define LC2K_REG_NUMBER 8

namespace lc2k {
	class cpu {
		uint32_t *ram; /* The main RAM of the system */
		uint32_t reg_array[LC2K_REG_NUMBER];
		uint32_t pc;
		uint32_t mem_size;

		uint32_t read_mem(uint32_t address);
		void write_mem(uint32_t address, uint32_t value);

		bool exe_single_ins(uint32_t instruction);
	public:
		cpu(uint32_t *r, uint32_t m) : ram(r), mem_size(m) {}

		void start_execute(uint32_t start_pos);
		void start_execute() {
			start_execute(0);
		}
		void continue_execute() {
			start_execute(pc);
		}

		static const int opcode_add = 0;
		static const int opcode_nand = 1;
		static const int opcode_lw = 2;
		static const int opcode_sw = 3;
		static const int opcode_beq = 4;
		static const int opcode_jalr = 5;
		static const int opcode_halt = 6;
		static const int opcode_noop = 7;
	};
};

#endif
