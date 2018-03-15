#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include"Codes.h"
#include<iostream>
#include<stdio.h>

#define instruction memory[IP]

int memory[TOTAL_SIZE];

class CInterpreter
{
public:

	void start(std::string inputFileName)
	{
		FILE* f = fopen(inputFileName.c_str(), "r");
		fread((void*)(memory), sizeof(int), TOTAL_SIZE, f);
		fclose(f);
		bool flag = true;
		while (flag) {
			flag = runCommand();
			flush(inputFileName);
		}
	}

private:

	bool runCommand()
	{
		switch (memory[instruction]) {
		case MOVE: runMove(); break;
		case SUM: runSum(); break;
		case DIFF: runDiff(); break;
		case PRINT: runPrint(); break;
		case SCAN_REG: runScan(); break;
		case IF_EQUAL: runIfEqual(); break;
		case IF_MORE: runIfMore(); break;
		case VARIABLE: instruction += 2; break;
		case PUT_VALUE: runPut(); break;
		case PUT_FROM_ADDRESS: runPutFrom(); break;
		case POP: runPop(); break;
		case LOOK: runLook(); break;
		case LOOK_ADDRESS: runLookAndAdd(); break;
		case GOTO: runGoto(); break;
		case GOTO_ADDRESS: runGotoAdress(); break;
		case PRINT_STR: runPrintString(); break;
		}

		if (memory[instruction] == STOP){
			return false;
		}
		return true;
	}

	void runIfEqual()
	{
		if(memory[REG1] == memory[REG2]) {
			instruction = memory[instruction + 1];
			return;
		}
		instruction = memory[instruction + 2];
	}

	void runIfMore()
	{
		if(memory[REG1] > memory[REG2]) {
			instruction = memory[instruction + 1];
			return;
		}
		instruction = memory[instruction + 2];
	}

	void runEqual()
	{
		if (memory[REG1] == memory[REG2]) {
			memory[REG3] = 1;

		}
		else {
			memory[REG3] = 0;
		}
		instruction++;
	}

	void runMove()
	{
		memory[memory[instruction + 2]] = memory[memory[instruction + 1]];
		instruction += 3;
	}

	void runSum()
	{
		memory[REG3] = memory[REG1] + memory[REG2];
		instruction++;
	}

	void runDiff()
	{
		memory[REG3] = memory[REG1] - memory[REG2];
		instruction++;
	}

	void runPrint()
	{
		switch (memory[instruction + 1]) {
			case 1: std::cout << memory[REG1] << std::endl; break;
			case 2: std::cout << memory[REG2] << std::endl; break;
			case 3: std::cout << memory[REG3] << std::endl; break;
		}
		instruction += 2;
	}

	void runScan()
	{
		switch (memory[instruction + 1]) {
			case 1: std::cin >> memory[REG1];  break;
			case 2: std::cin >> memory[REG2];  break;
			case 3: std::cin >> memory[REG3]; break;
		}
		instruction += 2;
	}

	void runPut()
	{
		memory[memory[LOCAL_STACK_ADDRESS]--] = memory[instruction + 1];
		instruction += 2;
	}

	void runPop()
	{
		memory[LOCAL_STACK_ADDRESS]++;
		instruction++;
	}

	void runLook()
	{
		memory[REG4] = memory[memory[LOCAL_STACK_ADDRESS] + memory[instruction + 1]];
		instruction += 2;
	}

	void runGoto()
	{
		instruction = memory[instruction + 1];
	}

	void runGotoAdress()
	{
		instruction = memory[memory[instruction + 1]];
	}

	void runPutFrom()
	{
		memory[memory[LOCAL_STACK_ADDRESS]--] = memory[memory[instruction + 1]];
		instruction+=2;
	}

	void runLookAndAdd()
	{
		memory[memory[instruction + 2]] = memory[LOCAL_STACK_ADDRESS] + memory[instruction + 1];
		instruction += 3;
	}

	void runPrintString()
	{
		printf("%s", (char*)(memory + memory[memory[instruction + 1]]));
		instruction += 2;
	}

	void flush(std::string vmFile)
	{
		FILE* output = fopen(vmFile.c_str(), "w");
		int written = 0;
		while (written < TOTAL_SIZE) {
			written += fwrite(memory, sizeof(int), TOTAL_SIZE - written, output);
		}
		std::fclose(output);
	}
};
