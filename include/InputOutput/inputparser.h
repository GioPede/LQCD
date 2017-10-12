#pragma once
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>

class InputParser{
    public:
        InputParser(int argn, char* argv[]);
        std::vector<int> latticeSize = std::vector<int> (4);
        std::vector<int> subLatticeSize = std::vector<int> (4);
        std::vector<std::string> inputConfList;

        int MCSteps, thermSteps, nConf;
        char actionTag[256];
        double beta, MCUpdateParameter;
        const char* outDir;
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
