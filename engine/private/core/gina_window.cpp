#include "core/gina_window.h"

#include "core/gina_input.h"
#include "core/gina_logger.h"

namespace gina
{
    Window::~Window()
    {
        Shutdown();
    }

    bool Window::Create(const std::string& title, int32 width, int32 height, bool fullscreen)
    {
        if (m_hwnd) 
        {
            LOG_WARN("Window already created");
            return false;
        }

        m_title = std::wstring(title.begin(), title.end());
        m_fullscreen = fullscreen;

        if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2))
        {
            LOG_ERROR("Failed to set DPI awareness");
        }

        RegisterWindowClass();
        if (!CreateWindowHandle(width, height))
        {
            return false;
        }

        RetrieveClientSize();
        SetFullscreen(fullscreen);

        LOG_INFO("Window created: {} ({}x{})", title, width, height);
        return true;
    }

    bool Window::ProcessMessages()
    {
        MSG msg = {};
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                return false;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return true;
    }

    void Window::SetWindowSize(int32 width, int32 height)
    {
        if (width <= 0 || height <= 0 || !m_hwnd)
        {
            return;
        }

        if (width == m_width && height == m_height)
        {
            return;
        }

        SaveWindowPosition();

        RECT windowRect = { 0, 0, width, height };
        AdjustWindowRect(&windowRect, GetWindowLong(m_hwnd, GWL_STYLE), FALSE);

        SetWindowPos(m_hwnd, nullptr, 
            m_windowPosition.left, m_windowPosition.top,
            windowRect.right - windowRect.left, 
            windowRect.bottom - windowRect.top,
            SWP_NOZORDER | SWP_FRAMECHANGED);

        RetrieveClientSize();
        m_resizeAction.Invoke(m_width, m_height);
    }

    void Window::SetFullscreen(bool fullscreen)
    {
        if (m_fullscreen == fullscreen || !m_hwnd)
        {
            return;
        }
        
        m_fullscreen = fullscreen;

        if (fullscreen)
        {
            SaveWindowPosition();
            EnableFullscreen();
        }
        else
        {
            DisableFullscreen();
            RestoreWindowPosition();
        }
    }
    
    void Window::RegisterWindowClass()
    {
        WNDCLASSEX wc = {};
        m_hinstance = GetModuleHandle(nullptr);
        
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = m_hinstance;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.lpszClassName = m_title.c_str();

        if (!RegisterClassEx(&wc))
        {
            LOG_ERROR("Failed to register window class");
        }
    }

    bool Window::CreateWindowHandle(int32 width, int32 height)
    {
        DWORD style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;
        DWORD exStyle = WS_EX_APPWINDOW;

        RECT windowRect = { 0, 0, width, height };
        AdjustWindowRect(&windowRect, style, FALSE);

        m_hwnd = CreateWindowEx(
            exStyle, m_title.c_str(), m_title.c_str(), style,
            CW_USEDEFAULT, CW_USEDEFAULT,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            nullptr, nullptr, m_hinstance, this);

        if (!m_hwnd)
        {
            LOG_ERROR("Failed to create window");
            return false;
        }

        ShowWindow(m_hwnd, SW_SHOW);
        UpdateWindow(m_hwnd);
        return true;
    }

    void Window::RetrieveClientSize()
    {
        RECT clientRect;
        GetClientRect(m_hwnd, &clientRect);
        m_width = clientRect.right;
        m_height = clientRect.bottom;
    }

    void Window::EnableFullscreen()
    {
        DEVMODE dmSettings = {};
        dmSettings.dmSize = sizeof(dmSettings);
        dmSettings.dmPelsWidth = m_width;
        dmSettings.dmPelsHeight = m_height;
        dmSettings.dmBitsPerPel = 32;
        dmSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        ChangeDisplaySettings(&dmSettings, CDS_FULLSCREEN);
        
        SetWindowLong(m_hwnd, GWL_STYLE, GetWindowLong(m_hwnd, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
        SetWindowPos(m_hwnd, HWND_TOPMOST, 0, 0, m_width, m_height, SWP_FRAMECHANGED);
        
        ShowCursor(FALSE);
    }

    void Window::DisableFullscreen()
    {
        ChangeDisplaySettings(nullptr, 0);
        
        SetWindowLong(m_hwnd, GWL_STYLE, GetWindowLong(m_hwnd, GWL_STYLE) | WS_OVERLAPPEDWINDOW);
        SetWindowPos(m_hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
        
        ShowCursor(TRUE);
    }

    void Window::DestroyWindowHandle()
    {
        if (m_hwnd)
        {
            DestroyWindow(m_hwnd);
            m_hwnd = nullptr;
        }
    }

    void Window::UnregisterWindowClass()
    {
        if (m_hinstance && !m_title.empty())
        {
            UnregisterClass(m_title.c_str(), m_hinstance);
        }
    }

    void Window::Shutdown()
    {
        if (m_fullscreen)
        {
            DisableFullscreen();
        }

        DestroyWindowHandle();
        UnregisterWindowClass();
        m_resizeAction.Clear();
    }

    LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
    {
        switch (msg)
        {
        case WM_CREATE:
        {
            auto createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));
            return 0;
        }
        
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
            
        case WM_SIZE:
        {
            auto window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            if (window)
            {
                window->m_width = LOWORD(lParam);
                window->m_height = HIWORD(lParam);
                window->m_resizeAction.Invoke(window->m_width, window->m_height);
            }
            return 0;
        }
        
        default:
        {
            auto window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            if (window)
            {
                return Input::Get().MessageHandler(hwnd, msg, wParam, lParam);
            }
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        }
    }

    void Window::SaveWindowPosition()
    {
        if (m_hwnd)
        {
            GetWindowRect(m_hwnd, &m_windowPosition);
        }
    }

    void Window::RestoreWindowPosition()
    {
        if (m_hwnd && (m_windowPosition.left != 0 || m_windowPosition.top != 0))
        {
            SetWindowPos(m_hwnd, nullptr, 
                m_windowPosition.left, m_windowPosition.top,
                m_windowPosition.right - m_windowPosition.left,
                m_windowPosition.bottom - m_windowPosition.top,
                SWP_NOZORDER | SWP_FRAMECHANGED);
        }
    }
}