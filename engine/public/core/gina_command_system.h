#ifndef _GINA_COMMAND_SYSTEM_H_
#define _GINA_COMMAND_SYSTEM_H_

#include <d3d12.h>
#include <wrl/client.h>
#include "core/gina_types.h"
#include "core/gina_constants.h"

using namespace Microsoft::WRL;

namespace gina
{
    class CommandSystem
    {
    public:
        void Initialize(ComPtr<ID3D12Device> device);
        
        ComPtr<ID3D12CommandQueue> GetCommandQueue() const { return m_commandQueue; }
        ComPtr<ID3D12CommandAllocator> GetCommandAllocator(uint32 index) const;
        ComPtr<ID3D12GraphicsCommandList> GetCommandList() const { return m_commandList; }

        void ResetCommandList(uint32 frameIndex);

    private:
        void CreateCommandQueue();
        void CreateCommandAllocators();
        void CreateCommandList();

    private:
        ComPtr<ID3D12Device> m_device;
        ComPtr<ID3D12CommandQueue> m_commandQueue;
        ComPtr<ID3D12GraphicsCommandList> m_commandList;
        ComPtr<ID3D12CommandAllocator> m_commandAllocators[BUFFER_COUNT];
    };
}

#endif // !_GINA_COMMAND_SYSTEM_H_