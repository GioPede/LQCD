#pragma once
#include "Math/lattice.h"

namespace LatticeIO {

    class InputConf{
    public:
        static void readConf(Lattice& lattice, int confNum);
        static void readConf(Lattice& lattice, const char *inputFile);
        static void readSubLattice(Lattice& lattice, int confNum);
        static void setInputDir(std::string inputDir);

        static void getInputList(std::vector<std::string>& inputConfList);
    private:
        static int m_linkSize;
        static std::string m_inputDir;
    };

} // end LatticeIO
