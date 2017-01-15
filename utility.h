#ifndef __UTILITY_H_
#define __UTILITY_H_

#include <iostream>

void hexdump(std::istream &infile);
void enc_base64(std::istream &infile);
void dec_base64(std::istream &infile);

#endif