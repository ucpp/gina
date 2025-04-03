#ifndef _GINA_STRING_UTILS_H_
#define _GINA_STRING_UTILS_H_

#include <string>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

namespace gina
{
    class StringUtils
    {
    public:
        static std::string WideToUTF8(const std::wstring& wstr);
        static std::string WideToUTF8(const wchar_t* wstr);
        
        static std::wstring UTF8ToWide(const std::string& str);
        static std::wstring UTF8ToWide(const char* str);
    };
}

#endif // !_GINA_STRING_UTILS_H_