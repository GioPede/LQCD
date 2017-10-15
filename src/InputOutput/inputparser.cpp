#include "InputOutput/inputparser.h"
#include <cstdio>
#include <cstdlib>
#include <string>

// Given an input file it reads through it and gets basic parameters for:
//   - Lattice Size and Sublattice Size
//   - Monte Carlo steps, thermalization and number of conf to save
//   - Action type and parameters
//   - List of Observables to compute and their parameters (if any)


// Initializator (main function) reads through and parses the whole file
InputParser::InputParser(int argn, char* argv[]){
    // open the file
    m_inputFileName = argv[1];
    m_inputFile = fopen(m_inputFileName, "r+");

    for(int i = 3; i < argn; i++){
        std::string fileName(argv[i]);
        inputConfList.push_back(fileName);
    }

    // get its length and check if it is valid
    m_length = countFileLines();
    if(m_length == 0)  printf("File does not exist or is empty\n");

    // read the data from the file
    readSize();
    readMCParams();
    readAction();
    readObservablesList();
    sprintf(outDir, "%s_%c_B%.2lf", argv[2], startType, beta);
}

// Simple funtion that returns the number of lines in the file
int InputParser::countFileLines(){
    int lines = 0;
    char ch;
    while(!feof(m_inputFile)){    // TODO: should add some try-catch in here
        ch = fgetc(m_inputFile);
        if(ch == '\n')
            lines++;
    }
    freopen(m_inputFileName, "r+", m_inputFile);
    return lines;
}

// Expects two sequences of four integers, one for the lattice size, on for the
// sublattice. Also checks if they are meaningful.
void InputParser::readSize(){
    // read the line
    char line[256];
    do{
        fgets(line, sizeof line, m_inputFile);
    }  // ignore comment lines
    while(*line == '\n' || *line == ' ' || *line == '#');

    // read size first
    sscanf(line, "%i %i %i %i", &latticeSize[0], &latticeSize[1], &latticeSize[2], &latticeSize[3]);

    // then the sublattice size
    fgets(line, sizeof line, m_inputFile);
    sscanf(line, "%i %i %i %i", &subLatticeSize[0], &subLatticeSize[1], &subLatticeSize[2], &subLatticeSize[3]);

    // check if the sizes are compatible
    for(int i = 0; i < 4; i++){
        if(latticeSize[i] < subLatticeSize[i] || latticeSize[i] % subLatticeSize[i] != 0){
            printf("Invalid sublattice size along direction %i\n", i);
            exit(0);
        }
    }
}

// Expects three integers, with total MC steps, thermalization steps and the number of
// configurations to save
void InputParser::readMCParams(){
    // read the line
    char line[256];
    do{
        fgets(line, sizeof line, m_inputFile);
    }  // ignore comment lines
    while(*line == '\n' || *line == ' ' || *line == '#');

    // read the number of thermalization steps
    sscanf(line, "%c", &startType);

    // read the number of MC steps
    fgets(line, sizeof line, m_inputFile);
    sscanf(line, "%i", &MCSteps);

    // read the number of thermalization steps
    fgets(line, sizeof line, m_inputFile);
    sscanf(line, "%i", &thermSteps);


    // read the number of configurations to generate
    fgets(line, sizeof line, m_inputFile);
    sscanf(line, "%i", &nConf);

    // read the update parameter epsilon
    fgets(line, sizeof line, m_inputFile);
    sscanf(line, "%lf", &MCUpdateParameter);
}

// Expects the action tag and optional parameters
void InputParser::readAction(){
    char line[256];
    do{
        fgets(line, sizeof line, m_inputFile);
    }  // ignore comment lines
    while(*line == '\n' || *line == ' ' || *line == '#');

    sscanf(line, "%s %*[^\n]\n", &actionTag);

    if(std::string(actionTag) == "puregauge"){
        fgets(line, sizeof line, m_inputFile);
        sscanf(line, "%lf", &beta);
    }
}

void InputParser::readObservablesList(){
}
