#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

class SpinLock
{
public:
	void lock()
	{
		// CAS (Compare-And-Swap)

		bool expected = false;
		bool desired = true;

		// CAS 의사 코드
		//if (_locked == expected)
		//{
		//	expected = _locked;
		//	_locked = desired;
		//	return true;
		//}
		//else
		//{
		//	return false;
		//}

		while (_locked.compare_exchange_strong(expected, desired) == false)
		{
			expected = false;
		}

		// 밑 두 개를 묶어서 위에 한번에 설명한것임
		/*while (_locked)
		{

		}

		_locked = true;*/
	}

	void unlock()
	{
		//_locked = false;
		_locked.store(false);
	}

private:
	atomic<bool> _locked = false;
};

int32 sum = 0;
mutex m;

void Add()
{
	for(int32 i = 0; i < 100000; ++i)
	{
		lock_guard<mutex> guard(m);
		sum++;
	}
}

void Sub()
{
	for (int32 i = 0; i < 100000; ++i)
	{
		lock_guard<mutex> guard(m);
		sum--;
	}
}

int main()
{
	thread t1(Add);
	thread t2(Sub);

	t1.join();
	t2.join();

	cout << sum << endl;
}

