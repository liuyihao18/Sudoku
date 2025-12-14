#include "stdafx.h"
#include "thread_pool.h"

ThreadPool& ThreadPool::GetInstance()
{
	static ThreadPool threadPool{8};
	return threadPool;
}

ThreadPool::ThreadPool(const size_t capacity)
{
	for (size_t i = 0; i < capacity; i++)
	{
		Workers.emplace_back(
			[this, i]
			{
				while (true)
				{
					std::function<void()> task;
					{
						std::unique_lock lock(Mutex);
						ConditionVariable.wait(lock, [this]
						{
							return Stop || !Tasks.empty();
						});
						if (Stop && Tasks.empty())
						{
							return;
						}
						task = std::move(Tasks.front());
						Tasks.pop();
					}
#ifdef _DEBUG
					std::ostringstream os;
					os << "* Worker "sv << i << " Start"sv << std::endl;
					std::cout << os.str();
#endif
					task();
#ifdef _DEBUG
					os.str("");
					os << "* Worker "sv << i << " End"sv << std::endl;
					std::cout << os.str();
#endif
				}
			});
	}
}

ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(Mutex);
		Stop = true;
	}
	ConditionVariable.notify_all();
}
