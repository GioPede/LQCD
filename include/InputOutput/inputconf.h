#pragma once
#include "Math/lattice.h"

namespace LatticeIO {

    class InputConf{
    public:
        static void readConf(Lattice& lattice, int confNum);
        static void readConf(Lattice& lattice, const char *inputFile);
        static void readSubLattice(Lattice& lattice, int confNum);
    private:
        static int m_linkSize;
    };

} // end LatticeIO
