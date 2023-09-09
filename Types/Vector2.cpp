#include "Vector2.h"

Vector2::Vector2(float _x, float _y)
    : x(_x), y(_y) { }

Vector2 Vector2::Zero() { return Vector2(0, 0); }
Vector2 Vector2::Up() { return Vector2(0, 1); }
Vector2 Vector2::Down() { return Vector2(0, -1); }
Vector2 Vector2::Left() { return Vector2(-1, 0); }
Vector2 Vector2::Right() { return Vector2(1, 0); }
Vector2 Vector2::One() { return Vector2(1, 1); }

float Vector2::Magnitude() const { return sqrt(x * x + y * y); }
float Vector2::MagnitudeSquared() const { return x * x + y * y; }
float Vector2::Distance(const Vector2& v) const { return sqrt(((x - v.x) * (x - v.x)) + ((y - v.y) * (y - v.y))); }
float Vector2::DistanceSquared(const Vector2& v) const { return ((x - v.x) * (x - v.x)) + ((y - v.y) * (y - v.y)); }
float Vector2::Dot(const Vector2& v) const { return x * v.x + y * v.y; }
float Vector2::Cross(const Vector2& v) const { return x * v.y - y * v.x; }
Vector2& Vector2::Ortho() { *this = Vector2(0,0); return *this; }
void Vector2::Normalize()
{
    float mag = Magnitude();

    if (mag < .00001)
    {
        x = y = 0;
    }

    x, y /= mag;
}

Vector2 Vector2::Normalized()
{
    float mag = Magnitude();

    if (mag < .00001)
    {
        x = y = 0;
        return *this;
    }

    return Vector2(x /= mag, y /= mag);
}

float Vector2::Distance(const Vector2& v1, const Vector2& v2) { return sqrt(((v1.x - v2.x) * (v1.x - v2.x)) + ((v1.y - v2.y) * (v1.y - v2.y))); }
float Vector2::DistanceSquared(const Vector2& v1, const Vector2& v2) { return ((v1.x - v2.x) * (v1.x - v2.x)) + ((v1.y - v2.y) * (v1.y - v2.y)); }
float Vector2::Dot(const Vector2& v1, const Vector2& v2) { return v1.x * v2.x + v1.y * v2.y; }
float Vector2::Cross(const Vector2& v1, const Vector2& v2) { return v1.x * v2.y - v1.y * v2.x; }