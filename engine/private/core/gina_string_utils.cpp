#include "core/gina_string_utils.h"

namespace gina
{
    std::string StringUtils::WideToUTF8(const std::wstring& wstr)
    {
        return WideToUTF8(wstr.c_str());
    }

    std::string StringUtils::WideToUTF8(const wchar_t* wstr)
    {
        if (!wstr || *wstr == L'\0') return {};
        
        int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
        if (sizeNeeded <= 0) return {};
        
        std::string result(sizeNeeded, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &result[0], sizeNeeded, nullptr, nullptr);
        result.resize(sizeNeeded - 1);
        
        return result;
    }

    std::wstring StringUtils::UTF8ToWide(const std::string& str)
    {
        return UTF8ToWide(str.c_str());
    }

    std::wstring StringUtils::UTF8ToWide(const char* str)
    {
        if (!str || *str == '\0') return {};
        
        int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
        if (sizeNeeded <= 0) return {};
        
        std::wstring result(sizeNeeded, 0);
        MultiByteToWideChar(CP_UTF8, 0, str, -1, &result[0], sizeNeeded);
        result.resize(sizeNeeded - 1);
        
        return result;
    }
}