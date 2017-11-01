#pragma once
#include <string>
#include <array>

class InputConf{
    public:
        InputConf(class App* app);
        void readSubLattice(int update);
        void readConfiguration(int confNum);
        void readConfiguration(const char *inputFile);
private:
        class App* m_app = nullptr;
        std::array<int, 4> m_subSize;
        std::array<int, 4> m_fullSize;
        std::array<int, 4> m_subBlock;
        int m_linkSize;
};
