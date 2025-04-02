#include "core/gina_frame_timer.h"
#include "core/gina_assert.h"

namespace gina
{
    FrameTimer::FrameTimer() noexcept
    {
        QueryPerformanceFrequency(&m_qpcFrequency);
        Reset();
    }

    void FrameTimer::Reset() noexcept
    {
        QueryPerformanceCounter(&m_qpcLastTime);
    }

    double FrameTimer::GetElapsedSeconds() const noexcept
    {
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);

        const auto timeDelta = currentTime.QuadPart - m_qpcLastTime.QuadPart;
        return static_cast<double>(timeDelta) / static_cast<double>(m_qpcFrequency.QuadPart);
    }

    double FrameTimer::GetElapsedMilliseconds() const noexcept
    {
        return GetElapsedSeconds() * 1000.0;
    }
}