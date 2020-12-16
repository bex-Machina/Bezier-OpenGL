#pragma once
#include <math.h>

struct savedPoint {
    savedPoint(float x, float y) : x(x), y(y), backpoint(nullptr), frontpoint(nullptr) {}
    float x, y;
    savedPoint* backpoint;
    savedPoint* frontpoint;
};

inline savedPoint* linearBezier(const savedPoint* p0, const savedPoint* p1, float t)
{
    float x = p0->x * (1 - t) + p1->x * t;
    float y = p0->y * (1 - t) + p1->y * t;
    //return new savedPoint(p0->x * (1 - t) + p1->x * t, p0->y * (1 - t) + p1->y * t);
    return new savedPoint(x, y);
}

inline savedPoint* cubicBezier(savedPoint* p0, savedPoint* p1, savedPoint* p2, savedPoint* p3, float t)
{
    const double temp = (1.0 - t);
    float x = p0->x * pow(temp, 3) + p1->x * 3 * pow(temp, 2) * t + p2->x * 3 * temp * pow(t, 2) + p3->x * pow(t, 3);
    float y = p0->y * pow(temp, 3) + p1->y * 3 * pow(temp, 2) * t + p2->y * 3 * temp * pow(t, 2) + p3->y * pow(t, 3);
    //return new LineSegmentPoint(p0->x * pow(temp, 3) + p1->x * 3 * pow(temp, 2) * t + p2->x * 3 * temp * pow(t, 2) + p3->x * pow(t, 3), p0->y * pow(temp, 3) + p1->y * 3 * pow(temp, 2) * t + p2->y * 3 * temp * pow(t, 2) + p3->y * pow(t, 3));
    return new savedPoint(x, y);
}

