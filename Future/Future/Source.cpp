#include"CFuture.h"
#include"CPromise.h"
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

void bar(CPromise<int>& promise)
{
	int* res = foo();
	promise.SetValue(res);
}

void test()
{
	DelayedResult<int> result;
	CFuture<int> future(&result);
	CPromise<int> promise(&result);

	std::thread newThread(bar, std::ref(promise));
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


void bar2(CPromise<int>& promise)
{
	try {
		throw std::overflow_error("test");
	}
	catch (std::exception err) {
		promise.SetException(err);
	}
}

void testException()
{
	DelayedResult<int> res;
	CFuture<int> future(&res);
	CPromise<int> promise(&res);
	std::thread newThread(bar2, std::ref(promise));
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