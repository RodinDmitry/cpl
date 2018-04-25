#pragma once
#include"CClassManager.h"
#include<iostream>


#define VIRTUAL_CLASS(className) struct className { \
	className() { \
		std::string name = typeid(this).name(); \
		CClassManager::registerClass(name.substr(0, name.size() - 2), this); \
	}

#define VIRTUAL_CLASS_DERIVED(className, baseClass) struct className : baseClass { \
		className() { \
		std::string name = typeid(this).name(); \
		std::string baseName = typeid(baseClass).name(); \
		CClassManager::registerDerivedClass(baseName, name.substr(0, name.size() - 2), this); \
	}

#define END };

#define DECLARE_METHOD(baseClass, methodName) { \
CClassManager::addMethod(typeid(baseClass).name(), methodName, [](void* ptr) { \
std::cout << "Called method from " << typeid(baseClass).name() << std::endl; \
baseClass* instance = reinterpret_cast<baseClass*>(ptr);

#define END_DECLARE });}

#define CALL(instance, methodName) 	CClassManager::callMethod(instance, methodName);
