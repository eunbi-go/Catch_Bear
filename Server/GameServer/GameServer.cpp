#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <future>

//atomic<bool> flag;

int main()
{
	//flag = false;

	//// flag = true;
	//flag.store(true, memory_order::memory_order_seq_cst);

	//// bool val = flag;
	//bool val = flag.load(memory_order::memory_order_seq_cst);

	//// 이전 flag값을 prev에 넣고, flag값을 수정
	//{
	//	bool prev = flag.exchange(true);
	//	// 밑에 이거처럼 하면 원자적이지 않으므로 다른 스레드에서 공용 데이터를 잘못 건드릴수있음
	//	//bool prev = flag;
	//	//flag = true;
	//}

	//// CAS 조건부 수정
	//{
	//	bool expected = false;
	//	bool desired = true;
	//	flag.compare_exchange_strong(expected, desired);
	//}

	// Memory Model (정책)
	// 1) Sequentially Consistent (seq_cst)
	// 2) Acquire-Release (acquire, release)
	// 3) Relaxed (relaxed)

	// 1) sez_cst (가장 엄격 = 컴파일러 최적화 여지 적음 = 직관적)
	// 2) acquire-release
	// 3) relaxed (자유롭다 = 컴파일러 최적화 여지 많음 = 직관적이지 않음)
}

