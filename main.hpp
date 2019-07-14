#ifndef MAIN_HPP
#define MAIN_HPP

#include "tools.hpp"

enum op_t {
	N, R, I, S, B, U, J
};

enum op_type {
	NONE, // Special
	ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND, // R
	JALR, LB, LH, LW, LBU, LHU, ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI, // I
	SB, SH, SW, // S
	BEQ, BNE, BLT, BGE, BLTU, BGEU, // B
	LUI, AUIPC, // U
	JAL // J
};

extern bool is_branch(op_type);

struct Register {
	int ir, npc, opcode, imm, A, B, ALUoutput, lmd;
	bool cond;
	op_t command_t;
	op_type command_type;

};

class Node { // IF -> ID -> EX -> MEM -> WB

private:

	int delay;

public:

	Node(): delay(0) {}

	void work_IF();

	bool work_ID();

	void work_EX();

	void work_MEM();

	void work_WB();

};

extern Register if_id, id_ex, ex_mem, mem_wb;

extern Node IF, ID, EX, MEM, WB;

extern unsigned char RAM[0x20000];

extern int reg[32], pc, lock[32], lock_RAM[0x20000];

extern int locks;

#endif