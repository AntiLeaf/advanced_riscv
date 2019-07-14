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

	void work_ID();

	void work_EX();

	void work_MEM();

	void work_WB();

};


Register if_id, id_ex, ex_mem, mem_wb;

Node IF, ID, EX, MEM, WB;

unsigned char RAM[0x20000];

int reg[32], pc;

void Node::work_IF() { // 指令提取
	if (ex_mem.cond)
		pc = ex_mem.ALUoutput;
	if_id.npc = pc;
	// printf("\npc = 0x%x\n", pc);

	if_id.ir = read_32(RAM + pc);

	if (if_id.ir == 0x00c68223)
		return;

	// if (ex_mem.cond)
		// printf("ALUoutput=0x%x\n", ex_mem.ALUoutput);
	pc += 4;
}

void Node::work_ID() { // 指令译码/寄存器提取
	int command = if_id.ir;

	id_ex.opcode = get_opcode(command);

	if (id_ex.opcode == 0b0110011) { // R
		id_ex.command_t = R;
		int funct3 = get_funct3(command);

		if (funct3 == 0b000) {
			if (!get_bit(command, 30))
				id_ex.command_type = ADD;
			else
				id_ex.command_type = SUB;
		}

		else if (funct3 == 0b001)
			id_ex.command_type = SLL;

		else if (funct3 == 0b010)
			id_ex.command_type = SLT;

		else if (funct3 == 0b011)
			id_ex.command_type = SLTU;

		else if (funct3 == 0b100)
			id_ex.command_type = XOR;

		else if (funct3 == 0b101) {
			if (!get_bit(command, 30))
				id_ex.command_type = SRL;
			else
				id_ex.command_type = SRA;
		}

		else if (funct3 == 0b110)
			id_ex.command_type = OR;

		else if (funct3 == 0b111)
			id_ex.command_type = AND;
	}

	else if (id_ex.opcode == 0b1100111 || id_ex.opcode == 0b0000011 || id_ex.opcode == 0b0010011) { // I
		id_ex.command_t = I;
		id_ex.imm = get_inst_i(if_id.ir);
		int funct3 = get_funct3(if_id.ir);

		if (id_ex.opcode == 0b1100111)
			id_ex.command_type = JALR;

		else {

			if (id_ex.opcode == 0b0000011) { // LB, LH, LW, LBU, LHU

				if (funct3 == 0b000)
					id_ex.command_type = LB;

				else if (funct3 == 0b001)
					id_ex.command_type = LH;

				else if (funct3 == 0b010)
					id_ex.command_type = LW;

				else if (funct3 == 0b100)
					id_ex.command_type = LBU;

				else if (funct3 == 0b101)
					id_ex.command_type = LHU;
			}

			else if (id_ex.opcode == 0b0010011) { // ADDI, SLTI, SLTIU, XORI, ORI, ANDI

				if (funct3 == 0b000)
					id_ex.command_type = ADDI;

				else if (funct3 == 0b010)
					id_ex.command_type = SLTI;

				else if (funct3 == 0b011)
					id_ex.command_type = SLTIU;

				else if (funct3 == 0b100)
					id_ex.command_type = XORI;

				else if (funct3 == 0b110)
					id_ex.command_type = ORI;

				else if (funct3 == 0b111)
					id_ex.command_type = ANDI;

				else if (funct3 == 0b001)
					id_ex.command_type = SLLI;

				else if (funct3 == 0b101) {
					if (!get_bit(command, 30))
						id_ex.command_type = SRLI;
					else
						id_ex.command_type = SRAI;
				}
			}
		}
	}

	else if (id_ex.opcode == 0b0100011) { // S
		id_ex.command_t = S;
		id_ex.imm = get_inst_s(command);
		int funct3 = get_funct3(command);

		if (funct3 == 0b000)
			id_ex.command_type = SB;

		else if (funct3 == 0b001)
			id_ex.command_type = SH;

		else if (funct3 == 0b010)
			id_ex.command_type = SW;
	}

	else if (id_ex.opcode == 0b1100011) { // B
		id_ex.command_t = B;
		id_ex.imm = get_inst_b(command);
		int funct3 = get_funct3(command);

		if (funct3 == 0b000)
			id_ex.command_type = BEQ;

		else if (funct3 == 0b001)
			id_ex.command_type = BNE;

		else if (funct3 == 0b100)
			id_ex.command_type = BLT;

		else if (funct3 == 0b101)
			id_ex.command_type = BGE;

		else if (funct3 == 0b110)
			id_ex.command_type = BLTU;

		else if (funct3 == 0b111)
			id_ex.command_type = BGEU;
	}

	else if (id_ex.opcode == 0b0110111 || id_ex.opcode == 0b0010111) { // U
		id_ex.command_t = U;
		id_ex.imm = get_inst_u(command);

		if (id_ex.opcode == 0b0110111)
			id_ex.command_type = LUI;

		else if (id_ex.opcode == 0b0010111)
			id_ex.command_type = AUIPC;
	}

	else if (id_ex.opcode == 0b1101111) { // J
		id_ex.command_t = J;
		id_ex.imm = get_inst_j(command);
		// printf("command = 0x%x  imm = 0x%x\n", command, id_ex.imm);
		id_ex.command_type = JAL;
	}

	id_ex.A = reg[get_rs1(if_id.ir)];
	id_ex.B = reg[get_rs2(if_id.ir)];
	// printf("rs1 = %d  rs2 = %d  rd = %d\n", get_rs1(if_id.ir), get_rs2(if_id.ir), get_rd(if_id.ir));

	id_ex.ir = command;
	id_ex.npc = if_id.npc;
	
	// check(id_ex.command_type);
}

