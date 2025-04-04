#include "core/gina_command_system.h"

#include "core/gina_logger.h"
#include "core/gina_assert.h"

namespace gina
{
    void CommandSystem::Initialize(ComPtr<ID3D12Device> device)
    {
        m_device = device;
        CreateCommandQueue();
        CreateCommandAllocators();
        CreateCommandList();
        
        LOG_INFO("Command system initialized successfully");
    }

    ComPtr<ID3D12CommandAllocator> CommandSystem::GetCommandAllocator(uint32 index) const
    {
        GINA_ASSERT_MSG(index < BUFFER_COUNT, "Command allocator index out of bounds");
        GINA_ASSERT_MSG(m_commandAllocators[index].Get() != nullptr, "Command allocator is not initialized");
        
        return m_commandAllocators[index];
    }

    void CommandSystem::CreateCommandQueue()
    {
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.NodeMask = 0;

        HRESULT hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
        GINA_ASSERT_HRESULT(hr, "Failed to create command queue");
    }

    void CommandSystem::CreateCommandAllocators()
    {
        for (uint32 i = 0; i < BUFFER_COUNT; ++i)
        {
            HRESULT hr = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocators[i]));
            GINA_ASSERT_HRESULT(hr, "Failed to create command allocator");
        }
    }

    void CommandSystem::CreateCommandList()
    {
        HRESULT hr = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators[0].Get(), nullptr, IID_PPV_ARGS(&m_commandList));
        GINA_ASSERT_HRESULT(hr, "Failed to create command list");

        hr = m_commandList->Close();
        GINA_ASSERT_HRESULT(hr, "Failed to close command list after creation");
    }

    void CommandSystem::ResetCommandList(uint32 frameIndex)
    {
        HRESULT hr = m_commandAllocators[frameIndex]->Reset();
        GINA_ASSERT_HRESULT(hr, "Failed to reset command allocator");

        hr = m_commandList->Reset(m_commandAllocators[frameIndex].Get(), nullptr);
        GINA_ASSERT_HRESULT(hr, "Failed to reset command list");
    }
}