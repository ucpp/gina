#include "core/gina_frame_timer.h"
#include "core/gina_assert.h"

namespace gina
{
    FrameTimer::FrameTimer() noexcept
    {
        const BOOL success = QueryPerformanceFrequency(&m_qpcFrequency);
        GINA_ASSERT_MSG(success, "High-performance counter not supported");
        Reset();
    }

    void FrameTimer::Reset() noexcept
    {
        QueryPerformanceCounter(&m_qpcLastTime);
    }

    double FrameTimer::GetElapsedSeconds() const noexcept
    {
        int64 currentTime;
        QueryPerformanceCounter(&currentTime);

        const auto timeDelta = currentTime.QuadPart - m_qpcLastTime.QuadPart;
        return static_cast<double>(timeDelta) / static_cast<double>(m_qpcFrequency.QuadPart);
    }

    double FrameTimer::GetElapsedMilliseconds() const noexcept
    {
        return GetElapsedSeconds() * 1000.0;
    }
}