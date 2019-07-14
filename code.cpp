#include <cstdio>

#include "tools.hpp"
#include "main.hpp"

void disp() {
	for (int i = 0; i < 32; i++)
		printf(" %d", reg[i]);
	printf("\n");
}

int main() {
	char c;
	do
		c = getchar();
	while (c != EOF && (c == ' ' || c == '\n'));

	while (c != EOF) {
		int pos;
		scanf("%x", &pos);

		int x;
		while (scanf("%x", &x) == 1)
			RAM[pos++] = x;

		do
			c = getchar();
		while (c != EOF && (c == ' ' || c == '\n'));
	}
	for (;;) {
		// printf("pc = 0x%x\n", pc);
		getchar();
		locks = 0;
		printf("%08x %08x %08x %08x", if_id.ir, id_ex.ir, ex_mem.ir, mem_wb.ir);
		printf("mem_wb.rd = %d pc = 0x%x\n", get_rd(mem_wb.ir), mem_wb.npc - 4);
		WB.work_WB();
		printf("ex_mem.rd = %d pc = 0x%x\n", get_rd(ex_mem.ir), ex_mem.npc - 4);
		MEM.work_MEM();
		printf("id_ex.rd = %d pc = 0x%x\n", get_rd(id_ex.ir), id_ex.npc - 4);
		EX.work_EX();
		printf("if_id.rd = %d pc = 0x%x\n", get_rd(if_id.ir), if_id.npc - 4);
		if (ID.work_ID())
			break;
		IF.work_IF();
		disp();
		if (locks == 5) {
			printf("Fuck you!\n");
			for (;;);
		}
	}
	WB.work_WB();
	MEM.work_MEM();
	EX.work_EX();
	WB.work_WB();
	MEM.work_MEM();
	WB.work_WB();
	printf("%d\n", reg[10] & 255);
	// for (;;);
	return 0;
}