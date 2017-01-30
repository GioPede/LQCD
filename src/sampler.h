#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>

class Sampler {
public:
    Sampler(class System* system);
    void sample(bool acceptedStep);
    void printOutputToTerminal();
    void computeAverages();
    
private:
    int     m_numberOfMetropolisSteps = 0;
    int     m_stepNumber = 0;

    class System* m_system = nullptr;
};
