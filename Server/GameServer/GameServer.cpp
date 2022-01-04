#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>

// atomic : All Or Nothing
// atomic 연산은 많이 느려서 정말로 필요할때만 사용해야 한다, 병목현상도 걸릴 수 있음
atomic<int32> sum = 0;

void Add()
{
	for (int32 i = 0; i < 100'0000; i++)
	{
		sum.fetch_add(1);
		//sum++;
	}
}

void Sub()
{
	for (int32 i = 0; i < 100'0000; i++)
	{
		sum.fetch_add(-1);
		//sum--;
	}
}

int main()
{
	std::thread t1(Add);
	std::thread t2(Sub);
	t1.join();
	t2.join();
	cout << sum << endl;

}

