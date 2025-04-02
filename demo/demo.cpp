#include "core/gina_window.h"
#include "core/gina_logger.h"
#include "core/gina_input.h"

using namespace gina;

int main()
{
    LOG_INFO("Starting Gina Animation demo");

    Window window;
    if (!window.Create("Gina Animation", 1280, 720))
    {
        LOG_ERROR("Failed to create window");
        return 1;
    }

    while (window.ProcessMessages())
    {
        if (Input::Get().GetKeyDown(VK_ESCAPE))
        {
            LOG_INFO("Escape pressed - exiting");
            break;
        }

        if (Input::Get().GetMouseButtonDown(MouseButton::Left))
        {
            LOG_INFO("Left mouse button clicked at ({}, {})", 
                   Input::Get().GetMouseX(), 
                   Input::Get().GetMouseY());
        }

        Input::Get().ResetStates();
    }

    LOG_INFO("Application shutdown");
    return 0;
}