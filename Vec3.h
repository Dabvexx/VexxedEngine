#ifndef VECTOR3_H
#define VECTOR3_H

#include <math.h>

class Vec3
{
public:
    float x;
    float y;
    float z;

    Vec3(float _x = 0, float _y = 0, float _z = 0);

    // Functions.
    static Vec3 Zero();
    static Vec3 Up();
    static Vec3 Down();
    static Vec3 Left();
    static Vec3 Right();
    static Vec3 Forward();
    static Vec3 Back();
    static Vec3 One();

    float Magnitude() const;
    float MagnitudeSquared() const;
    Vec3 Normalized();
    void Normalize();
    void ClampMagnitude(float maxLength);

    static void ClampMagnitude(Vec3* v, float maxLength);
    static float Distance(const Vec3& v1, const Vec3& v2);
    static float DistanceSquared(const Vec3& v1, const Vec3& v2);
    static float Dot(const Vec3& v1, const Vec3& v2);
    static Vec3 Cross(const Vec3& v1, const Vec3& v2);
    static Vec3 Lerp(const Vec3& v1, const Vec3& v2, float t);
    static Vec3 LerpUnclamped(const Vec3& v1, const Vec3& v2, float t);
    static float Angle(const Vec3& from, const Vec3& to);
    static Vec3 Max(const Vec3& v1, const Vec3& v2);
    static Vec3 Min(const Vec3& v1, const Vec3& v2);

    // Assignment and equality/comparison operators.
    inline Vec3& operator = (const Vec3& v) { x = v.x; y = v.y; z = v.z; return *this; }
    inline Vec3& operator = (const float& f) { x = f; y = f; z = f; return *this; }
    inline Vec3& operator - (void) { x = -x; y = -y; z = -z; return *this; }
    // The == operator may be an issue, replace with something that will check against an epsilon like 2s complement.
    inline bool operator == (const Vec3& v) const { return (x == v.x) && (y == v.y) && (z == v.z); }
    inline bool operator != (const Vec3& v) const { return (x != v.x) || (y != v.y) && (z != v.z); }
    inline bool operator > (const Vec3& v) { return (x > v.x) && (y > v.y) && (z > v.z); }
    inline bool operator < (const Vec3& v) { return (x < v.x) && (y < v.y) && (z < v.z); }
    inline bool operator <= (const Vec3& v) { return (x >= v.x) && (y >= v.y) && (z >= v.z); }
    inline bool operator >= (const Vec3& v) { return (x <= v.x) && (y <= v.y) && (z <= v.z); }

    //Vec2 to Vec2 operators
    inline const Vec3 operator + (const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    inline const Vec3 operator - (const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    inline const Vec3 operator * (const Vec3& v) const { return Vec3(x * v.x, y * v.y, z * v.z); }
    inline const Vec3 operator / (const Vec3& v) const { return Vec3(x / v.x, y / v.y, z / v.z); }

    // Vec2 to this operators
    inline Vec3& operator += (const Vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
    inline Vec3& operator -= (const Vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    inline Vec3& operator *= (const Vec3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
    inline Vec3& operator /= (const Vec3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }

    // Scaler to Vec2 operators
    inline const Vec3 operator + (float v) const { return Vec3(x + v, y + v, z + v); }
    inline const Vec3 operator - (float v) const { return Vec3(x - v, y - v, z - v); }
    inline const Vec3 operator * (float v) const { return Vec3(x * v, y * v, z * v); }
    inline const Vec3 operator / (float v) const { return Vec3(x / v, y / v, z / v); }

    //Scaler to this operators
    inline Vec3& operator += (float v) { x += v; y += v; z += v; return *this; }
    inline Vec3& operator -= (float v) { x -= v; y -= v; z -= v; return *this; }
    inline Vec3& operator *= (float v) { x *= v; y *= v; z *= v; return *this; }
    inline Vec3& operator /= (float v) { x /= v; y /= v; z /= v; return *this; }
};

#endif