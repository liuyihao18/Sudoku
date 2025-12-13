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
#ifdef _DEBUG
                    std::ostringstream os;
                    os << "* Worker " << i << " Start" << std::endl;
                    std::cout << os.str();
#endif
                    Task();
#ifdef _DEBUG
                    os.str("");
                    os << "* Worker " << i << " End" << std::endl;
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
    for (std::thread &Worker : Workers)
    {
        Worker.join();
    }
    std::cout << std::endl;
}
