#pragma once
#include <vector>

class Lattice {
public:
    Lattice(std::vector<int> size);
    std::vector<int>& getSize() { return m_size; }
    double getNumberOfDimensions() { return m_numberOfDimensions; }
private:
    int     m_numberOfDimensions = 0;
    std::vector<int> m_size = std::vector<int>();
};

