#pragma once
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <array>
#include <string>

class InputParser{
    public:
        InputParser(int argn, char* argv[]);
        std::array<int, 4> latticeSize;
        std::array<int, 4> subLatticeSize;
        std::vector<std::string> inputConfList;

        int MCSteps, thermSteps, nConf;
        char actionTag[32];
        double beta, MCUpdateParameter;
        char outDir[128];
        char startType;

    private:
        FILE* m_inputFile;
        const char* m_inputFileName;
        int m_length;
        int countFileLines();
        void readSize();
        void readMCParams();
        void readAction();
        void readObservablesList();
};
