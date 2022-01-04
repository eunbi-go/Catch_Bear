#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

vector<int32> v;
mutex m;		// 일종의 자물쇠

// RALL
template<typename T>
class LockGuard
{
public:
	LockGuard(T& m)
	{
		_mutex = &m;
		_mutex->lock();
	}
	~LockGuard()
	{
		_mutex->unlock();
	}
private:
	T* _mutex;
};

void Push()
{
	for (int32 i = 0; i < 10000; ++i)
	{
		//LockGuard<std::mutex> LockGuard(m);
		//std::lock_guard<std::mutex> LockGuard(m);
		std::unique_lock<std::mutex> uniqueLock(m, std::defer_lock);	// 잠기는 시점을 뒤로 미룰수있다	

		uniqueLock.lock();
		//m.lock();

		v.push_back(i);

		//m.unlock();
	}
}

int main()
{
	std::thread t1(Push);
	std::thread t2(Push);

	t1.join();
	t2.join();

	cout << v.size() << endl;
}

