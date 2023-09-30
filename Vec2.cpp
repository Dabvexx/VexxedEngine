#include "Vec2.h"

Vec2::Vec2(float _x = 0, float _y = 0)
    : x(_x), y(_y) { }

Vec2::Vec2()
{
    x = 0;
    y = 0;
}

Vec2 Vec2::Zero() { return Vec2(0, 0); }
Vec2 Vec2::Up() { return Vec2(0, 1); }
Vec2 Vec2::Down() { return Vec2(0, -1); }
Vec2 Vec2::Left() { return Vec2(-1, 0); }
Vec2 Vec2::Right() { return Vec2(1, 0); }
Vec2 Vec2::One() { return Vec2(1, 1); }

float Vec2::Magnitude() const { return sqrt(x * x + y * y); }
float Vec2::MagnitudeSquared() const { return x * x + y * y; }
float Vec2::Distance(const Vec2& v) const { return sqrt(((x - v.x) * (x - v.x)) + ((y - v.y) * (y - v.y))); }
float Vec2::DistanceSquared(const Vec2& v) const { return ((x - v.x) * (x - v.x)) + ((y - v.y) * (y - v.y)); }
float Vec2::Dot(const Vec2& v) const { return x * v.x + y * v.y; }
float Vec2::Cross(const Vec2& v) const { return x * v.y - y * v.x; }
Vec2& Vec2::Ortho() { *this = Vec2(0, 0); return *this; }

void Vec2::Normalize()
{
    float mag = Magnitude();

    if (mag < .00001)
    {
        x = y = 0;
    }

    x, y /= mag;
}

Vec2 Vec2::Normalized()
{
    float mag = Magnitude();

    if (mag < .00001)
    {
        x = y = 0;
        return *this;
    }

    return Vec2(x /= mag, y /= mag);
}

float Vec2::Distance(const Vec2& v1, const Vec2& v2) { return sqrt(((v1.x - v2.x) * (v1.x - v2.x)) + ((v1.y - v2.y) * (v1.y - v2.y))); }
float Vec2::DistanceSquared(const Vec2& v1, const Vec2& v2) { return ((v1.x - v2.x) * (v1.x - v2.x)) + ((v1.y - v2.y) * (v1.y - v2.y)); }
float Vec2::Dot(const Vec2& v1, const Vec2& v2) { return v1.x * v2.x + v1.y * v2.y; }
float Vec2::Cross(const Vec2& v1, const Vec2& v2) { return v1.x * v2.y - v1.y * v2.x; }