#pragma once
#include "../Math/su3.h"

class Point{
    public:
        Point();
        SU3& operator[](int i) { return m_links[i]; }
        SU3 operator[](int i) const { return m_links[i]; }
        SU3 m_links[4];
};
