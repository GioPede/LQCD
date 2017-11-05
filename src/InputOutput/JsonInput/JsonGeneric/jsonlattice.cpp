#include "InputOutput/JsonInput/json.hpp"
#include "InputOutput/JsonInput/jsoninput.h"
#include "InputOutput/JsonInput/JsonGeneric/jsonlattice.h"
#include <iostream>

namespace LatticeIO {
    namespace JsonInput {

        std::array<int, 4> latticeSize;
        std::array<int, 4> subLatticeSize;
        void validateLatticeInput(){
            // check lattice size
            if(jsonFile.count("LatticeSize") == 0){
                std::cerr << "LatticeQCD Error: no LatticeSize specified\n";
                exit(1);
            }
            if(jsonFile.count("LatticeSize") != 1){
                std::cerr << "LatticeQCD Error: too many LatticeSize specified\n";
                exit(1);
            }
            if(    jsonFile["LatticeSize"].size() != 4
                || jsonFile["LatticeSize"]["x"].size() != 1
                || jsonFile["LatticeSize"]["y"].size() != 1
                || jsonFile["LatticeSize"]["z"].size() != 1
                || jsonFile["LatticeSize"]["t"].size() != 1){

                std::cerr << "LatticeQCD Error: wrong LatticeSize specification\n"
                             "Correct format is:\n "
                             "\"LatticeSize\" : {\n"
                             "   \"x\" : Nx, \n"
                             "   \"y\" : Ny, \n"
                             "   \"z\" : Nz, \n"
                             "   \"t\" : Nt  \n"
                             "}\n";
                exit(1);
            }
            latticeSize[0] = jsonFile["LatticeSize"]["x"];
            latticeSize[1] = jsonFile["LatticeSize"]["y"];
            latticeSize[2] = jsonFile["LatticeSize"]["z"];
            latticeSize[3] = jsonFile["LatticeSize"]["t"];
        }

        void validateSubLatticeInput(){
            // check sublattice size
            if(jsonFile.count("SubLatticeSize") == 0){
                std::cerr << "LatticeQCD Error: no SubLatticeSize specified\n";
                exit(1);
            }
            if(jsonFile.count("SubLatticeSize") != 1){
                std::cerr << "LatticeQCD Error: too many SubLatticeSize specified\n";
                exit(1);
            }
            if(    jsonFile["SubLatticeSize"].size() != 4
                || jsonFile["SubLatticeSize"]["x"].size() != 1
                || jsonFile["SubLatticeSize"]["y"].size() != 1
                || jsonFile["SubLatticeSize"]["z"].size() != 1
                || jsonFile["SubLatticeSize"]["t"].size() != 1){

                std::cerr << "LatticeQCD Error: wrong SubLatticeSize specification\n"
                             "Correct format is:\n "
                             "\"SubLatticeSize\" : {\n"
                             "   \"x\" : Nx, \n"
                             "   \"y\" : Ny, \n"
                             "   \"z\" : Nz, \n"
                             "   \"t\" : Nt  \n"
                             "}\n";
                exit(1);
            }
            subLatticeSize[0] = jsonFile["SubLatticeSize"]["x"];
            subLatticeSize[1] = jsonFile["SubLatticeSize"]["y"];
            subLatticeSize[2] = jsonFile["SubLatticeSize"]["z"];
            subLatticeSize[3] = jsonFile["SubLatticeSize"]["t"];
        }
    } // end JsonInput
} // end LatticeIO
