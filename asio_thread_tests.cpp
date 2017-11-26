#include <boost/test/unit_test.hpp>

#include "ThreadPool.hpp"

#include <thread>
#include <iostream>

using namespace std::chrono_literals;

BOOST_AUTO_TEST_SUITE(asio_thread_tests)

BOOST_AUTO_TEST_CASE( pool_thread )
{
    ThreadPool pool (4);

    auto l = [](){std::cout << "--thread--" << std::endl;};

    pool.post(l);
    pool.post(l);
    pool.post(l);
    pool.post(l);
    pool.post(l);

    std::this_thread::sleep_for(10ms);
}

BOOST_AUTO_TEST_CASE( pool_thread_post_over )
{
    ThreadPool pool {4};

    int meaningOfLife = 0;

    auto longFunction = [](){std::this_thread::sleep_for(100ms);};
    auto shortFunction = [&meaningOfLife](){meaningOfLife = 42;};

    pool.post(longFunction);
    pool.post(longFunction);
    pool.post(longFunction);
    pool.post(longFunction);

    pool.post(shortFunction);

    std::this_thread::sleep_for(10ms);

    BOOST_CHECK_EQUAL(0, meaningOfLife);
}

BOOST_AUTO_TEST_CASE( pool_thread_post_in )
{
    ThreadPool pool(4);

    int meaningOfLife = 0;

    auto longFunction = [](){std::cout << "long" << std::endl; std::this_thread::sleep_for(100ms);};
    auto shortFunction = [&meaningOfLife](){std::cout << "short" << std::endl; meaningOfLife = 42;};

    pool.post(shortFunction);

    pool.post(longFunction);
    pool.post(longFunction);
    pool.post(longFunction);

    pool.post(shortFunction);

    pool.post(longFunction);

    std::this_thread::sleep_for(1000ms);

    BOOST_CHECK_EQUAL(42, meaningOfLife);
}

BOOST_AUTO_TEST_CASE( pool_thread_dispatch_run_first )
{
    ThreadPool pool {4};

    int meaningOfLife = 0;

    pool.post([&meaningOfLife](){meaningOfLife = 1;std::this_thread::sleep_for(10ms);});
    pool.post([&meaningOfLife](){meaningOfLife = 2;std::this_thread::sleep_for(10ms);});
    pool.post([&meaningOfLife](){meaningOfLife = 3;std::this_thread::sleep_for(10ms);});
    pool.post([&meaningOfLife](){meaningOfLife = 4;std::this_thread::sleep_for(10ms);});

    pool.post([&meaningOfLife](){meaningOfLife = 42;});
    pool.dispatch([&meaningOfLife](){meaningOfLife = 24;});

    std::this_thread::sleep_for(10ms);

    BOOST_CHECK_EQUAL(42, meaningOfLife);
}

BOOST_AUTO_TEST_SUITE_END()
