#include "core/gina_device.h"

#include "core/gina_logger.h"
#include "core/gina_assert.h"
#include "core/gina_graphics_adapter.h"

namespace gina
{
    void Device::Initialize(HWND hwnd, uint32 width, uint32 height, bool vsyncEnabled)
    {
        LOG_INFO("Initializing graphics device...");

        HRESULT hr = DXGIDeclareAdapterRemovalSupport();
        GINA_ASSERT_HRESULT(hr, "Failed to declare adapter removal support");

        EnableDebugLayer();
        CreateDXGIFactory();
        CreateGraphicsDevice();
        
        m_commandSystem.Initialize(m_device);
        m_swapChain.Initialize(hwnd, width, height, vsyncEnabled, m_dxgiFactory, m_commandSystem.GetCommandQueue());
        m_fence.Initialize(m_device.Get());

        LOG_INFO("Graphics device initialized successfully");
    }

    void Device::EnableDebugLayer()
    {
#if defined(_DEBUG)
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();
            m_dxgiFactoryCreationFlags |= DXGI_CREATE_FACTORY_DEBUG;
            LOG_INFO("D3D12 debug layer enabled");

            ComPtr<ID3D12Debug1> debugController1;
            if (SUCCEEDED(debugController.As(&debugController1)))
            {
                debugController1->SetEnableGPUBasedValidation(TRUE);
                LOG_INFO("GPU-based validation enabled");
            }
        }
        else
        {
            LOG_WARN("Failed to initialize D3D12 debug layer");
        }
#endif
    }
    
    void Device::CreateDXGIFactory()
    {
        HRESULT hr = CreateDXGIFactory2(m_dxgiFactoryCreationFlags, IID_PPV_ARGS(&m_dxgiFactory));
        GINA_ASSERT_HRESULT(hr, "Failed to create DXGI factory");
    }
    
    void Device::CreateGraphicsDevice()
    {
        if (USE_WARP_DEVICE)
        {
            LOG_INFO("Creating WARP software device");
            ComPtr<IDXGIAdapter> warpAdapter;
            HRESULT hr = m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter));
            GINA_ASSERT_HRESULT(hr, "Failed to enumerate WARP adapter");
            
            hr = D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));
            GINA_ASSERT_HRESULT(hr, "Failed to create D3D12 device with WARP adapter");
        }
        else
        {
            LOG_INFO("Creating hardware accelerated device");
            ComPtr<IDXGIAdapter1> hardwareAdapter = GraphicsAdapter::FindAdapter(true);
            GINA_ASSERT_MSG(hardwareAdapter.Get(), "Failed to find a suitable hardware adapter");
            
            HRESULT hr = D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));
            GINA_ASSERT_HRESULT(hr, "Failed to create D3D12 device with hardware adapter");
        }

        LOG_INFO("D3D12 device created successfully");
    }

    void Device::FlushCommandQueue()
    {
        const uint64 fenceValue = m_fence.Signal(m_commandSystem.GetCommandQueue().Get());
        m_fence.WaitOnCPU(fenceValue);
    }
}