#pragma once
#include"CClassTree.h"

class CTypeInfo
{
public:
	CTypeInfo();
	~CTypeInfo();

	static CTypeInfo getInstance(void* instance);
	static CTypeInfo getInstance(std::string name);

	std::string name;
	int size;
	int hash;

	inline bool operator==(const CTypeInfo& other);
	inline bool operator!=(const CTypeInfo& other);

};

