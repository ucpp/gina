#ifndef _GINA_ASSERT_H_
#define _GINA_ASSERT_H_

#include "core/gina_logger.h"

#ifdef _DEBUG
    #define GINA_ASSERT_MSG(expr, msg) \
        do { \
            if (!(expr)) { \
                gina::Logger::Get().Log(gina::LogLevel::Error, "Assert [{}] failed: {}", #expr, msg); \
                throw std::runtime_error(std::string("Assert: ") + #expr + " | " + msg); \
            } \
        } while(0)

    #define GINA_ASSERT_HRESULT(hr, context) \
        do { \
            HRESULT result = (hr); \
            if (FAILED(result)) { \
                gina::Logger::Get().Log(gina::LogLevel::Error, "HRESULT 0x{:08X} failed: {}", result, context); \
                throw std::runtime_error(std::string("HRESULT 0x") + std::to_string(result) + " | " + context); \
            } \
        } while(0)

    #define GINA_ASSERT(expr) GINA_ASSERT_MSG(expr, "")
#else
    #define GINA_ASSERT_MSG(expr, msg)
    #define GINA_ASSERT_HRESULT(hr, context)
    #define GINA_ASSERT(expr)
#endif

#endif // !_GINA_ASSERT_H_