#include"MyExeption.h"
#include<iostream>

class NewObject : public Object
{
public:
	NewObject() {};
	~NewObject() override
	{
		std::cout << "Destroyed" << std::endl;
	}
};

void test1() // expected exit 5
{
	TRY
	{
		int a = 5;
	NewObject obj;
	THROW(5);
	}
		CATCH(1, 4)
	}
}

void func(int i)
{
	if (i == 3) {
		THROW(1);
	}
	else
	{
		NewObject obj;
		func(i + 1);
	}
}

void test2()
{
	TRY func(0);
	CATCH(2, 1, 2) std::cout << "Caught" << std::endl; }
}

int main()
{

	test2();
	//test1();

	return 0;
}