#pragma once
#include<condition_variable>
#include<mutex>
#include<atomic>

template<class T>
struct DelayedResult
{
	T* result;
	std::atomic<bool> isReady = false;
	std::atomic<bool> hasExeption = false;
	std::mutex mut;
	std::exception exception;
	std::condition_variable notification;

};