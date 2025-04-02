#ifndef _GINA_INPUT_H_
#define _GINA_INPUT_H_

#include <array>
#include <bitset>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "core/gina_types.h"
#include "core/gina_singleton.h"

namespace gina
{
    enum class MouseButton
    {
        Left,
        Right
    };

    class Input : public Singleton<Input>
    {
        friend class Singleton<Input>;
        friend class Window;

    public:
        bool GetKey(uint32 keyCode) const noexcept { return m_keyStates.test(keyCode); }
        bool GetKeyDown(uint32 keyCode) const noexcept { return m_keyStates.test(keyCode) && !m_prevKeyStates.test(keyCode); }
        bool GetKeyUp(uint32 keyCode) const noexcept { return !m_keyStates.test(keyCode) && m_prevKeyStates.test(keyCode); }

        bool GetMouseButton(MouseButton button) const noexcept
        { 
            return m_mouseButtons.test(static_cast<size_t>(button)); 
        }

        bool GetMouseButtonDown(MouseButton button) const noexcept
        { 
            const auto idx = static_cast<size_t>(button);
            return m_mouseButtons.test(idx) && !m_prevMouseButtons.test(idx);
        }

        bool GetMouseButtonUp(MouseButton button) const noexcept
        { 
            const auto idx = static_cast<size_t>(button);
            return !m_mouseButtons.test(idx) && m_prevMouseButtons.test(idx);
        }

        int32 GetMouseX() const noexcept { return m_mouseX; }
        int32 GetMouseY() const noexcept { return m_mouseY; }
        int32 GetMouseWheelDelta() const noexcept { return m_mouseWheelDelta; }

        void ResetStates() noexcept;

    private:
        LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) noexcept;

        void KeyDown(uint32 keyCode) noexcept { m_keyStates.set(keyCode); }
        void KeyUp(uint32 keyCode) noexcept { m_keyStates.reset(keyCode); }

        void MouseMove(int32 xPos, int32 yPos) noexcept { m_mouseX = xPos; m_mouseY = yPos; }
        void MouseButtonDown(MouseButton button) noexcept { m_mouseButtons.set(static_cast<size_t>(button)); }
        void MouseButtonUp(MouseButton button) noexcept { m_mouseButtons.reset(static_cast<size_t>(button)); }
        void MouseWheel(int32 delta) noexcept { m_mouseWheelDelta += delta; }

    private:
        int32 m_mouseX = 0;
        int32 m_mouseY = 0;
        int32 m_mouseWheelDelta = 0;

        std::bitset<256> m_keyStates;
        std::bitset<256> m_prevKeyStates;
        std::bitset<2> m_mouseButtons;
        std::bitset<2> m_prevMouseButtons;
    };
}

#endif // !_GINA_INPUT_H_