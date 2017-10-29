#pragma once
#include <cstdio>
#include <vector>

class OutputObs{
    public:
    OutputObs(class App* app);
    void writeObservables(int MCSteps);
    void closeFile();
    void writeFlowObservables(int confNum, std::vector<std::vector<double> > obsMatrix);
private:
        class App* m_app = nullptr;
        bool m_write;
        FILE* m_file;
};

