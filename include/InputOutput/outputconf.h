#pragma once
#include <vector>

class OutputConf{
    public:
        OutputConf(class App* app);
        void writeSubLattice(int update);
        void writeConfiguration(int confNum);
    private:
        class App* m_app = nullptr;
        std::vector<int> m_subSize;
        std::vector<int> m_fullSize;
        std::vector<int> m_subBlock;
        int m_linkSize;
};
