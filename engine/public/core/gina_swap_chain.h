#ifndef _GINA_SWAP_CHAIN_H_
#define _GINA_SWAP_CHAIN_H_

#include <wrl/client.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#include "core/gina_types.h"

using namespace Microsoft::WRL;

namespace gina
{
    class SwapChain
    {
    public:
        void Initialize(HWND hwnd, uint32 width, uint32 height, bool vsyncEnabled, ComPtr<IDXGIFactory4> factory, ComPtr<ID3D12CommandQueue> commandQueue);
        
        ComPtr<IDXGISwapChain3> GetSwapChain() const { return m_swapChain; }
        uint32 GetCurrentBackBufferIndex() const;

    private:
        ComPtr<IDXGISwapChain3> m_swapChain;
    };
}

#endif // !_GINA_SWAP_CHAIN_H_