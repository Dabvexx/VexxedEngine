#include "Transform2D.h"

// Transforms will be based relative to the bottom left corner of the screen.
struct Transform2D {
    Vector2 pos;
    Vector2 scale;
    float rotation;

    Transform2D::Transform2D(Vector2 _pos, Vector2 _scale, float _rotation) 
        : pos(_pos), scale(_scale), rotation(_rotation) { }

        
};