void Node::work_EX() { // 执行/有效地址

	ex_mem.ir = id_ex.ir;
	ex_mem.command_t = id_ex.command_t;
	ex_mem.command_type = id_ex.command_type;
	ex_mem.A = id_ex.A;
	ex_mem.B = id_ex.B;
	ex_mem.imm = id_ex.imm;

	ex_mem.cond = false;

	if (id_ex.command_t == R) {

		if (id_ex.command_type == ADD)
			ex_mem.ALUoutput = id_ex.A + id_ex.B;

		else if (id_ex.command_type == SUB)
			ex_mem.ALUoutput = id_ex.A - id_ex.B;

		else if (id_ex.command_type == SLL)
			ex_mem.ALUoutput = id_ex.A << (id_ex.B & 31);

		else if (id_ex.command_type == SLT)
			ex_mem.ALUoutput = (int)(id_ex.A < id_ex.B);

		else if (id_ex.command_type == SLTU)
			ex_mem.ALUoutput = (int)((unsigned)id_ex.A < (unsigned)id_ex.B);

		else if (id_ex.command_type == XOR)
			ex_mem.ALUoutput = id_ex.A ^ id_ex.B;

		else if (id_ex.command_type == SRL)
			ex_mem.ALUoutput = (unsigned)id_ex.A >> (id_ex.B & 31);

		else if (id_ex.command_type == SRA)
			ex_mem.ALUoutput = id_ex.A >> (id_ex.B & 31);

		else if (id_ex.command_type == OR)
			ex_mem.ALUoutput = id_ex.A | id_ex.B;

		else if (id_ex.command_type == AND)
			ex_mem.ALUoutput = id_ex.A & id_ex.B;
	}

	else if (id_ex.command_t == I) {

		// printf("imm = %d\n", id_ex.imm);

		if (id_ex.command_type == JALR) {
			ex_mem.ALUoutput = (id_ex.A + id_ex.imm) & -2;
			ex_mem.imm = id_ex.npc + 4;
			ex_mem.cond = true;
		}

		else {

			if (id_ex.command_type == LB || id_ex.command_type == LH || id_ex.command_type == LW || id_ex.command_type == LBU || id_ex.command_type == LHU) {
				ex_mem.ALUoutput = id_ex.A + id_ex.imm;
				ex_mem.B = id_ex.B;
			}

			else if (id_ex.command_type == ADDI)
				ex_mem.ALUoutput = id_ex.A + ex_mem.imm;

			else if (id_ex.command_type == SLTI)
				ex_mem.ALUoutput = (int)(id_ex.A < ex_mem.imm);

			else if (id_ex.command_type == SLTIU)
				ex_mem.ALUoutput = (int)((unsigned)id_ex.A < (unsigned)ex_mem.imm);

			else if (id_ex.command_type == XORI)
				ex_mem.ALUoutput = id_ex.A ^ id_ex.imm;

			else if (id_ex.command_type == ORI)
				ex_mem.ALUoutput = id_ex.A ^ id_ex.imm;

			else if (id_ex.command_type == ANDI)
				ex_mem.ALUoutput = id_ex.A & id_ex.imm;

			else if (id_ex.command_type == SLLI)
				ex_mem.ALUoutput = id_ex.A << (id_ex.imm & 31);

			else if (id_ex.command_type == SRLI)
				ex_mem.ALUoutput = (unsigned)id_ex.A >> (id_ex.imm & 31);

			else if (id_ex.command_type == SRAI)
				ex_mem.ALUoutput = id_ex.A >> (id_ex.imm & 31);
		}
	}

	else if (id_ex.command_t == S) {
		ex_mem.ALUoutput = id_ex.imm + id_ex.A;
		delay = 3;
	}

	else if (id_ex.command_t == B) {
		ex_mem.ALUoutput = id_ex.npc + id_ex.imm;

		if (id_ex.command_type == BEQ)
			ex_mem.cond = (id_ex.A == id_ex.B);

		else if (id_ex.command_type == BNE)
			ex_mem.cond = (id_ex.A != id_ex.B);

		else if (id_ex.command_type == BLT)
			ex_mem.cond = (id_ex.A < id_ex.B);

		else if (id_ex.command_type == BGE)
			ex_mem.cond = (id_ex.A >= id_ex.B);

		else if (id_ex.command_type == BLTU)
			ex_mem.cond = ((unsigned)id_ex.A < (unsigned)id_ex.B);

		else if (id_ex.command_type == BGEU)
			ex_mem.cond = ((unsigned)id_ex.A >= (unsigned)id_ex.B);

		// printf("A = %d  B = %d  cond = %d\n", id_ex.A, id_ex.B, (int)ex_mem.cond);
	}

	else if (id_ex.command_t == U) {
		if (id_ex.command_type == AUIPC)
			ex_mem.ALUoutput = id_ex.imm + id_ex.npc;

		else ex_mem.imm = id_ex.imm;
	}

	else if (id_ex.command_t == J) {
		// printf("JAL!\n");
		ex_mem.ALUoutput = id_ex.npc + id_ex.imm;
		ex_mem.imm = id_ex.npc + 4;
		ex_mem.cond = true;
	}
}

