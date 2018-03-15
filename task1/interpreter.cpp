#include"CInterpreter.h"

int main(int argc, char const *argv[])
{
	if (argc != 2) {
		std::cout << "Invalid arguments" << std::endl;
	}
	CInterpreter interpreter;
	interpreter.start(argv[1]);
	return 0;
}
