#ifndef TRANSFORM_2D
#define TRANSFORM_2D

#include "Vector2.h"

namespace VexxedEngine::Components {

    struct Transform2D
    {
        Vector2 pos;
        Vector2 scale;
        float rotation;

        // Constructors.
        Transform2D(Vector2 pos = Vector2::Zero(), Vector2 scale = Vector2(1,1), float rotation = 0);
    };
}
#endif