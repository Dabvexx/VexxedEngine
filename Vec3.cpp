#include "Vec3.h"
#include <algorithm> 

Vec3::Vec3(float _x, float _y, float _z)
    : x(_x), y(_y), z(_z) { }

// Static Properties.
Vec3 Vec3::Zero() { return Vec3(0, 0, 0); }
Vec3 Vec3::Up() { return Vec3(0, 1, 0); }
Vec3 Vec3::Down() { return Vec3(0, -1, 0); }
Vec3 Vec3::Left() { return Vec3(-1, 0, 0); }
Vec3 Vec3::Right() { return Vec3(1, 0, 0); }
Vec3 Vec3::Forward() { return Vec3(0, 0, 1); }
Vec3 Vec3::Back() { return Vec3(0, 0, -1); }
Vec3 Vec3::One() { return Vec3(1, 1, 1); }

float Vec3::Magnitude() const { return sqrt(x * x + y * y + z * z); }
float Vec3::MagnitudeSquared() const { return x * x + y * y + z * z; }
Vec3 Vec3::Normalized()
{
    float mag = Magnitude();

    if (mag < .00001)
    {
        return Vec3(0, 0, 0);
    }

    return Vec3(x / mag, y / mag, z / mag);
}

void Vec3::Normalize()
{
    float mag = Magnitude();

    if (mag < .00001)
    {
        x, y, z = 0;
    }

    x, y, z /= mag;
}

void Vec3::ClampMagnitude(float maxLength) {}

void Vec3::ClampMagnitude(Vec3* v, float maxLength) {} // <-- TODO:
float Vec3::Distance(const Vec3& v1, const Vec3& v2) { return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2)); }
float Vec3::DistanceSquared(const Vec3& v1, const Vec3& v2) { return (pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2)); }
float Vec3::Dot(const Vec3& v1, const Vec3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
Vec3 Vec3::Cross(const Vec3& v1, const Vec3& v2) { return Vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y * v1.y * v2.x); }
//Vec3 Vec3::Lerp(const Vec3& v1, const Vec3& v2, float t) { return v1 + (v2 - v1) * std::clamp(t, 0.0f, 1.0f); }
Vec3 Vec3::LerpUnclamped(const Vec3& v1, const Vec3& v2, float t) { return v1 + (v2 - v1) * t; }
float Vec3::Angle(const Vec3& from, const Vec3& to) { return acos(Vec3::Dot(from, to) / (from.Magnitude() * to.Magnitude())); }
Vec3 Vec3::Max(const Vec3& v1, const Vec3& v2) { return Vec3(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z)); }
Vec3 Vec3::Min(const Vec3& v1, const Vec3& v2) { return Vec3(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z)); }