#include "CTypeInfo.h"



CTypeInfo::CTypeInfo()
{
}


CTypeInfo::~CTypeInfo()
{
}

CTypeInfo CTypeInfo::getInstance(void* instance)
{
	std::string name = CClassTree::getName(instance);
	return getInstance(name);
}

CTypeInfo CTypeInfo::getInstance(std::string name)
{
	CTypeInfo info;
	info.name = name;
	info.hash = CClassTree::getHash(name);
	info.size = CClassTree::getSize(name);
	return info;
}

inline bool CTypeInfo::operator==(const CTypeInfo& other)
{
	if (this->hash != other.hash) {
		return false;
	}
	return this->name == other.name && this->size == other.size;
}

inline bool CTypeInfo::operator!=(const CTypeInfo& other)
{
	return !(*this == other);
}


