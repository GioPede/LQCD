#pragma once
#include <vector>
#include <random>
#include <array>

class GaugeFieldReader{
    public:
        GaugeFieldReader(class InputParser* input);

        // main functions
        void initGFR();
        void sampleConfigurations();

        // setters-getters
        void addObservable(class Observable* observable);
        const char* getOutDir() { return m_outDir; }
        class Point& getLatticeSite(int x, int y, int z, int t);
        std::array<int, 4>& getSize() { return m_size; }
        std::vector<double>& getObsValues() { return m_obsValues; }
        std::vector<class Observable*>& getObs() { return m_obs; }

private:
        // member classes
        class Lattice* m_lat = nullptr;
        std::vector<class Observable*> m_obs;
        std::vector<double> m_obsValues;
        std::vector<std::string> m_inputConfList;
        std::array<int, 4> m_size;


        // member variables
        const char* m_outDir;
};
