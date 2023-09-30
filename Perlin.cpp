#include "Perlin.h"
#include "Vec2.h"

Vec2 RandomGradient(int ix, int iy) {
    // No precomputed gradients mean this works for any number of grid coordinates
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2;
    unsigned a = ix, b = iy;
    a *= 3284157443;

    b ^= a << s | a >> w - s;
    b *= 1911520717;

    a ^= b << s | b >> w - s;
    a *= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]

    // Create the vector from the angle
    Vec2 v;
    v.x = sin(random);
    v.y = cos(random);

    return v;
}

float DotGridGradient(int ix, int iy, float x, float y)
{
	// Get gradients from integer coordinates
	Vec2 gradient = RandomGradient(ix, iy);

    float dx = x - (float)ix;
    float dy = y - (float)iy;
    
    return (dx * gradient.x + dy * gradient.y);
}

// Cubic interpolation.
float Interpolate(float a0, float a1, float w) {
    return (a1 - a0) * (3.0f - w * 2.0f) * w * w + a0;
}

float Perlin(float x, float y)
{
	// Determine grid cell corner coordinates.
	int x0 = (int)x;
	int y0 = (int)y;
	int x1 = x0 + 1;
	int y1 = y0 + 1;

    // Compute Interpolation weights.
	float sx = x - (float)x0;
	float sy = y - (float)y0;

    // Compute and interpolate top corners.
    float n0 = DotGridGradient(x0, y0, x, y);
    float n1 = DotGridGradient(x1, y1, x, y);
    float ix0 = Interpolate(n0, n1, sx);

    // Compute and interpolate bottom two corners.
    n0 = DotGridGradient(x0, y1, x, y);
    n1 = DotGridGradient(x1, y1, x, y);
    float ix1 = Interpolate(n0, n1, sx);

    // Interpolate the two previous interpolated values along the vertical weight.
    float value = Interpolate(ix0, ix1, sy);

    return value;

}