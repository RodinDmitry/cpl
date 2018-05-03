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

class E : public A {
public:

	E()
	{
		REGISTER_DERIVED_CLASS(1, STR(A));
	}
};

class triangleA
{
public:
	triangleA()
	{
		REGISTER_BASE_CLASS;
	}
	int a;
};

class triangleB : triangleA
{
public:
	triangleB()
	{
		REGISTER_DERIVED_CLASS(1, STR(triangleA));
	}
};

class triangleC : public triangleA, public triangleB
{
public:
	triangleC()
	{
		REGISTER_DERIVED_CLASS(2, STR(triangleA), STR(triangleB));
	}
};

int main()
{

	C inst;
	C* ptr = &inst;
	triangleC instance;
	triangleC* ptrC = &instance;
	//triangleA*  ptrA = DYNAMIC_CAST(triangleA*, ptrC);
	triangleA* statA = static_cast<triangleA*>(ptrC);

	E* res = DYNAMIC_CAST(E*, ptr);
	return 0;
}