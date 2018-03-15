#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<map>
#include<set>
#include<vector>
#include<iostream>
#include<string>
#include<string.h>
#include"Codes.h"

class CAssembler
{
public:

	CAssembler()
	{
		for (int i = 0; i < TOTAL_SIZE; ++i) {
			memory[i] = 0;
		}
	}

	void generate(std::string inputFileName, std::string outputFileName)
	{
		FILE* output = fopen(outputFileName.c_str(), "w");
		freopen(inputFileName.c_str(), "r", stdin);
		generateMemory();
		int written = 0;
		while (written < TOTAL_SIZE) {
			written += fwrite(memory, sizeof(int), TOTAL_SIZE - written, output);
		}
		std::fclose(output);
	}

private:

	struct variable
	{
		variable(){}

		variable(std::string name)
		{
			variableName = name;
		}

		int address = 0;
		int value = 0;
		std::string variableName;

		bool operator () (const variable& left, const variable& right) const
		{
			return left.variableName > right.variableName;
		}
	};

	struct function
	{
		function(){}

		function(std::string name)
		{
			functionName = name;
		}

		int address = 0;
		int argumentsCount = 0;
		std::string functionName;
		std::vector<int> machineCode;
		std::vector<variable> local;

		bool operator () (const function& left, const function& right) const
		{
			return left.functionName < right.functionName;
		}
	};

	void generateMemory()
	{
		std::string asmInstr;

		while (std::cin >> asmInstr) {
			if (asmInstr == "def") {
					createFunction();
			}
		}
		for (auto var : global) {
			memory[var.address] = var.value;
		}
		memory[LOCAL_STACK_ADDRESS] = currentGlobalVar;
		auto start = functions.find(function("main"));
		memory[IP] = start->address;
	}

	void createFunction()
	{
		std::string name;
		int argumentsCount;
		std::cin >> name;
		function newFunction(name);
		std::cin >> argumentsCount;
		newFunction.argumentsCount = argumentsCount;
		for (int i = 0; i < argumentsCount; ++i) {
			std::cin >> name;
			newFunction.local.push_back(name);
		}
		newFunction.address = currentCode;
		generateFunction(newFunction);
		currentCode += newFunction.machineCode.size();
		for (int i = newFunction.address;
			i < newFunction.machineCode.size() + newFunction.address; ++i) {
			memory[i] = newFunction.machineCode[i - newFunction.address];
		}
		functions.insert(newFunction);
	}

	int parseArgument(std::string& arg)
	{
		if (arg == "REG1") {
			return REG1;
		}

		if (arg == "REG2") {
			return REG2;
		}

		if (arg == "REG3") {
			return REG3;
		}
		auto var = global.find(variable(arg));
		if (var != global.end()) {
			return var->address;
		}
		return -1;
	}

	int parseArgument(function& func, int offset, std::string& arg)
	{
		int tryParse = parseArgument(arg);
		if (tryParse != -1) {
			return tryParse;
		}

		for (int i = 0; i < func.local.size(); i++) {
			if (func.local[i].variableName == arg) {
				func.machineCode.push_back(LOOK);
				func.machineCode.push_back(func.local.size() - i + offset);
				return REG4;
			}
		}

		return -1;
	}



	int parseArgument(function& func, std::string& arg, int& address)
	{
		int tryParse = parseArgument(arg);
		if (tryParse != -1) {
			return tryParse;
		}

		for (int i = 0; i < func.local.size(); ++i) {
			if (func.local[i].variableName == arg) {
				func.machineCode.push_back(LOOK_ADDRESS);
				func.machineCode.push_back(func.local.size() - i);
				address = func.machineCode.size();
				func.machineCode.push_back(1);
				return -1;
			}
		}

		return -1;
	}

	variable createVariable()
	{
		std::string name, value;
		std::cin >> name >> value;
		variable var(name);
		var.value = std::stoi(value);
		return var;
	}

	void addVarible(function& func)
	{
		variable var = createVariable();
		func.machineCode.push_back(PUT_VALUE);
		func.machineCode.push_back(var.value);
		func.local.push_back(var);
	}

	void addStringVariable(function& func)
	{
		std::string name, token, result;
		std::cin >> name;
		std::cin >> token;
		while (token.back() != '\"') {
			result.append(token + ' ');
			std::cin >> token;
		}
		result.append(token);
		result.erase(0, 1);
		result.erase(result.size() - 1, 1);
		result.append("\0");

		variable var(name);
		var.value = currentStringStart;
		strcpy((char*)(memory + currentStringStart), result.c_str());
		currentStringStart += result.size() + 1;
		var.address = currentGlobalVar--;
		global.insert(var);
	}

	void addPrintString(function& func)
	{
		std::string arg;
		std::cin >> arg;
		int code = parseArgument(func, 0, arg);
		if (code != -1) {
			func.machineCode.push_back(PRINT_STR);
			func.machineCode.push_back(code);
		}
	}

