#pragma once
#include <vector>

class OutputTerm{
    public:
        OutputTerm(class App* app);
        void printInitialConditions();
        void printThermSteps(int step, double acceptRatio);
        void printGenerationStep(int confNum, double acceptRatio);
        void writeObservables(int confNum);
        void writeFlowObservables(std::vector<double> obsVector);
private:
        class App* m_app = nullptr;
        bool m_write;
};
