#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "utility.h"

using namespace std;

const string CODES = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
const unsigned char UPPER_OFFSET = 0;
const unsigned char LOWER_OFFSET = 26;
const unsigned char INT_OFFSET = 52;
const unsigned char PLUS = 62;
const unsigned char SLASH = 63;
const unsigned char EQUAL = 0;
unsigned char buf[4];

void hexdump(istream &infile)
{
	unsigned address = 0;
	vector<unsigned char> vec(16, 0);
	while (true) {
		int idx = 0, beg = 0;
		for (; idx < 16 && infile >> noskipws >> vec[idx]; ++idx) {}
		printf("%06x:", address);
		address += 16;
		for (; beg < min(8, idx); ++beg) {
			printf(" %02x", vec[beg]);
		}
		for (; beg < 8; ++beg) {
			printf(" --");
		}
		printf(" ");
		for (; beg < 16; ++beg) {
			if (beg < idx) {
				printf(" %02x", vec[beg]);
			} else {
				printf(" --");
			}
		}
		printf("  ");
		// If the data byte is >=0x20 and <=0x7e you should use the
		// corresponding ASCII character. If the data byte is <=0x1f
		// or equals to 0x7f, you should print a period character.
		// If the data byte is >=0x80 you should print a ~ character. 
		for (beg = 0; beg < idx; ++beg) {
			if (vec[beg] >= 0x20 && vec[beg] <= 0x7e) {
				printf("%c", vec[beg]);
			} else if (vec[beg] <= 0x1f || vec[beg] == 0x7f) {
				printf(".");
			} else {
				printf("~");
			}
		}
		for (; beg < 16; ++beg) {
			printf(" ");
		}
		printf("\n");
		if (idx < 16) {
			break;
		}
	}
}

void encode_3_bytes(unsigned char in[], int padding) {
	for (int i = 0; i < 4; ++i) {
		buf[i] = 0;
	}
	buf[0] = CODES[(0xfc & in[0]) >> 2];
	buf[1] = CODES[((0x3 & in[0]) << 4) | ((0xf0 & in[1]) >> 4)];
	buf[2] = CODES[((0xf & in[1]) << 2) | ((0xc0 & in[2]) >> 6)];
	buf[3] = CODES[0x3f & in[2]];
	for (int i = 0; i < padding; ++i) {
		buf[3-i] = (unsigned char)'=';
	}
}

void enc_base64(istream &infile)
{
	int cnt = 0;
	unsigned char in[3];
	bool end = false;
	while (!end) {
		int padding=0;
		for (int i = 0; i < 3; ++i) {
			if (!(infile >> noskipws >> in[i])) {
				in[i] = 0;
				padding++;
				end = true;
			}
		}
		// We've already finished reading last time. The
		// input is just of size 3*n.
		if (padding == 3) {
			// If we end at the middle of a line, need to
			// print one more newline
			if (cnt) {
				printf("\n");
			}
			break;
		}
		encode_3_bytes(in, padding);
		printf("%s", buf);
		cnt += 4;
		if (cnt == 64 || end) {
			printf("\n");
			cnt = 0;
		}
	}
}

unsigned char decodeChar(unsigned char ch)
{
	if ('A' <= ch && ch <= 'Z') {
		return ch-'A'+UPPER_OFFSET;
	} else if ('a' <= ch && ch <= 'z') {
		return ch-'a'+LOWER_OFFSET;
	} else if ('0' <= ch && ch <= '9') {
		return ch-'0'+INT_OFFSET;
	} else if (ch == '+') {
		return PLUS;
	} else if (ch == '/') {
		return SLASH;
	} else if (ch == '=') {
		return EQUAL;
	} else {
		fprintf(stderr, "BAD INPUT DATA\n");
		exit(1);
	}
}

string decode_4_bytes(unsigned char in[]) 
{
	unsigned char tmp[4];
	for (int i = 0; i < 4; i++) {
		tmp[i] = decodeChar(in[i]);
	}
	string res;
	res += ((tmp[0] & 0x3f) << 2) | ((tmp[1] & 0x30) >> 4);
	if (in[2] == '=') {
		return res;
	}
	res += ((tmp[1] & 0x0f) << 4) | ((tmp[2] & 0x3c) >> 2);
	if (in[3] == '=') {
		return res;
	}
	res += ((tmp[2] & 0x03) << 6) | ((tmp[3] & 0x3f));
	return res;
}

void dec_base64(istream &infile)
{
	unsigned char in[4];
	while (true) {
		int i = 0;
		for (; i < 4 && infile >> in[i]; ++i) {}
		if (i < 4) {
			if (i == 0) {
				break;
			}
			fprintf(stderr, "INVALID INPUT!\n");
			exit(1);
		}
		cout << decode_4_bytes(in);
	}
}
