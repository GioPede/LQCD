#pragma once
#include <vector>
#include <array>
#include "point.h"

class Lattice{
public:
    Lattice(std::array<int, 4> size);
    Point& operator()(int x, int y, int z, int t)
        { return m_lattice[x][y][z][t]; }
    Point operator()(int x, int y, int z, int t) const
        { return m_lattice[x][y][z][t]; }
    std::vector<std::vector<std::vector<std::vector<Point>>>> m_lattice;
    std::array<int, 4>& getSize() { return m_size; }
    void setToRandom();
    void setToUnity();
    class SU3 shift(int x, int y, int z, int t, int mu,
                     int shiftDir, int shiftSign);
    class SU3 shift2(int x, int y, int z, int t, int mu,
                      int shiftDir, int shiftSign, int shiftDir2, int shiftSign2);

private:
    std::array<int, 4> m_size;
};
