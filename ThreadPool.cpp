#include "ThreadPool.hpp"

ThreadPool::ThreadPool(std::size_t poolSize)
    : _work(new boost::asio::io_service::work(_ioService))
{
    for (std::size_t i ; i < poolSize ; ++i)
    {
        _pool.create_thread([this](){_ioService.run();});
    }
}

ThreadPool::~ThreadPool()
{
    _work.reset();
    _ioService.stop();
    _pool.join_all();
}

void ThreadPool::post(const std::function<void()>& function)
{
    _ioService.post(function);
}

void ThreadPool::dispatch(const std::function<void()>& function)
{
    _ioService.dispatch(function);
}
