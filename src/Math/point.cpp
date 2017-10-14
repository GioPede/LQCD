#include "Math/point.h"
#include "Math/su3.h"

Point::Point(){
    for(int i = 0; i < 4; i++)
        m_links[i].setSU3Identity();
}

