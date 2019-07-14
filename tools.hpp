#ifndef TOOLS_HPP
#define TOOLS_HPP

// I/O

extern int read_8(const unsigned char*);

extern int read_u8(const unsigned char*);

extern int read_16(const unsigned char*);

extern int read_u16(const unsigned char*);

extern int read_32(const unsigned char*);

extern void write_8(unsigned char*, int);

extern void write_16(unsigned char*, int);

extern void write_32(unsigned char*, int);

// Integer handling

extern int get_high(int, int);

extern int get_low(int, int);

extern int get_range(int, int, int);

extern int get_bit(int, int);

extern int get_highest_bit(int, int);

// Operation decoding

extern int get_opcode(int);

extern int get_rs1(int);

extern int get_rs2(int);

extern int get_funct3(int);

extern int get_rd(int);

extern int get_inst_i(int);

extern int get_inst_s(int);

extern int get_inst_b(int);

extern int get_inst_u(int);

extern int get_inst_j(int);

#endif