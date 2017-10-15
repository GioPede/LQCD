#pragma once
#include <cstdio>

class OutputObs{
    public:
    OutputObs(class App* app);
    void writeObservables(int MCSteps);
    void closeFile();
private:
        class App* m_app = nullptr;
        bool m_write;
        FILE* m_file;
};

