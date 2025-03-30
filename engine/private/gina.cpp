#include "gina.h"

#include <iostream>
#include <imgui.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <json.hpp>

namespace gina
{
    int32_t Sum(int32_t lhs, int32_t rhs)
    {
        return lhs + rhs;
    }
}