#pragma once
#include<vector>
#include<csetjmp>
#include<iostream>
#include<stdarg.h>



class Object
{
public:
	Object();
	virtual ~Object();
};

extern std::vector<std::jmp_buf*> envs;
extern std::vector<int> stackMarks;
extern std::vector<Object*> objects;
extern bool isCatching;

void clearStack();
void setStarkMark();
void checkExeption(int exeption, int numExpected, ...);

#define TRY std::jmp_buf env; \
	envs.push_back(&env); \
	setStarkMark(); \
	int exeption = setjmp(env); \
	if (exeption == 0) {

#define THROW(exeption) std::jmp_buf* env = (std::jmp_buf*)(*envs.back()); \
	if(isCatching) { \
	exit(exeption); \
	} \
	isCatching = true; \
	std::cout << envs.size() << std::endl; \
	envs.pop_back(); \
	clearStack(); \
	longjmp(*env, exeption);

#define CATCH(numExpected, ...) } else { \
	isCatching = false; \
	checkExeption(exeption, numExpected, __VA_ARGS__);