#pragma once
#include <array>

class OutputConf{
public:
    OutputConf(class App* app);
    void writeSubLattice(int update);
    void writeConfiguration(int confNum);
private:
    class App* m_app = nullptr;
    std::array<int, 4> m_subSize;
    std::array<int, 4> m_fullSize;
    std::array<int, 4> m_subBlock;
    int m_linkSize;
};
