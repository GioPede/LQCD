#pragma once
#include <vector>
#include <random>
#include "Apps/app.h"


class GaugeFieldReader : public App {
    public:
        GaugeFieldReader(class InputParser* input, class Parallel* parallel);

        // main functions
        void initGFR();
        void sampleConfigurations();

        // setters-getters
        void setMCSteps(int nMCSteps);
        void setMCStepSize(double epsilon);
        void setCorrelationSteps(int nCorrSteps);
        void setThermSteps(int nThermSteps);
        void setConfQuantity(int nConfQty);
        void setSize(std::vector<int> size);
        void setAction(class Action* action);
        void addObservable(class Observable* observable);
        const char* getOutDir() { return m_outDir; }
        char getStartType() { return m_startType; }
        class Point& getLatticeSite(int x, int y, int z, int t);
        class Parallel* getParallel() { return m_parallel; }
        std::vector<int>& getSize() { return m_size; }
        std::vector<double>& getObsValues() { return m_obsValues; }
        std::vector<class Observable*>& getObs() { return m_obs; }


    private:
        // member classes
        class Lattice* m_lat = nullptr;
        class Action* m_act = nullptr;
        class Parallel* m_parallel = nullptr;
        class OutputConf* m_outputConf = nullptr;
        class InputConf* m_inputConf = nullptr;
        std::vector<class Observable*> m_obs;
        std::vector<double> m_obsValues;
        std::mt19937_64* m_random = nullptr;
        std::vector<std::string> m_inputConfList;
        std::vector<int> m_size;


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
};
