#include "CClassManager.h"

CClassManager CClassManager::managerInstance;

CClassManager::CClassManager()
{
}


CClassManager::~CClassManager()
{
}

void CClassManager::registerClass(std::string className, void* instance)
{
	std::string name = stripName(className);
	managerInstance.instances[name].insert(instance);
	initClass(name, instance);
}

void CClassManager::registerDerivedClass(std::string baseClass, std::string derivedClass, void* instance)
{
	std::string baseName = stripName(baseClass);
	std::string derivedName = stripName(derivedClass);
	managerInstance.instances[derivedName].insert(instance);
	initClass(derivedName, instance);
}

void CClassManager::addMethod(std::string className, std::string methodName, std::function<void(void*)> function)
{
	std::string name = stripName(className);
	managerInstance.methodTable[name][methodName] = function;
}

void CClassManager::callMethod(void* instance, std::string methodName)
{
	managerInstance.virtualMethodTable[instance][methodName](instance);
}

void CClassManager::initClass(std::string name, void* instance)
{
	for (auto instance : managerInstance.instances[name]) {
		for (std::map<std::string, std::function<void(void*)>>::iterator function 
			= managerInstance.methodTable[name].begin();
			function != managerInstance.methodTable[name].end(); function++) {
			managerInstance.virtualMethodTable[instance][function->first] = function->second;
		}
	}
}

std::string CClassManager::stripName(std::string& name)
{
	return name.substr(7, name.size() - 7);
}





