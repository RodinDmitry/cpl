#include<iostream>
#include"CClassTree.h"
#include"DynamicCast.h"


class A {
public:
	A() 
	{
		REGISTER_BASE_CLASS;
	}

	int a;
};

class B : public A {
public:
	B()
	{
		REGISTER_DERIVED_CLASS(1, STR(A));
	}

	int b;
};

class D
{
public:

	D()
	{
		REGISTER_BASE_CLASS;
	}

	int d;

};

class C : public D, public B {
public:

	C()
	{
		REGISTER_DERIVED_CLASS(2, STR(D), STR(B));
	}

	int c;
};



int main()
{
	C inst;
	C* ptr = &inst;
	A* ptra = dynamic_cast<A*>(ptr);

	B* ptrb = dynamic_cast<B*>(ptr);
	
	std::cout << STR(decltype(ptr)) << std::endl;

	A* testA = DYNAMIC_CAST(A*, ptr);

	return 0;
}