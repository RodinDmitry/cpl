#pragma once
#include<string>
#include<set>
#include<typeinfo>
#include<map>
#include<functional>
#include<assert.h>
#include<iostream>

class CClassManager
{
public:
	CClassManager();
	~CClassManager();
	static void registerClass(std::string className, void* instance);
	static void registerDerivedClass(std::string baseClass, std::string derivedClass, void* instance);
	static void addMethod(std::string className
		, std::string methodName
		, std::function<void(void*)> function);
	static void callMethod(void* instance, std::string methodName);
	


private:
	static CClassManager managerInstance;

	std::map<std::string, std::set<void*>> instances;
	std::map<std::string, std::map<std::string, std::function<void(void*)>>> methodTable;
	std::map<void*, std::map<std::string, std::function<void(void*)>>> virtualMethodTable;
	static void initClass(std::string className, void* instance);
	static std::string stripName(std::string& name);
};

