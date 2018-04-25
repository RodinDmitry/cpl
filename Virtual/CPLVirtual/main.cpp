#include"VirtualClass.h"
#include"CClassManager.h"
#include<typeinfo>
#include<iostream>
#include<string>

VIRTUAL_CLASS(baseClass)
int a = 0;
END

VIRTUAL_CLASS_DERIVED(derClass, baseClass)
int b = 1;
END

int main() {

	DECLARE_METHOD(baseClass, "print")
	std::cout << instance->a << std::endl;
	END_DECLARE
	DECLARE_METHOD(derClass, "print") 
	std::cout << instance->b << std::endl;
	END_DECLARE
	baseClass test;
	CALL(&test, "print");
	derClass der;
	der.b = 2;
	baseClass* ptr = reinterpret_cast<baseClass*>(&der);
	CALL(ptr, "print");
	return 0;
}