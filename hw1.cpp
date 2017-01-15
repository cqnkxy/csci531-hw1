#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "utility.h"

using namespace std;

void parseCmdAndRun(int argc, char *argv[]) {
	if (argc > 3 || argc < 2) {
		fprintf(stderr, "Malformed command\n");
	} else if (strcmp(argv[1], "hexdump") == 0) {
		if (argc == 3) {
			ifstream infile(argv[2]);
			if (!infile.is_open()) {
				fprintf(stderr, "input file %s doesn't exists\n", argv[2]);
				exit(1);
			}
			hexdump(infile);
		} else {
			hexdump(cin);
		}
	} else if (strcmp(argv[1], "enc-base64") == 0) {
		if (argc == 3) {
			ifstream infile(argv[2]);
			if (!infile.is_open()) {
				fprintf(stderr, "input file %s doesn't exists\n", argv[2]);
				exit(1);
			}
			enc_base64(infile);
		} else {
			enc_base64(cin);
		}
	} else if (strcmp(argv[1], "dec-base64") == 0) {
		if (argc == 3) {
			ifstream infile(argv[2]);
			if (!infile.is_open()) {
				fprintf(stderr, "input file %s doesn't exists\n", argv[2]);
				exit(1);
			}
			dec_base64(infile);
		} else {
			dec_base64(cin);
		}
	} else {
		fprintf(stderr, "Malformed command\n");
	}
}

int main(int argc, char *argv[]) 
{
	parseCmdAndRun(argc, argv);
}