void Node::work_MEM() { // 存储器访问
	mem_wb.ir = ex_mem.ir;
	mem_wb.command_t = ex_mem.command_t;
	mem_wb.command_type = ex_mem.command_type;
	mem_wb.ALUoutput = ex_mem.ALUoutput;
	mem_wb.imm = ex_mem.imm;

	if (ex_mem.command_t == I) {
		if (ex_mem.command_type == LB)
			mem_wb.lmd = read_8(RAM + ex_mem.ALUoutput);

		else if (ex_mem.command_type == LH)
			mem_wb.lmd = read_16(RAM + ex_mem.ALUoutput);

		else if (ex_mem.command_type == LW)
			mem_wb.lmd = read_32(RAM + ex_mem.ALUoutput);

		else if (ex_mem.command_type == LBU)
			mem_wb.lmd = read_u8(RAM + ex_mem.ALUoutput);

		else if (ex_mem.command_type == LHU)
			mem_wb.lmd = read_u16(RAM + ex_mem.ALUoutput);
	}

	else if (ex_mem.command_t == S) {
		if (ex_mem.command_type == SB)
			write_8(RAM + ex_mem.ALUoutput, ex_mem.B);

		else if (ex_mem.command_type == SH)
			write_16(RAM + ex_mem.ALUoutput, ex_mem.B);

		else if (ex_mem.command_type == SW)
			write_32(RAM + ex_mem.ALUoutput, ex_mem.B);
	}
}

void Node::work_WB() { // 写回
	int rd = get_rd(mem_wb.ir);
	if (rd) {
		if (mem_wb.command_t == R)
			reg[rd] = mem_wb.ALUoutput;

		else if (mem_wb.command_t == I) {

			if (mem_wb.command_type == LB || mem_wb.command_type == LH || mem_wb.command_type == LW || mem_wb.command_type == LBU || mem_wb.command_type == LHU)
				reg[rd] = mem_wb.lmd;

			else if (mem_wb.command_type == JALR)
				reg[rd] = mem_wb.imm;

			else {
				reg[rd] = mem_wb.ALUoutput;
				// printf("reg[rd] = %d\n", reg[rd]);
			}
		}

		else if (mem_wb.command_t == U) {

			if (mem_wb.command_type == LUI) {
				reg[rd] = mem_wb.imm;
				// printf("reg[rd] = %d\n", reg[rd]);
			}

			else if (mem_wb.command_type == AUIPC)
				reg[rd] = mem_wb.ALUoutput;
		}

		else if (mem_wb.command_type == JAL) {
			reg[rd] = mem_wb.imm;
			// printf("reg[rd] = 0x%x\n", reg[rd]);
		}
	}
}

#endif