#pragma once

class ThreadPool
{
public:
	static ThreadPool& GetInstance();

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;

	template <typename Func, typename... Args>
	decltype(auto) AddTask(Func&& func, Args&&... args);

private:
	explicit ThreadPool(size_t capacity);
	~ThreadPool();

	std::vector<std::jthread> Workers;
	std::queue<std::function<void()>> Tasks;
	std::mutex Mutex;
	std::condition_variable ConditionVariable;
	bool Stop{false};
};

template <typename Func, typename... Args>
decltype(auto) ThreadPool::AddTask(Func&& func, Args&&... args)
{
	using RetVal = std::invoke_result_t<Func, Args...>;
	std::shared_ptr task = std::make_shared<std::packaged_task<RetVal()>>(
		std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
	std::future<RetVal> result = task->get_future();
	{
		std::unique_lock lock(Mutex);
		if (Stop)
		{
			throw std::runtime_error("Enqueue a task on a stopped thread pool!");
		}
		Tasks.emplace([task]
		{
			(*task)();
		});
	}
	ConditionVariable.notify_one();
	return result;
}
