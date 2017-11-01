#pragma once
#include <vector>
#include <random>
#include <array>
#include "Apps/app.h"


class WilsonFlow : public App {
    public:
        WilsonFlow(class InputParser* input);

        // main functions
        void flowConfigurations();

        // setters-getters
        void setAction(class Action* action);
        void addObservable(class Observable* observable);
        const char* getOutDir() { return m_outDir; }
        char getStartType() { return m_startType; }
        class Point& getLatticeSite(int x, int y, int z, int t);
        std::array<int, 4>& getSize() { return m_size; }
        std::vector<double>& getObsValues() { return m_obsValues; }
        std::vector<class Observable*>& getObs() { return m_obs; }


    private:
        // member classes
        class Lattice* m_lat = nullptr;
        // flow lattices
        class Lattice* m_Z0 = nullptr;
        class Lattice* m_Z1 = nullptr;
        class Lattice* m_Z2 = nullptr;
        class Action* m_act = nullptr;
        class OutputConf* m_outputConf = nullptr;
        class OutputTerm* m_outputTerm = nullptr;
        class OutputObs* m_outputObs = nullptr;
        class InputConf* m_inputConf = nullptr;
        std::vector<class Observable*> m_obs;
        std::vector<double> m_obsValues;
        std::vector<std::string> m_inputConfList;
        std::array<int, 4> m_size;

        struct SU3 omega;
        // member variables
        int m_MCSteps;
        int m_correlationSteps;
        int m_thermSteps;
        int m_confs;
        long int m_accepted = 0;
        long int m_updates = 0;
        double m_epsilon;
        long m_seed;
        const char* m_outDir;
        char m_startType;
        void computeObservables();
        void flowStep(double epsilon);
        void applyWilsonFlow(int confNum, double epsilon);
};
