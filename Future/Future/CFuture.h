#pragma once
#include<functional>
#include<mutex>
#include<atomic>
#include<thread>

template <class T>
class CFuture
{
public:

	CFuture() 
	{
		isReady.store(false);
		hasExeption.store(false);
	}

	void SetValue(T* data)
	{
		std::unique_lock<std::mutex> lock(mut);
		result = data;
		isReady.store(true);

	}

	void SetException(std::exception& error)
	{
		std::unique_lock<std::mutex> lock(mut);
		exception = error;
		hasExeption.store(true);
	}

	T* Get()
	{ 
		// waiting for the data
		while (!isReady.load() && !hasExeption.load()) {
			std::this_thread::yield();
		}
		if (hasExeption.load()) {
			throw exception;
		}
		std::unique_lock<std::mutex> lock(mut);
		return result;
	}

	bool TryGet(T*& data)
	{
		if (hasExeption.load()) {
			throw exception;
		}
		if (!isReady.load()) {
			return false;
		}
		std::unique_lock<std::mutex> lock(mut);
		data = result;
		return true;
	}

	

private:

	T* result;
	std::atomic<bool> isReady;
	std::atomic<bool> hasExeption;
	std::mutex mut;
	std::exception exception;



};

