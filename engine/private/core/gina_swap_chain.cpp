#include "core/gina_swap_chain.h"

#include "core/gina_constants.h"
#include "core/gina_logger.h"
#include "core/gina_assert.h"

namespace gina
{
    void SwapChain::Initialize(HWND hwnd, uint32 width, uint32 height, bool vsyncEnabled, ComPtr<IDXGIFactory4> factory, ComPtr<ID3D12CommandQueue> commandQueue)
    {
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = width;
        swapChainDesc.Height = height;
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.Stereo = FALSE;
        swapChainDesc.SampleDesc = { 1, 0 };
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = BUFFER_COUNT;
        swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        swapChainDesc.Flags = vsyncEnabled ? 0 : DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

        ComPtr<IDXGISwapChain1> swapChain1;
        HRESULT hr = factory->CreateSwapChainForHwnd(commandQueue.Get(), hwnd, &swapChainDesc, nullptr, nullptr, &swapChain1);
        GINA_ASSERT_HRESULT(hr, "Failed to create swap chain");

        hr = swapChain1.As(&m_swapChain);
        GINA_ASSERT_HRESULT(hr, "Failed to cast swap chain to IDXGISwapChain3");

        LOG_INFO("Swap chain initialized ({}x{}, VSync: {})", width, height, vsyncEnabled ? "On" : "Off");
    }

    uint32 SwapChain::GetCurrentBackBufferIndex() const
    {
        return m_swapChain->GetCurrentBackBufferIndex();
    }
}