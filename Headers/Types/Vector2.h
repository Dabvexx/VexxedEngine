#ifndef VECTOR2_H
#define VECTOR2_H

#include <math.h>

class Vector2
{
    public:
        float x;
        float y;

        // Constructors.
        Vector2(float _x, float _y);

        // Functions.
        static Vector2 Zero();
        static Vector2 Up();
        static Vector2 Down();
        static Vector2 Left();
        static Vector2 Right();
        static Vector2 One();

        float Magnitude() const;
        float MagnitudeSquared() const;
        float Distance(const Vector2& v) const;
        float DistanceSquared(const Vector2& v) const;
        float Dot(const Vector2& v) const;
        float Cross(const Vector2& v) const;
        Vector2& Ortho();
        void Normalize();
        Vector2 Normalized();

        // TODO: Decide if I want a translate for the vector 2 or just transform.

        static float Distance(const Vector2& v1, const Vector2& v2);
        static float DistanceSquared(const Vector2& v1, const Vector2& v2);
        static float Dot(const Vector2& v1, const Vector2& v2);
        static float Cross(const Vector2& v1, const Vector2& v2);

        // Assignment and equality/comparison operators.
        inline Vector2& Vector2::operator = (const Vector2& v) { x = v.x; y = v.y; return *this; }
        inline Vector2& Vector2::operator = (const float& f) { x = f; y = f; return *this; }
        inline Vector2& Vector2::operator - (void) { x = -x; y = -y; return *this; }
        // The == operator may be an issue, replace with something that will check against an epsilon like 2s complement.
        inline bool Vector2::operator == (const Vector2& v) const { return (x == v.x) && (y == v.y); }
        inline bool Vector2::operator != (const Vector2& v) const { return (x != v.x) || (y != v.y); }
        inline bool Vector2::operator > (const Vector2& v) { return (x > v.x) && (y > v.y); }
        inline bool Vector2::operator < (const Vector2& v) { return (x < v.x) && (y < v.y); }
        inline bool Vector2::operator <= (const Vector2& v) { return (x >= v.x) && (y >= v.y); }
        inline bool Vector2::operator >= (const Vector2& v) { return (x <= v.x) && (y <= v.y); }

        //Vec2 to Vec2 operators
        inline const Vector2 Vector2::operator + (const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
        inline const Vector2 Vector2::operator - (const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
        inline const Vector2 Vector2::operator * (const Vector2& v) const { return Vector2(x * v.x, y * v.y); }
        inline const Vector2 Vector2::operator / (const Vector2& v) const { return Vector2(x / v.x, y / v.y); }

        // Vec2 to this operators
        inline Vector2& Vector2::operator += (const Vector2& v) { x += v.x; y += v.y; return *this; }
        inline Vector2& Vector2::operator -= (const Vector2& v) { x -= v.x; y -= v.y; return *this; }
        inline Vector2& Vector2::operator *= (const Vector2& v) { x *= v.x; y *= v.y; return *this; }
        inline Vector2& Vector2::operator /= (const Vector2& v) { x /= v.x; y /= v.y; return *this; }

        // Scaler to Vec2 operators
        inline const Vector2 Vector2::operator + (float v) const { return Vector2(x + v, y + v); }
        inline const Vector2 Vector2::operator - (float v) const { return Vector2(x - v, y - v); }
        inline const Vector2 Vector2::operator * (float v) const { return Vector2(x * v, y * v); }
        inline const Vector2 Vector2::operator / (float v) const { return Vector2(x / v, y / v); }

        //Scaler to this operators
        inline Vector2& Vector2::operator += (float v) { x += v; y += v; return *this; }
        inline Vector2& Vector2::operator -= (float v) { x -= v; y -= v; return *this; }
        inline Vector2& Vector2::operator *= (float v) { x *= v; y *= v; return *this; }
        inline Vector2& Vector2::operator /= (float v) { x /= v; y /= v; return *this; }
};

#endif