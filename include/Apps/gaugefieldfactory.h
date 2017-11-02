#pragma once
#include <vector>
#include <random>

class GaugeFieldFactory {
    public:
        GaugeFieldFactory(class InputParser* input);

        // main functions
        void generateConfigurations();

        // setters-getters
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
        class Action* m_act = nullptr;
        std::vector<class Observable*> m_obs;
        std::vector<double> m_obsValues;
        std::mt19937* m_random = nullptr;

        // member variables
        std::array<int, 4> m_size;
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
        void MCUpdate();
        void updateLink(int x, int y, int z, int t, int mu);
        struct SU3 newLink;
        void computeObservables();
        void thermalize();
        void sampleConf();
        void thermalizeTime();
        void sampleConfTime();
};
