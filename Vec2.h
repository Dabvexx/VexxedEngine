#ifndef VECTOR2_H
#define VECTOR2_H

#include <math.h>

class Vec2
{
public:
    float x;
    float y;

    // Constructors.
    Vec2(float _x, float _y);
    Vec2();

    // Functions.
    static Vec2 Zero();
    static Vec2 Up();
    static Vec2 Down();
    static Vec2 Left();
    static Vec2 Right();
    static Vec2 One();

    float Magnitude() const;
    float MagnitudeSquared() const;
    float Distance(const Vec2& v) const;
    float DistanceSquared(const Vec2& v) const;
    float Dot(const Vec2& v) const;
    float Cross(const Vec2& v) const;
    Vec2& Ortho();
    void Normalize();
    Vec2 Normalized();

    // TODO: Decide if I want a translate for the vector 2 or just transform.

    static float Distance(const Vec2& v1, const Vec2& v2);
    static float DistanceSquared(const Vec2& v1, const Vec2& v2);
    static float Dot(const Vec2& v1, const Vec2& v2);
    static float Cross(const Vec2& v1, const Vec2& v2);

    // Assignment and equality/comparison operators.
    inline Vec2& operator = (const Vec2& v) { x = v.x; y = v.y; return *this; }
    inline Vec2& operator = (const float& f) { x = f; y = f; return *this; }
    inline Vec2& operator - (void) { x = -x; y = -y; return *this; }
    // The == operator may be an issue, replace with something that will check against an epsilon like 2s complement.
    inline bool operator == (const Vec2& v) const { return (x == v.x) && (y == v.y); }
    inline bool operator != (const Vec2& v) const { return (x != v.x) || (y != v.y); }
    inline bool operator > (const Vec2& v) { return (x > v.x) && (y > v.y); }
    inline bool operator < (const Vec2& v) { return (x < v.x) && (y < v.y); }
    inline bool operator <= (const Vec2& v) { return (x >= v.x) && (y >= v.y); }
    inline bool operator >= (const Vec2& v) { return (x <= v.x) && (y <= v.y); }

    //Vec2 to Vec2 operators
    inline const Vec2 operator + (const Vec2& v) const { return Vec2(x + v.x, y + v.y); }
    inline const Vec2 operator - (const Vec2& v) const { return Vec2(x - v.x, y - v.y); }
    inline const Vec2 operator * (const Vec2& v) const { return Vec2(x * v.x, y * v.y); }
    inline const Vec2 operator / (const Vec2& v) const { return Vec2(x / v.x, y / v.y); }

    // Vec2 to this operators
    inline Vec2& operator += (const Vec2& v) { x += v.x; y += v.y; return *this; }
    inline Vec2& operator -= (const Vec2& v) { x -= v.x; y -= v.y; return *this; }
    inline Vec2& operator *= (const Vec2& v) { x *= v.x; y *= v.y; return *this; }
    inline Vec2& operator /= (const Vec2& v) { x /= v.x; y /= v.y; return *this; }

    // Scaler to Vec2 operators
    inline const Vec2 operator + (float v) const { return Vec2(x + v, y + v); }
    inline const Vec2 operator - (float v) const { return Vec2(x - v, y - v); }
    inline const Vec2 operator * (float v) const { return Vec2(x * v, y * v); }
    inline const Vec2 operator / (float v) const { return Vec2(x / v, y / v); }

    //Scaler to this operators
    inline Vec2& operator += (float v) { x += v; y += v; return *this; }
    inline Vec2& operator -= (float v) { x -= v; y -= v; return *this; }
    inline Vec2& operator *= (float v) { x *= v; y *= v; return *this; }
    inline Vec2& operator /= (float v) { x /= v; y /= v; return *this; }
};

#endif