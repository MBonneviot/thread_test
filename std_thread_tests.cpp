#define BOOST_TEST_MODULE Thread_tests_suite
#include <boost/test/unit_test.hpp>

#include <mutex>
#include <thread>
#include <future>

namespace { // anonymous namespace
    long fibo(long value)
    {
        if (value == 0) return 0;
        if (value == 1) return 1;
        return fibo(value - 1) + fibo(value - 2);
    }
} // end anonymous namespace

BOOST_AUTO_TEST_SUITE(std_thread_tests)

BOOST_AUTO_TEST_CASE( basic_thread )
{
    long fibo20 = 0;
    auto threadFunction = [&fibo20](){fibo20 = fibo(20);};
    std::thread th (threadFunction);

    th.join();

    BOOST_CHECK_EQUAL(6765l, fibo20);
}

BOOST_AUTO_TEST_CASE( async_thread )
{
    auto future = std::async(std::launch::async, fibo, 20);

    BOOST_CHECK_EQUAL(6765l, future.get());
}
BOOST_AUTO_TEST_SUITE_END()
