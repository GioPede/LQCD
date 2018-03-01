#pragma once
#include <vector>
#include <random>
#include "Apps/app.h"
#include "Math/su3.h"

class GaugeFieldFactory : public App {
public:
    GaugeFieldFactory(int MCSteps, int thermSteps,
                      int NConf, double epsilon,
                      std::string startType);
    // main functions
    void generateConfigurations();

    // setters-getters
    class Point& getLatticeSite(int x, int y, int z, int t);
    std::vector<double>& getObsValues() { return m_obsValues; }
    void execute();
    //void createLattice(std::array<int,4> latticeSize);
    std::vector<std::string> m_inputConfList;

private:
    // member classes
    std::vector<double> m_obsValues;

    // Private methods
    void initialize();

    // Input variables
    int m_MCSteps;
    int m_correlationSteps;
    int m_thermSteps;
    int m_confs;
    std::string m_startType;
    double m_epsilon;


    long int m_accepted = 0;
    long int m_updates = 0;
    long m_seed;
    const char* m_outDir;
    void MCUpdate();
    void updateLink(int x, int y, int z, int t, int mu);
    SU3 newLink;
    void computeObservables();
    void thermalize();
    void sampleConf();
    void thermalizeTime();
    void sampleConfTime();
    void continueSample(int confStart);
};
