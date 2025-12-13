#pragma once
class ThreadPool
{
public:
    ThreadPool(size_t Capacity);
    ~ThreadPool();
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;
    ThreadPool &operator=(ThreadPool &&) = delete;

    template <typename Func, typename... Args>
    decltype(auto) AddTask(Func &&func, Args &&...args);

private:
    size_t Capacity;
    std::vector<std::jthread> Workers;
    std::queue<std::function<void()>> Tasks;
    std::mutex Mutex;
    std::condition_variable ConditionVariable;
    bool Stop;
};

template <typename Func, typename... Args>
inline decltype(auto) ThreadPool::AddTask(Func &&func, Args &&...args)
{
    using RetVal = typename std::invoke_result_t<Func, Args...>;
    std::shared_ptr Task = std::make_shared<std::packaged_task<RetVal()>>(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
    std::future<RetVal> Result = Task->get_future();
    {
        std::unique_lock<std::mutex> Lock(Mutex);
        if (Stop)
        {
            throw std::runtime_error("Enqueue a task on a stopped thread pool!");
        }
        Tasks.emplace([Task]
                      { (*Task)(); });
    }
    ConditionVariable.notify_one();
    return Result;
}
