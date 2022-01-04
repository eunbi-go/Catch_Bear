#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <future>

int64 Calculate()
{
	int64 sum = 0;
	
	for (int32 i = 0; i < 100'000; i++)
		sum += i;

	return sum;
}

void PromiseWorker(std::promise<string>&& promise)
{
	promise.set_value("msg");
}

void TaskWorker(std::packaged_task<int64(void)>&& task)
{
	task();
}

int main()
{
	// 동기 실행
	//int64 sum = Calculate();
	//cout << sum << endl;
	
	// std::future
	{
		// 1) deferred -> 지연해서 실행하세요 (Calculate를 나중에 호출하는거랑 별반 다를게 없다) -> 요청하는 시점과 호출하는 시점이 다르므로 비동기
		// 2) async -> 별도의 쓰레드를 만들어서 실행하세요
		// 3) deferred | async -> 둘 중 알아서 골라주세요

		// 언젠가 미래에 결과물을 뱉어줄거야!
		std::future<int64> future = std::async(std::launch::async, Calculate);
		
		// TODO


		// 결과물이 궁금할때 비로소 이걸 호출해서 확인함
		int64 sum = future.get();
	}

	// std::promise
	{
		// 미래(std::future)에 결과물을 반환해줄거라 약속(std::promise) (계약서?)
		std::promise<string> promise;
		std::future<string> future = promise.get_future();

		thread t(PromiseWorker, std::move(promise));

		string msg = future.get();
		cout << msg << endl;

		t.join();
	}

	// std::packaged_task
	{
		std::packaged_task<int64(void)> task(Calculate);
		std::future<int64> future = task.get_future();

		std::thread t(TaskWorker, std::move(task));

		int64 sum = future.get();
		cout << sum << endl;

		t.join();
	}

	// 결론)
	// mutex, condition_variable까지 가지 않고 단순한 애들을 처리하기 좋다
	// 특히나 한번 발생하는 이벤트에 유용하다
	// 닭잡는데 소잡는 칼 쓸 필요 없다
	// 1) async
	// 원하는 함수를 비동기적으로 실행
	// 2) promise
	// 결과물을 promise를 통해 future로 받아줌
	// 3) package_task
	// 원하는 함수의 실행 결과를 package_task를 통해 future로 받아줌 
}

