#pragma once

#include <memory>
#include <string>

namespace ArcEngine
{
    enum LogLevel
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    };

    class Logger
    {
    public:
        Logger(const std::string& _FilePath);
        ~Logger();

        void Log();

    private:

    };
}