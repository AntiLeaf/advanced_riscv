#include <cstdio>

#include "tools.hpp"
#include "main.hpp"

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
		// getchar();
		IF.work_IF();
		if (if_id.ir == 0x00c68223) {
			printf("%d\n", reg[10] & 255);
			break;
		}
		ID.work_ID();
		EX.work_EX();
		MEM.work_MEM();
		WB.work_WB();
	}
	return 0;
}