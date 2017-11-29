#include <boost/test/unit_test.hpp>

#include <boost/di.hpp>

#include <memory>
#include <iostream>

namespace di = boost::di;

namespace {
    enum class LoggerType
    {
        DATABASE,
        CONSOLE
    };
    std::ostream& operator<<(std::ostream& os, LoggerType lt)
    {
        switch (lt)
        {
            case LoggerType::CONSOLE: os << "CONSOLE"; break;
            case LoggerType::DATABASE: os << "DATABASE"; break;
        }
        return os;
    }

    struct Logger
    {
        virtual LoggerType type() const = 0;
    };
    struct DBLogger : public Logger
    {
        virtual LoggerType type() const override {return LoggerType::DATABASE;}
    };
    struct ConsoleLogger : public Logger
    {
        virtual LoggerType type() const override {return LoggerType::CONSOLE;}
    };

    struct TestLoggerInjection
    {
        std::unique_ptr<Logger> logger;
    };
}

BOOST_AUTO_TEST_SUITE(asio_thread_tests)

BOOST_AUTO_TEST_CASE( di_test_dblogger )
{
    const auto injector = di::make_injector(
        di::bind<Logger>().to<DBLogger>());
    
    auto testLogger = injector.create<TestLoggerInjection>();

    BOOST_CHECK_EQUAL(LoggerType::DATABASE, testLogger.logger->type());
}

BOOST_AUTO_TEST_CASE( di_test_dbconsole )
{
    const auto injector = di::make_injector(
        di::bind<Logger>().to<ConsoleLogger>());
    
    auto testLogger = injector.create<TestLoggerInjection>();

    BOOST_CHECK_EQUAL(LoggerType::CONSOLE, testLogger.logger->type());
}

BOOST_AUTO_TEST_SUITE_END()
