#pragma once

#include <boost/core/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include <functional>
#include <memory>

class ThreadPool : private boost::noncopyable
{
public:
    ThreadPool(std::size_t poolSize);
    ~ThreadPool();

    void post(const std::function<void()>& function);
    void dispatch(const std::function<void()>& function);

private:
    boost::asio::io_service _ioService;
    std::unique_ptr<boost::asio::io_service::work> _work;
    boost::thread_group _pool;
};