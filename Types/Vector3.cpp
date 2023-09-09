#include "Vector3.h"

Vector3::Vector3(float _x, float _y, float _z)
    : x(_x), y(_y), z(_z) { }

// Static Properties.
Vector3 Vector3::Zero() { return Vector3(0, 0, 0); }
Vector3 Vector3::Up() { return Vector3(0, 1, 0); }
Vector3 Vector3::Down() { return Vector3(0, -1, 0); }
Vector3 Vector3::Left() { return Vector3(-1, 0, 0); }
Vector3 Vector3::Right() { return Vector3(1, 0, 0); }
Vector3 Vector3::Forward() { return Vector3(0, 0, 1); }
Vector3 Vector3::Back() { return Vector3(0, 0, -1); }
Vector3 Vector3::One() { return Vector3(1, 1, 1); }

float Vector3::Magnitude() const { return sqrt(x * x + y * y + z * z); }
float Vector3::MagnitudeSquared() const { return x * x + y * y + z * z; }
Vector3 Vector3::Normalized() 
{
    float mag = Magnitude();

    if (mag < .00001)
    {
        return Vector3(0,0,0);
    }

    return Vector3(x / mag, y / mag, z / mag);
}

void Vector3::Normalize() 
{
    float mag = Magnitude();

    if (mag < .00001)
    {
        x, y, z = 0;
    }

    x, y, z /= mag;
}

void Vector3::ClampMagnitude(float maxLength) {}

 void Vector3::ClampMagnitude(Vector3* v, float maxLength) {} // <-- TODO:
float Vector3::Distance(const Vector3& v1, const Vector3& v2) { return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2)); }
float Vector3::DistanceSquared(const Vector3& v1, const Vector3& v2) { return (pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2)); }
float Vector3::Dot(const Vector3& v1, const Vector3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2) { return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y * v1.y * v2.x); }
Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, float t) { return v1 + (v2 - v1) * std::clamp(t, 0.0f, 1.0f); }
Vector3 Vector3::LerpUnclamped(const Vector3& v1, const Vector3& v2, float t) { return v1 + (v2 - v1) * t; }
float Vector3::Angle(const Vector3& from, const Vector3& to) { return acos(Vector3::Dot(from, to) / (from.Magnitude() * to.Magnitude())); }
Vector3 Vector3::Max(const Vector3& v1, const Vector3& v2) { return Vector3(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z)); }
Vector3 Vector3::Min(const Vector3& v1, const Vector3& v2) { return Vector3(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z)); }