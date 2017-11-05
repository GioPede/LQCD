#pragma once
#include "Math/lattice.h"

namespace LatticeIO {

    class OutputConf{
    public:
        static void writeConf(Lattice& lattice, int confNum);
        static void writeSubLattice(Lattice& lattice, int confNum);
        static void setOutputDir(std::string outputDir);
    private:
        static int m_linkSize;
        static std::string m_outputDir;

    };

} // end LatticeIO
