#pragma once
#include <vector>

class System {
public: 
    // INITIALIZERS

    // MAIN FUNCTIONS
    bool metropolisStep();
    void runMetropolisSteps (int numberOfMetropolisSteps);

    // VARIABLE AND CLASS SETTERS
    void setEquilibrationFraction (double equilibrationFraction)      { m_equilibrationFraction = equilibrationFraction; }
    void setLattice               (class Lattice* lattice)            { m_lattice = lattice; }
    void setAction                (class Action*  action)             { m_action = action; }
    void setSampler               (class Sampler* sampler)            { m_sampler = sampler; }

    // VARIABLE AND CLASS GETTERS
    class Lattice*  getLattice()  { return m_lattice; }
    class Action*   getAction()   { return m_action; }
    class Sampler*  getSampler()  { return m_sampler; }
    int  getNumberOfMetropolisSteps() { return m_numberOfMetropolisSteps; }


private:
    // PRIVATE VARIABLES
    int     m_numberOfMetropolisSteps = 0;
    double  m_equilibrationFraction   = 0.0;

    class Lattice*                 m_lattice = nullptr;
    class Action*                  m_action = nullptr;
    class Sampler*                 m_sampler = nullptr;
};

