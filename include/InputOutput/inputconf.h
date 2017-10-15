#pragma once
#include <string>
#include <vector>

class InputConf{
    public:
        InputConf(class App* app);
        void readSubLattice(int update);
        void readConfiguration(int confNum);
        void readConfiguration(const char *inputFile);
private:
        class App* m_app = nullptr;
        std::vector<int> m_subSize;
        std::vector<int> m_fullSize;
        std::vector<int> m_subBlock;
        int m_linkSize;
};
