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
		WB.work_WB();
		MEM.work_MEM();
		EX.work_EX();
		if (ID.work_ID())
			break;
		IF.work_IF();
	}
	WB.work_WB();
	MEM.work_MEM();
	EX.work_EX();
	WB.work_WB();
	MEM.work_MEM();
	WB.work_WB();
	printf("%d\n", reg[10] & 255);
	return 0;
}
