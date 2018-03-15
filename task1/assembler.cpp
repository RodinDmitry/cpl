#include"CAssembler.h"


int main(int argc, char const *argv[])
{
	if (argc != 3) {
		std::cout << "Invalid arguments" << std::endl;
	}
	CAssembler assembler;
	assembler.generate(argv[1], argv[2]);
	return 0;
}
