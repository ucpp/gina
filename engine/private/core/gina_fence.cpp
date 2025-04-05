#include "core/gina_fence.h"

#include "core/gina_logger.h"
#include "core/gina_assert.h"

namespace gina
{
    Fence::~Fence()
    {
        if (m_eventHandle != nullptr)
        {
            CloseHandle(m_eventHandle);
            m_eventHandle = nullptr;
        }
    }

    void Fence::Initialize(ID3D12Device* device)
    {
        GINA_ASSERT_MSG(device != nullptr, "Invalid D3D12 device");

        HRESULT hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
        GINA_ASSERT_HRESULT(hr, "Failed to create D3D12 fence");
        Create();

        LOG_INFO("Fence initialized (current value: {})", m_currentValue);
    }

    uint64 Fence::Signal(ID3D12CommandQueue* commandQueue)
    {
        GINA_ASSERT_MSG(commandQueue != nullptr, "Invalid command queue");
        GINA_ASSERT_MSG(m_fence != nullptr, "Fence not initialized");

        const uint64 signalValue = ++m_currentValue;
        HRESULT hr = commandQueue->Signal(m_fence.Get(), signalValue);
        GINA_ASSERT_HRESULT(hr, "Failed to signal fence");

        return signalValue;
    }

    void Fence::WaitOnCPU(uint64 waitValue)
    {
        if (m_fence->GetCompletedValue() < waitValue)
        {
            HRESULT hr = m_fence->SetEventOnCompletion(waitValue, m_eventHandle);
            GINA_ASSERT_HRESULT(hr, "Failed to set fence event");

            WaitForSingleObject(m_eventHandle, INFINITE);
        }
    }

    void Fence::WaitOnGPU(ID3D12CommandQueue* commandQueue, uint64 waitValue)
    {
        GINA_ASSERT_MSG(commandQueue != nullptr, "Invalid command queue");
        HRESULT hr = commandQueue->Wait(m_fence.Get(), waitValue);
        GINA_ASSERT_HRESULT(hr, "Failed to wait on fence (GPU)");
    }

    uint64 Fence::GetCompletedValue() const
    {
        return m_fence->GetCompletedValue();
    }

    void Fence::Create()
    {
        m_eventHandle = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
        GINA_ASSERT_MSG(m_eventHandle != nullptr, "Failed to create fence event");
    }
}