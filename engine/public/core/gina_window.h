#ifndef _GINA_WINDOW_H_
#define _GINA_WINDOW_H_

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

#include "core/gina_action.h"
#include "core/gina_types.h"

namespace gina
{
    class Window
    {
    public:
        Window() = default;
        ~Window();

        bool Create(const std::string& title, int32 width, int32 height, bool fullscreen = false);
        bool ProcessMessages();

        void SetWindowSize(int32 width, int32 height);
        void SetFullscreen(bool fullscreen);

        HWND GetHandle() const noexcept { return m_hwnd; }
        int32 GetWidth() const noexcept { return m_width; }
        int32 GetHeight() const noexcept { return m_height; }
        bool IsFullscreen() const noexcept { return m_fullscreen; }

        Action<uint32, uint32>& OnResize() noexcept { return m_resizeAction; }

    private:
        void RegisterWindowClass();
        bool CreateWindowHandle(int32 width, int32 height);
        void RetrieveClientSize();
        void EnableFullscreen();
        void DisableFullscreen();

        void DestroyWindowHandle();
        void UnregisterWindowClass();
        void Shutdown();

        void SaveWindowPosition();
        void RestoreWindowPosition();

        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) noexcept;

    private:
        std::wstring  m_title;
        int32         m_width = 0;
        int32         m_height = 0;
        bool          m_fullscreen = false;
        HINSTANCE     m_hinstance = nullptr;
        HWND          m_hwnd = nullptr;
        RECT          m_windowPosition = {};
        RECT          m_monitorRect = {};

        Action<uint32, uint32> m_resizeAction;
    };
}
#endif // !_GINA_WINDOW_H_