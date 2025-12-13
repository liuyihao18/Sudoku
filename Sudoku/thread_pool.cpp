#include "stdafx.h"
#include "thread_pool.h"

ThreadPool::ThreadPool(size_t Capacity) : Capacity(Capacity), Stop(false)
{
    for (size_t i = 0; i < Capacity; i++)
    {
        Workers.emplace_back(
            [this, i]
            {
                while (true)
                {
                    std::function<void()> Task;
                    {
                        std::unique_lock<std::mutex> Lock(Mutex);
                        ConditionVariable.wait(Lock, [this]
                                               { return Stop || !Tasks.empty(); });
                        if (Stop && Tasks.empty())
                        {
                            return;
                        }
                        Task = std::move(Tasks.front());
                        Tasks.pop();
                    }
                    std::cout << "--- Worker " << i << " Start ---" << std::endl;
                    Task();
                    std::cout << "--- Worker " << i << " End ---" << std::endl;
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
    for (std::thread &Worker : Workers)
    {
        Worker.join();
    }
}
