#include "core/gina_graphics_adapter.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <Windows.h>

#include "core/gina_logger.h"
#include "core/gina_types.h"
#include "core/gina_string_utils.h"

namespace gina
{
    ComPtr<IDXGIAdapter1> GraphicsAdapter::FindAdapter(bool highPerformance)
    {
        ComPtr<IDXGIFactory4> factory;
        HRESULT hr = CreateDXGIFactory2(0, IID_PPV_ARGS(&factory));
        if (FAILED(hr))
        {
            LOG_ERROR("Failed to create DXGI factory for adapter search. HRESULT: 0x{:08X}", hr);
            return nullptr;
        }

        ComPtr<IDXGIAdapter1> adapter;
        ComPtr<IDXGIFactory6> factory6;
        
        if (SUCCEEDED(factory->QueryInterface(IID_PPV_ARGS(&factory6))))
        {
            DXGI_GPU_PREFERENCE gpuPreference = highPerformance ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED;
            for (uint32 index = 0; SUCCEEDED(factory6->EnumAdapterByGpuPreference(index, gpuPreference, IID_PPV_ARGS(&adapter))); ++index)
            {
                if (IsValidAdapter(adapter))
                {
                    LOG_INFO("Found suitable adapter: {}", GetAdapterDescription(adapter));
                    return adapter;
                }
            }
        }

        for (uint32 index = 0; SUCCEEDED(factory->EnumAdapters1(index, &adapter)); ++index)
        {
            if (IsValidAdapter(adapter))
            {
                LOG_INFO("Found suitable adapter: {}", GetAdapterDescription(adapter));
                return adapter;
            }
        }

        LOG_ERROR("No suitable graphics adapter found");
        
        return nullptr;
    }

    bool GraphicsAdapter::IsValidAdapter(ComPtr<IDXGIAdapter1>& adapter)
    {
        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
        {
            LOG_WARN("Skipping software adapter: {}", desc.Description);
            return false;
        }

        HRESULT hr = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
        if (FAILED(hr))
        {
            LOG_WARN("Adapter not compatible with D3D12: {}, HRESULT: 0x{:08X}", desc.Description, hr);
            return false;
        }

        return true;
    }

    std::string GraphicsAdapter::GetAdapterDescription(ComPtr<IDXGIAdapter1>& adapter)
    {
        if (!adapter)
        {
            LOG_WARN("Invalid adapter pointer");
            return {};
        }

        DXGI_ADAPTER_DESC1 desc;
        if (FAILED(adapter->GetDesc1(&desc)))
        {
            LOG_ERROR("Failed to get adapter description");
            return {};
        }

        return StringUtils::WideToUTF8(desc.Description);
    }
}