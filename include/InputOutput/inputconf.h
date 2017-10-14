#pragma once
#include <string>

class InputConf{
    public:
        InputConf(class App* app);
        void readSubLattice(int update);
        void readConfiguration(int confNum);
        void readConfiguration(std::string inputFile);
private:
        class App* m_app = nullptr;
};
