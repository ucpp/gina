#ifndef _GINA_FENCE_H_
#define _GINA_FENCE_H_

#include <wrl/client.h>
#include <d3dx12.h>

#include "core/gina_types.h"
#include "core/gina_non_copyable.h"

using namespace Microsoft::WRL;

namespace gina
{
    class Fence final : public NonCopyable
    {
    public:
        Fence() = default;
        ~Fence();

        void Initialize(ID3D12Device* device);
        uint64 Signal(ID3D12CommandQueue* commandQueue);

        void WaitOnCPU(uint64 waitValue);
        void WaitOnGPU(ID3D12CommandQueue* commandQueue, uint64 waitValue);

        uint64 GetCompletedValue() const;
        uint64 GetCurrentValue() const { return m_currentValue; }

    private:
        void Create();

        ComPtr<ID3D12Fence> m_fence;
        uint64 m_currentValue = 0;
        HANDLE m_eventHandle = nullptr;
    };
}

#endif // !_GINA_FENCE_H_