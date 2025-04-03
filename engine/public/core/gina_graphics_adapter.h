#ifndef _GINA_GRAPHICS_ADAPTER_H_
#define _GINA_GRAPHICS_ADAPTER_H_

#include <dxgi.h>
#include <string>
#include <wrl/client.h>

using namespace Microsoft::WRL;

namespace gina
{
    class GraphicsAdapter
    {
    public:
        static ComPtr<IDXGIAdapter1> FindAdapter(bool highPerformance);
        static bool IsValidAdapter(ComPtr<IDXGIAdapter1>& adapter);
        static std::string GetAdapterDescription(ComPtr<IDXGIAdapter1>& adapter);
    };
}

#endif // !_GINA_GRAPHICS_ADAPTER_H_