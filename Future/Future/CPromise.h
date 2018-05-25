#pragma once

#include"DelayedResult.h"

template<class T>
class CPromise
{
public:
	
	CPromise(DelayedResult<T>* res)
	{
		result = res;
	}

	void SetValue(T* data)
	{
		std::unique_lock<std::mutex> lock(result->mut);
		result->result = data;
		result->isReady.store(true);
		result->notification.notify_all();

	}

	void SetException(std::exception& error)
	{
		std::unique_lock<std::mutex> lock(result->mut);
		result->exception = error;
		result->hasExeption.store(true);
		result->notification.notify_all();
	}


private:

	DelayedResult<T>* result;
};