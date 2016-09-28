#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <endian.h>

#include "lc2k.h"

#define MEM_SIZE_DEFAULT 1024

uint32_t *allocate_mem(int mem_size)
{
	uint32_t *mem = new uint32_t[mem_size];
	for (int i = 0; i<mem_size; i++)
		mem[i] = 0;
	return mem;
}

void deallocate_mem(uint32_t *mem)
{
	delete[] mem;
}

bool read_prog(uint32_t *mem, FILE *fp, int mem_size)
{
	size_t s = fread(mem, sizeof(uint32_t), mem_size - 1, fp);
	for (size_t i = 0; i < s; i++)
		mem[i] = be32toh(mem[i]);
	mem[s] = 0x01800000; /* halt */
}

void usage(const char *arg0)
{
	fprintf(stderr, "Usage: %s [-m size] [-i count] name\n"
			"The memory size is in 4bytes and the default is 1024*4 bytes\n"
			"If -i is specified, the program will stop after the number instructions executed\n",
		arg0);
}

int main(int argc, char **argv)
{
	int mem_size = MEM_SIZE_DEFAULT, opt;
	int ins_count = 0;
	while ((opt = getopt(argc, argv, "m:i:")) != -1) {
		switch (opt) {
			case 'm':
				mem_size = atoi(optarg);
				break;
			case 'i':
				ins_count = atoi(optarg);
				break;
			default:
				usage(argv[0]);
				return 1;
		}
	}

	if (optind >= argc) {
		fprintf(stderr, "No input file name specified!\n");
		usage(argv[0]);
		return 1;
	}

	uint32_t *mem = allocate_mem(mem_size);
	lc2k::cpu *lc2k_core = new lc2k::cpu(mem, (uint32_t) mem_size);

	FILE *fp = fopen(argv[optind], "rb");
	if (!fp) {
		fprintf(stderr, "Cannot read the input file!");
		return 1;
	}

	read_prog(mem, fp, mem_size);

	lc2k_core->start_execute(0, ins_count);

	lc2k_core->dump_core_hr(stdout);

	deallocate_mem(mem);

	return 0;
}
