#ifndef _GINA_FRAME_TIMER_H_
#define _GINA_FRAME_TIMER_H_

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace gina
{
    class FrameTimer final
    {
    public:
        FrameTimer() noexcept;
        
        void Reset() noexcept;
        double GetElapsedSeconds() const noexcept;
        double GetElapsedMilliseconds() const noexcept;

    private:
        LARGE_INTEGER m_qpcFrequency;
        LARGE_INTEGER m_qpcLastTime;
    };
}

#endif // !_GINA_FRAME_TIMER_H_