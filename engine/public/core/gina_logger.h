#ifndef _GINA_LOGGER_H_
#define _GINA_LOGGER_H_

#include <sstream>
#include <memory>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <iostream>

#include "core/gina_singleton.h"

namespace gina
{
    enum class LogLevel
    {
        Info,
        Warn,
        Error
    };

    class Logger final : public Singleton<Logger>
    {
        friend class Singleton<Logger>;

    public:
        void SetFileStream(const std::string& fileName = "log.txt");
        void SetConsoleStream(std::ostream& consoleStream = std::cout);

        template <typename... Args>
        void Log(LogLevel level, const std::string& format, Args... args)
        {
            try
            {
                std::ostringstream messageStream;
                FormatMessage(messageStream, format, args...);
                LogMessage(level, messageStream.str());
            }
            catch (...)
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                std::cerr << "[LOGGER ERROR] Failed to format log message" << std::endl;
            }
        }

    private:
        Logger();

        bool IsStreamValid() const;
        void LogMessage(LogLevel level, const std::string& message);
        std::string GetCurrentTime() const;
        std::string GetLevelString(LogLevel level) const;

        template <typename T, typename... Args>
        void FormatMessage(std::ostringstream& stream, const std::string& format, T&& value, Args&&... args)
        {
            size_t placeholderPos = format.find("{}");
            if (placeholderPos == std::string::npos)
            {
                stream << format;
                return;
            }

            stream << format.substr(0, placeholderPos);
            stream << std::forward<T>(value);
            FormatMessage(stream, format.substr(placeholderPos + 2), std::forward<Args>(args)...);
        }

        void FormatMessage(std::ostringstream& stream, const std::string& format)
        {
            stream << format;
        }

    private:
        std::unique_ptr<std::ostream> m_stream;
        std::mutex m_mutex;
        bool m_ownsStream = false;
    };

#ifdef _DEBUG
    #define LOG_INFO(msg, ...)  gina::Logger::Get().Log(gina::LogLevel::Info, msg, ##__VA_ARGS__)
    #define LOG_WARN(msg, ...)  gina::Logger::Get().Log(gina::LogLevel::Warn, msg, ##__VA_ARGS__)
    #define LOG_ERROR(msg, ...) gina::Logger::Get().Log(gina::LogLevel::Error, msg, ##__VA_ARGS__)
#else
    #define LOG_INFO(msg, ...)
    #define LOG_WARN(msg, ...)
    #define LOG_ERROR(msg, ...)
#endif
}

#endif // !_GINA_LOGGER_H_