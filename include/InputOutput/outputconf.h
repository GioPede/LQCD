#pragma once
#include "Math/lattice.h"

namespace LatticeIO {

    class OutputConf{
    public:
        static void writeConf(Lattice& lattice, int confNum);
        static void writeSubLattice(Lattice& lattice, int confNum);
    private:
        static int m_linkSize;
    };

} // end LatticeIO
