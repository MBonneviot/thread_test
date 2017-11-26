#include <boost/test/unit_test.hpp>

#include <iostream>
#include <mutex>
#include <thread>
#include <future>
#include <condition_variable>

using namespace std::chrono_literals;

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

namespace { // anonymous namespace
    class Restaurant
    {
    public:
        void eat(const std::string& clientName)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            //std::cout << clientName << " waiting to eat, " << _nbrDish << " remaining." << std::endl;
            _condition.wait(lock, [this](){return _nbrDish > 0;});

            _nbrDish -= 1;
            //std::cout << clientName << " has eaten, " << _nbrDish << " remaining." << std::endl;
            
            _condition.notify_all();
        }

        void serve(std::size_t nbrDish)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            serviceStarted = true;
            _nbrDish += nbrDish;
            //std::cout << "server served 2 dishes, " << _nbrDish << " remaining." << std::endl;
            _condition.notify_all();
        }
    
        std::size_t nbrDish() 
        {
            std::unique_lock<std::mutex> lock(_mutex);
            return _nbrDish; 
        }

        void synchroNbrDishes(std::size_t nbrDishes)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _condition.wait_for(lock, 10ms, [this, nbrDishes](){return _nbrDish == nbrDishes && serviceStarted;});
        }

    private:
        std::size_t _nbrDish = 0;
        bool serviceStarted = false;

        std::mutex _mutex;
        std::condition_variable _condition;
    };
} // end anonymous namespace

BOOST_AUTO_TEST_CASE( condition_variable )
{
    Restaurant restaurant;

    auto server = [&restaurant](){restaurant.serve(2);};
    auto client = [&restaurant](const std::string& clientName){restaurant.eat(clientName);};

    std::thread client1(client, "client1");
    std::thread client2(client, "client2");
    std::thread client3(client, "client3");

    std::thread server1(server);
    restaurant.synchroNbrDishes(0);
    BOOST_CHECK_EQUAL(0, restaurant.nbrDish());

    std::thread server2(server);
    restaurant.synchroNbrDishes(1);
    BOOST_CHECK_EQUAL(1, restaurant.nbrDish());
    
    client1.join();
    client2.join();
    client3.join();

    server1.join();
    server2.join();

    BOOST_CHECK_EQUAL(1, restaurant.nbrDish());
}

BOOST_AUTO_TEST_SUITE_END()
