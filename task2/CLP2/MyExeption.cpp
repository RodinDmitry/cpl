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
	objects.pop_back();
}

void clearStack()
{
	int end = stackMarks.back();
	stackMarks.pop_back();
	int start = objects.size() - 1;
	for (int i = start; i >= end; i--) {
		objects[i]->~Object();
	}
}

void setStarkMark()
{
	stackMarks.push_back(objects.size());
}