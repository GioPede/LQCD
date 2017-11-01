#pragma once
#include <array>
#include <string>

struct LatticeUnits{

    static void initialize(double beta);

    static double beta;
    static double latticeVolume;
    static double latticeSpacing;
    static std::array<int, 4> size;
    static int latticeSites;
    static int Nc;
    static double plaquette(double value);
    static double energyDensity(double value);
    static double topologicalCharge(double value);
    static double calculateLatticeSpacing(double beta);
};
