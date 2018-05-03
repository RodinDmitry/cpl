#pragma once
#include"CClassTree.h"
#include"CTypeInfo.h"
#include<stdarg.h>

#define STR(name) #name

#define REGISTER_BASE_CLASS std::string name = __func__; \
CClassTree::CreateBaseClass(name, sizeof(*this)); \
CClassTree::AddInstance(this, name); 

#define REGISTER_DERIVED_CLASS(numParents, ...) \
std::string name = __func__; \
CClassTree::CreateClass(name, sizeof(B), numParents, __VA_ARGS__); \
CClassTree::AddInstance(this, name);

#define DYNAMIC_CAST(target, instance)  reinterpret_cast<target>(CClassTree::Cast(instance, STR(target)))

#define TYPEID(obj) CTypeInfo::getInstance(obj)
