#pragma once
#include<vector>
#include<csetjmp>



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
	envs.pop_back(); \
	clearStack(); \
	longjmp(*env, exeption);

#define CATCH(expected) } else { \
	isCatching = false; \
	if (expected != exeption) { \
		if (envs.size() == 0) { \
			exit(exeption); \
		} \
		THROW(exeption); \
	}