	void addPrint(function& func)
	{
		std::string arg;
		std::cin >> arg;
		int code = parseArgument(func, 0, arg);
		if (code != -1) {
			func.machineCode.push_back(PRINT);
			func.machineCode.push_back(code);
		}
	}

	void addMove(function& func)
	{
		std::string arg1, arg2;
		std::cin >> arg1 >> arg2;
		int address;
		int code1 = parseArgument(func, 0, arg1);
		int code2 = parseArgument(func, arg2, address);
		if (code1 == -1) {
			func.machineCode.push_back(VARIABLE);
			func.machineCode.push_back(std::stoi(arg1));
			code1 = func.address + func.machineCode.size() - 1;
		}
		func.machineCode.push_back(MOVE);
		func.machineCode.push_back(code1);

		if (code2 == -1) {
			func.machineCode[address] = func.machineCode.size() + func.address;
			func.machineCode.push_back(1);
		}
		else {
			func.machineCode.push_back(code2);
		}
	}

	void addCall(function& func)
	{
		std::string callName;
		std::cin >> callName;
		function called;
		if (callName == func.functionName) {
			called = func;
		}
		else {
			called = *functions.find(function(callName));
		}
		func.machineCode.push_back(PUT_VALUE);
		func.machineCode.push_back(REG3);
		int address = func.machineCode.size() - 1;

		for (int i = 0; i < called.argumentsCount; i++) {
			std::string arg;
			std::cin >> arg;
			int code = parseArgument(func, 1, arg);
			if (code == -1) {
				func.machineCode.push_back(PUT_VALUE);
				func.machineCode.push_back(std::stoi(arg));
			}
			else {
				func.machineCode.push_back(PUT_FROM_ADDRESS);
				func.machineCode.push_back(code);
			}
		}
		func.machineCode.push_back(GOTO);
		func.machineCode.push_back(called.address);
		func.machineCode[address] = func.address + func.machineCode.size();
	}

	void addReturn(function& func)
	{
		for (int i = 0; i < func.local.size(); i++) {
			func.machineCode.push_back(POP);
		}
		func.machineCode.push_back(LOOK);
		func.machineCode.push_back(1);
		func.machineCode.push_back(POP);
		func.machineCode.push_back(GOTO_ADDRESS);
		func.machineCode.push_back(REG4);
	}

	void addIfEqual(function& func)
	{
		func.machineCode.push_back(IF_EQUAL);
		func.machineCode.push_back(func.address + func.machineCode.size() + 2);
		func.machineCode.push_back(-1);
		stackIf.push_back(func.machineCode.size() - 1);
	}

	void addIfMore(function& func)
	{
		func.machineCode.push_back(IF_MORE);
		func.machineCode.push_back(func.address + func.machineCode.size() + 2);
		func.machineCode.push_back(-1);
		stackIf.push_back(func.machineCode.size() - 1);
	}

	void addEndIf(function& func)
	{
		int address = stackIf.back();
		stackIf.pop_back();
		func.machineCode[address] = func.machineCode.size() + func.address;
	}

	void addScan(function& func)
	{
		std::string arg;
		std::cin >> arg;
		int code = parseArgument(func, 0, arg);
		func.machineCode.push_back(SCAN_REG);
		func.machineCode.push_back(code);
	}

	void generateFunction(function& func)
	{
		std::string command;

		while (true)
		{
			std::cin >> command;
			if (command == "end_function") {
				return;
			}

			if (command == "int") {
				addVarible(func);
				continue;
			}

			if (command == "string") {
				addStringVariable(func);
				continue;
			}

			if (command == "read") {
				addScan(func);
				continue;
			}

			if (command == "write") {
				addPrint(func);
				continue;
			}

			if (command == "write_text") {
				addPrintString(func);
				continue;
			}

			if (command == "move") {
				addMove(func);
				continue;
			}

			if (command == "sum") {
				func.machineCode.push_back(SUM);
				continue;
			}

			if (command == "diff") {
				func.machineCode.push_back(DIFF);
				continue;
			}

			if (command == "call") {
				addCall(func);
				continue;
			}

			if (command == "return") {
				addReturn(func);
				continue;
			}

			if (command == "exit") {
				func.machineCode.push_back(STOP);
				continue;
			}

			if (command == "if_eq") {
				addIfEqual(func);
				continue;
			}

			if (command == "if_more") {
				addIfMore(func);
				continue;
			}

			if (command == "endif") {
				addEndIf(func);
				continue;
			}

		}
	}

	std::set<variable, variable> global;
	std::set<function, function> functions;
	std::vector<int> stackIf;
	int currentStringStart = STRING_VAR_START;
	int currentGlobalVar = GLOBAL_VAR_START;
	int currentCode = CODE_START;
	int memory[TOTAL_SIZE];

};
