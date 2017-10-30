#pragma once
#include <vector>
#include "point.h"

class Lattice{
    public:
        Lattice(std::vector<int> size);
        Lattice(std::vector<int> size, class Parallel* parallel);
        Point& operator()(int x, int y, int z, int t)
            { return m_lattice[x][y][z][t]; }
        Point operator()(int x, int y, int z, int t) const
            { return m_lattice[x][y][z][t]; }
        std::vector<std::vector<std::vector<std::vector<Point>>>> m_lattice;
        std::vector<int> getSize() { return m_size; }
        void setToRandom();
        void setToUnity();
        class SU3& shift(int x, int y, int z, int t, int mu,
                        int shiftDir, int shiftSign);
        class SU3& shift2(int x, int y, int z, int t, int mu,
                         int shiftDir, int shiftSign, int shiftDir2, int shiftSign2);

private:
        std::vector<int> m_size;
        class Parallel* m_parallel = nullptr;
        SU3 msg;
};
