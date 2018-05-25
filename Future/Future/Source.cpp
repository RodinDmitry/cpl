#include"CFuture.h"
#include<thread>
#include<iostream>

int* foo()
{
	int* result = new int;
	*result = 0;
	for (int i = 0; i < 10000; i++) {
		result[0]++;
	}
	return result;
}

void bar(CFuture<int>& future)
{
	int* res = foo();
	future.SetValue(res);
}

void test()
{
	CFuture<int> future;
	std::thread newThread(bar, std::ref(future));
	for (int i = 0; i < 10; i++) {
		int* data;
		bool result = future.TryGet(data);
		std::cout << result << std::endl;
		if (result) {
			std::cout << "Result " << data[0] << std::endl;
			delete data;
			break;
		}
	}
	newThread.join();
}

void bar2(CFuture<int>& future)
{
	try {
		throw std::overflow_error("test");
	}
	catch (std::exception err) {
		future.SetException(err);
	}
}

void testException()
{
	CFuture<int> future;
	std::thread newThread(bar2, std::ref(future));
	newThread.join();
	try
	{
		future.Get();
	}
	catch (const std::exception err)
	{
		std::cout << err.what() << std::endl;
	}
}

int main()
{
	test();
	testException();
	system("pause");
	return 0;
}