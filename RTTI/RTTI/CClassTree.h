#pragma once
#include<string>
#include<vector>
#include<set>
#include<map>
#include<stdarg.h>
#include<algorithm>


class CClassTree
{
public:
	CClassTree();
	~CClassTree();

	static void AddInstance(void* instance, std::string name);
	static void CreateBaseClass(std::string& name, int size);
	static void CreateClass(std::string& name, int size, int numberOfParents, ...);
	static bool GetShift(void* instance, std::string target, int& shift);
	static void* Cast(void* instance, std::string target);
	static std::string getName(void* instance);
	static int getSize(std::string name);
	static unsigned int getHash(std::string name);
	

private:

	static CClassTree manager;

	struct Node
	{
		std::string className;
		int size;
		unsigned int hash;
		std::vector<Node*> parents;
		std::vector<Node*> children;
	};

	std::vector<Node*> nodes;
	std::map<void*, std::string> instanceToName;
	std::map<void*, void*> backwardSideCast; // cast to most derived ptr
	std::map<std::string, Node*> nameToNode;


	Node* createBaseClass(int size, std::string& name);
	void updateClass(Node* child, Node* parent);
	bool isParent(Node* parent, Node* child, std::vector<Node*>& path);
	int castStep(Node* parent, Node* child);
	
	
};

