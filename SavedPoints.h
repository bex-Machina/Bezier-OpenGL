#include <vector>

struct savedPoint {
    savedPoint(float x, float y) : x(x), y(y) {}
    float x, y;
};

struct savedLinesSegments {
    savedLinesSegments(float x, float y) :x(x), y(y) {}
    float x, y;
};

std::vector<savedPoint*>* Vpoint(new std::vector<savedPoint*>());
std::vector<savedLinesSegments*>* Lpoint(new std::vector<savedLinesSegments*>());

savedPoint* selected;
savedLinesSegments* selectedAnchor;
savedLinesSegments* selectedControlPont;


inline savedPoint* linearBezier(const savedPoint* p0, const savedPoint* p1, float t)
{
    float x = p0->x * (1 - t) + p1->x * t;
    float y = p0->y * (1 - t) + p1->y * t;
    return new savedPoint(x, y);
}

inline savedLinesSegments* LinelinearBezier(const savedPoint* p0, const savedPoint* p1, float t)
{
    float x = p0->x * (1 - t) + p1->x * t;
    float y = p0->y * (1 - t) + p1->y * t;
    return new savedLinesSegments(x, y);
}

inline savedLinesSegments* cubicBezier(savedLinesSegments* p0, savedLinesSegments* p1, savedLinesSegments* p2, savedLinesSegments* p3, float t)
{
    const double temp = (1.0 - t);
    float x = p0->x * pow(temp, 3) + p1->x * 3 * pow(temp, 2) * t + p2->x * 3 * temp * pow(t, 2) + p3->x * pow(t, 3);
    float y = p0->y * pow(temp, 3) + p1->y * 3 * pow(temp, 2) * t + p2->y * 3 * temp * pow(t, 2) + p3->y * pow(t, 3);
    return new savedLinesSegments(x, y);
}


inline bool pointHitTestP(const savedPoint* point, UINT x, UINT y) {
    int dx = point->x - x;
    int dy = point->y - y;

    const int radius = 16;
    return (dx * dx + dy * dy) < (radius * radius);
}

inline bool pointHitTestL(const savedLinesSegments* point, UINT x, UINT y) {
    int dx = point->x - x;
    int dy = point->y - y;

    const int radius = 16;
    return (dx * dx + dy * dy) < (radius * radius);
}



inline int pointhit(int x, int y)
{
    for (int i = 0; i < Vpoint->size(); i++)
    {
        if (pointHitTestP((*Vpoint)[i], x, y))
        {
            return i;
        }
    }
    return -1;
}

inline int anchorPointHIT(int x, int y)
{
    for (int i = 0; i < Lpoint->size(); i++)
    {
        if (i % 3 == 1 || i % 3 == 2)
        {
            if (pointHitTestL((*Lpoint)[i], x, y))
            {
                return i;
            }
        }

    }
    return -1;
}


inline int controlPointHIT(int x, int y)
{
    for (int i = 0; i < Lpoint->size(); i++)
    {
        if (i % 3 != 1 && i % 3 != 2)
        {
            if (pointHitTestL((*Lpoint)[i], x, y))
            {
                return i;
            }
        }

    }
    return -1;
}

inline int findSelected()
{
    for (int i = 0; i < Lpoint->size(); i++)
    {
        if (i % 3 == 1 || i % 3 == 2)
        {
            if (selectedAnchor == (*Lpoint)[i])
            {
                return i;
            }
        }
    }
    return -1;
}


inline int findSelectedAnchor()
{
    for (int i = 0; i < Lpoint->size(); i++)
    {
        if (i % 3 == 0)
        {
            if (selectedControlPont == (*Lpoint)[i])
            {
                return i;
            }
        }
    }
    return -1;
}