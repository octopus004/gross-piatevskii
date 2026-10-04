#include"3d.h"
#include "real3d.h"
#include"cfg.h"
#include <iostream>
int main(int argc, char** argv) {
	std::cout << "w"<<std::endl;
	//std::cout << cfg_init(argv[2]);
	if (!cfg_init(argv[2])) {
		fprintf(stderr, "Wrong input parameter file.\n");
		exit(EXIT_FAILURE);
	}
	std::cout << cfg_size << std::endl;
	RealThreeD real;
	FILE* out;
	out = stdout;
	real.write(out);
	return 0;
}