#pragma once
#include"DelayedResult.h"

template <class T>
class CFuture
{
public:

	CFuture(DelayedResult<T>* res)
	{
		result = res;
	}

	T* Get()
	{ 
		// waiting for the data
		std::unique_lock<std::mutex> lock(result->mut);
		while (!result->isReady.load() && !result->hasExeption.load()) {
			result->notification.wait(lock);
		}
		if (result->hasExeption.load()) {
			throw result->exception;
		}
		return result->result;
	}

	bool TryGet(T*& data)
	{
		if (result->hasExeption.load()) {
			throw result->exception;
		}
		if (!result->isReady.load()) {
			return false;
		}
		std::unique_lock<std::mutex> lock(result->mut);
		data = result->result;
		return true;
	}

	

private:

	DelayedResult<T>* result;

};

