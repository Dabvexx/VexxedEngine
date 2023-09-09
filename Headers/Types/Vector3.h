#ifndef VECTOR3_H
#define VECTOR3_H

#include <math.h>
#include <algorithm>

class Vector3
{
    public:
        float x;
        float y;
        float z;

        Vector3(float _x = 0, float _y = 0, float _z = 0);
        
        // Functions.
        static Vector3 Zero();
        static Vector3 Up();
        static Vector3 Down();
        static Vector3 Left();
        static Vector3 Right();
        static Vector3 Forward();
        static Vector3 Back();
        static Vector3 One();

        float Magnitude() const;
        float MagnitudeSquared() const;
        Vector3 Normalized();
        void Normalize();
        void ClampMagnitude(float maxLength);

        static void ClampMagnitude(Vector3* v, float maxLength);
        static float Distance(const Vector3& v1, const Vector3& v2);
        static float DistanceSquared(const Vector3& v1, const Vector3& v2);
        static float Dot(const Vector3& v1, const Vector3& v2);
        static Vector3 Cross(const Vector3& v1, const Vector3& v2);
        static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
        static Vector3 LerpUnclamped(const Vector3& v1, const Vector3& v2, float t);
        static float Angle(const Vector3& from, const Vector3& to);
        static Vector3 Max(const Vector3& v1, const Vector3& v2);
        static Vector3 Min(const Vector3& v1, const Vector3& v2);

        // Assignment and equality/comparison operators.
        inline Vector3& Vector3::operator = (const Vector3& v) { x = v.x; y = v.y; z = v.z; return *this; }
        inline Vector3& Vector3::operator = (const float& f) { x = f; y = f; z = f; return *this; }
        inline Vector3& Vector3::operator - (void) { x = -x; y = -y; z = -z; return *this; }
        // The == operator may be an issue, replace with something that will check against an epsilon like 2s complement.
        inline bool Vector3::operator == (const Vector3& v) const { return (x == v.x) && (y == v.y) && (z == v.z); }
        inline bool Vector3::operator != (const Vector3& v) const { return (x != v.x) || (y != v.y) && (z != v.z); }
        inline bool Vector3::operator > (const Vector3& v) { return (x > v.x) && (y > v.y) && (z > v.z); }
        inline bool Vector3::operator < (const Vector3& v) { return (x < v.x) && (y < v.y) && (z < v.z); }
        inline bool Vector3::operator <= (const Vector3& v) { return (x >= v.x) && (y >= v.y) && (z >= v.z); }
        inline bool Vector3::operator >= (const Vector3& v) { return (x <= v.x) && (y <= v.y) && (z <= v.z); }

        //Vec2 to Vec2 operators
        inline const Vector3 Vector3::operator + (const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
        inline const Vector3 Vector3::operator - (const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
        inline const Vector3 Vector3::operator * (const Vector3& v) const { return Vector3(x * v.x, y * v.y, z * v.z); }
        inline const Vector3 Vector3::operator / (const Vector3& v) const { return Vector3(x / v.x, y / v.y, z / v.z); }

        // Vec2 to this operators
        inline Vector3& Vector3::operator += (const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
        inline Vector3& Vector3::operator -= (const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
        inline Vector3& Vector3::operator *= (const Vector3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
        inline Vector3& Vector3::operator /= (const Vector3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }

        // Scaler to Vec2 operators
        inline const Vector3 Vector3::operator + (float v) const { return Vector3(x + v, y + v, z + v); }
        inline const Vector3 Vector3::operator - (float v) const { return Vector3(x - v, y - v, z - v); }
        inline const Vector3 Vector3::operator * (float v) const { return Vector3(x * v, y * v, z * v); }
        inline const Vector3 Vector3::operator / (float v) const { return Vector3(x / v, y / v, z / v); }

        //Scaler to this operators
        inline Vector3& Vector3::operator += (float v) { x += v; y += v; z += v; return *this; }
        inline Vector3& Vector3::operator -= (float v) { x -= v; y -= v; z -= v; return *this; }
        inline Vector3& Vector3::operator *= (float v) { x *= v; y *= v; z *= v; return *this; }
        inline Vector3& Vector3::operator /= (float v) { x /= v; y /= v; z /= v; return *this; }
};

#endif