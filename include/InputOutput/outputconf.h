#pragma once

class OutputConf{
    public:
        OutputConf(class App* app);
        void writeSubLattice(int update);
        void writeConfiguration(int confNum);
private:
        class App* m_app = nullptr;
};
