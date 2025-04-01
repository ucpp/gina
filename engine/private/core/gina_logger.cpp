#include "core/gina_logger.h"

#include <fstream>

namespace gina
{
    Logger::Logger() : m_stream(std::make_unique<std::ofstream>("log.txt")), m_ownsStream(true)
    {
        if (!IsStreamValid())
        {
            m_stream = std::make_unique<std::ostream>(std::cout.rdbuf());
            m_ownsStream = false;
        }
    }

    void Logger::SetFileStream(const std::string& fileName)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        if (m_ownsStream)
        {
            m_stream.reset();
        }

        m_stream = std::make_unique<std::ofstream>(fileName);
        m_ownsStream = true;

        if (!IsStreamValid())
        {
            m_stream = std::make_unique<std::ostream>(std::cout.rdbuf());
            m_ownsStream = false;
            LOG_ERROR("Failed to open log file '{}', falling back to console", fileName);
        }
    }

    void Logger::SetConsoleStream(std::ostream& consoleStream)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        if (m_ownsStream)
        {
            m_stream.reset();
        }

        m_stream = std::make_unique<std::ostream>(consoleStream.rdbuf());
        m_ownsStream = false;
    }

    void Logger::LogMessage(LogLevel level, const std::string& message)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (!IsStreamValid())
        {
            std::cerr << "[LOGGER ERROR] Failed to write log message. Stream is not valid." << std::endl;
            return;
        }

        *m_stream << GetCurrentTime() << " " << GetLevelString(level) << " " << message << std::endl;
    }

    bool Logger::IsStreamValid() const
    {
        return m_stream && *m_stream;
    }

    std::string Logger::GetCurrentTime() const
    {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);

        std::tm localTime = {};
#ifdef _MSC_VER
        localtime_s(&localTime, &time);
#else
        std::tm* tmPtr = std::localtime(&time);
        if (!tmPtr)
        {
            return "[TIME_ERROR]";
        }
        localTime = *tmPtr;
#endif

        std::ostringstream timeStream;
        timeStream << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
        return timeStream.str();
    }

    std::string Logger::GetLevelString(LogLevel level) const
    {
        switch (level)
        {
            case LogLevel::Info:  return "[INFO]";
            case LogLevel::Warn:  return "[WARN]";
            case LogLevel::Error: return "[ERROR]";
            default:              return "[UNKNOWN]";
        }
    }
}