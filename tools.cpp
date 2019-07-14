#include "tools.hpp"

// I/O

inline int read_8(const unsigned char *s) {
	return (int)(char)*s;
}

inline int read_u8(const unsigned char *s) {
	return (int)*s;
}

inline int read_16(const unsigned char *s) {
	return (int)(*s | (int)(char)*(s + 1) << 8);
}

inline int read_u16(const unsigned char *s) {
	return (int)(*s | ((unsigned)*(s + 1) << 8));
}

inline int read_32(const unsigned char *s) {
	return (int)(*s | ((int)*(s + 1) << 8) | ((int)*(s + 2) << 16) | ((int)*(s + 3) << 24));
}

inline void write_8(unsigned char *s, int x) {
	*s = x & 255;
}

inline void write_16(unsigned char *s, int x) {
	*s = x & 255;
	*(s + 1) = (x >> 8) & 255;
}

void write_32(unsigned char *s, int x) {
	*s = x & 255;
	*(s + 1) = (x >> 8) & 255;
	*(s + 2) = (x >> 16) & 255;
	*(s + 3) = (x >> 24) & 255;
}

// Integer handling

inline int get_high(int x, int l) { // sign extended
	return x >> l;
}

inline int get_low(int x, int r) {
	return x & ((1 << (r + 1)) - 1);
}

inline int get_range(int x, int l, int r) {
	return get_low(get_high(x, l), r - l);
}

inline int get_bit(int x, int t) {
	return (x >> t) & 1;
}

inline int get_highest_bit(int x, int t) {
	return ((x >> t) & 1) ? -1 : 0;
}

// Operation decoding

inline int get_opcode(int x) {
	return get_low(x, 6);
}

inline int get_rs1(int x) {
	return get_range(x, 15, 19);
}

inline int get_rs2(int x) {
	return get_range(x, 20, 24);
}

inline int get_funct3(int x) {
	return get_range(x, 12, 14);
}

inline int get_rd(int x) {
	return get_range(x, 7, 11);
}

inline int get_inst_i(int x) {
	return get_high(x, 20);
}

inline int get_inst_s(int x) {
	return (get_high(x, 25) << 5) | get_range(x, 7, 11);
}

inline int get_inst_b(int x) {
	return (get_range(x, 8, 11) << 1) | (get_range(x, 25, 30) << 5) |
		(get_bit(x, 7) << 11) | (get_highest_bit(x, 31) << 12);
}

inline int get_inst_u(int x) {
	return x & -(1 << 12);
}

inline int get_inst_j(int x) {
	return (get_range(x, 21, 30) << 1) | (get_bit(x, 20) << 11) |
		(get_range(x, 12, 19) << 12) | (get_highest_bit(x, 31) << 20);
}