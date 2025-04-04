#ifndef _GINA_DEVICE_H_
#define _GINA_DEVICE_H_

#include "core/gina_command_system.h"
#include "core/gina_swap_chain.h"
#include "core/gina_fence.h"
#include "core/gina_types.h"

namespace gina
{
    class Device
    {
    public:
        void Initialize(HWND hwnd, uint32 width, uint32 height, bool vsyncEnabled = true);

        ComPtr<ID3D12Device> GetDevice() const { return m_device; }
        CommandSystem& GetCommandSystem() { return m_commandSystem; }
        SwapChain& GetSwapChain() { return m_swapChain; }
        void FlushCommandQueue();

    private:
        void EnableDebugLayer();
        void CreateDXGIFactory();
        void CreateGraphicsDevice();

    private:
        ComPtr<ID3D12Device> m_device;
        ComPtr<IDXGIFactory4> m_dxgiFactory;
        CommandSystem m_commandSystem;
        SwapChain m_swapChain;
        Fence m_fence;
        uint32 m_dxgiFactoryCreationFlags = 0;
    };
}

#endif // !_GINA_DEVICE_H_