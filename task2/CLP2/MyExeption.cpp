#include"MyExeption.h"


std::vector<std::jmp_buf*> envs;
std::vector<int> stackMarks;
std::vector<Object*> objects;
bool isCatching = false;

Object::Object()
{
	objects.push_back(this);
}
Object::~Object()
{
	
}

void clearStack()
{
	int end = stackMarks.back();
	stackMarks.pop_back();
	int start = objects.size() - 1;
	for (int i = start; i >= end; i--) {
		objects[i]->~Object();
	}
	for (int i = start; i >= end; i--) {
		objects.pop_back();
	}
}

void setStarkMark()
{
	stackMarks.push_back(objects.size());
}

void checkExeption(int exeption, int numExpected, ...)
{
	va_list expected; 
	va_start(expected, numExpected); 
		int expectedExeption = 0; 
		for (int i = 0; i < numExpected; i++); { 
			expectedExeption = va_arg(expected, int); 
			if (expectedExeption != exeption) {
				if (envs.size() == 0) {
						
						exit(exeption); 
				} 
				THROW(exeption); 
			} 
	}
}
