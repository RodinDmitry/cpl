#include "CClassTree.h"

CClassTree CClassTree::manager;

CClassTree::CClassTree()
{
}


CClassTree::~CClassTree()
{
}

void CClassTree::AddInstance(void* instance, std::string name)
{
	manager.instanceToName[instance] = name;
}

void CClassTree::CreateBaseClass(std::string & name, int size)
{
	if (manager.nameToNode.find(name) != manager.nameToNode.end()) {
		return;
	}
	Node* current = manager.createBaseClass(size, name);
}

void CClassTree::CreateClass(std::string& name, int size, int numberOfParents, ...)
{
	if (manager.nameToNode.find(name) != manager.nameToNode.end()) {
		return;
	}
	Node* current = manager.createBaseClass(size, name);
	va_list arguments;
	va_start(arguments, numberOfParents);
	for (int i = 0; i < numberOfParents; i++) {
		std::string name = va_arg(arguments, char*);
		Node* parent = manager.nameToNode[name];
		manager.updateClass(current, parent);
	}
}

int CClassTree::GetShift(void* instance, std::string target)
{
	std::string start = manager.instanceToName[instance];
	target = target.substr(0, target.size() - 1);
	Node* startNode = manager.nameToNode[start];
	Node* targetNode = manager.nameToNode[target];
	std::vector<Node*> upPath, downPath;
	bool upCast = manager.isParent(targetNode, startNode, upPath);
	bool downCast = manager.isParent(startNode, targetNode, downPath);
	std::reverse(downPath.begin(), downPath.end());
	int shift = 0;
	if (downCast) {
		for (int i = 0; i < downPath.size() - 1; i++) {
			shift += manager.castStep(downPath[i], downPath[i + 1]);
		}
		shift *= -1;
	} 
	if (upCast) {
		for (int i = 0; i < upPath.size() - 1; i++) {
			shift += manager.castStep(upPath[i], upPath[i + 1]);
		}
	}

	return shift;
}

std::string CClassTree::getName(void* instance)
{
	return manager.instanceToName[instance];
}

int CClassTree::getSize(std::string name)
{
	return manager.nameToNode[name]->size;
}

unsigned int CClassTree::getHash(std::string name)
{
	return manager.nameToNode[name]->hash;
}

CClassTree::Node* CClassTree::createBaseClass(int size, std::string& name)
{
	Node* node = new Node();
	node->size = size;
	node->className = name;
	std::hash<std::string> hashFunction;
	node->hash = hashFunction(name + std::to_string(size));
	nameToNode[name] = node;
	nodes.push_back(node);
	return node;
}

void CClassTree::updateClass(Node* child, Node* parent)
{
	parent->children.push_back(child);
	child->parents.push_back(parent);
}

bool CClassTree::isParent(Node * parent, Node * child, std::vector<Node*>& path)
{
	if (parent == child) {
		path.push_back(parent);
		return true;
	}
	for (int i = 0; i < parent->children.size(); i++) {
		if (isParent(parent->children[i], child, path)) {
			path.push_back(parent);
			return true;
		}
	}
	return false;
}

int CClassTree::castStep(Node* parent, Node* child)
{
	int shift = 0;
	for (int i = 0; i < child->parents.size(); i++) {
		if (child->parents[i] == parent) {
			return shift;
		}
		shift += child->parents[i]->size;
	}
	return shift;
}

