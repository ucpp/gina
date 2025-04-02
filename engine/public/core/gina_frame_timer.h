#ifndef _GINA_FRAME_TIMER_H_
#define _GINA_FRAME_TIMER_H_

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "core/gina_types.h"

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
        int64 m_qpcFrequency;
        int64 m_qpcLastTime;
    };
}

#endif // !_GINA_FRAME_TIMER_H_