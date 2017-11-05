#include "InputOutput/JsonInput/json.hpp"
#include "InputOutput/JsonInput/jsoninput.h"
#include "InputOutput/JsonInput/JsonGeneric/jsondirectories.h"
#include "InputOutput/io.h"
#include <iostream>

namespace LatticeIO {
    namespace JsonInput {

        void validateDirectories(){
            validateInputDir();
            validateOutputDir();
        }

        void registerDirectories(){
            registerInputDir();
            registerOutputDir();
        }

        void validateInputDir(){
            // check action
            if(jsonFile["App"]["type"] == "GFR" || jsonFile["App"]["type"] == "FLOW"){
                if(jsonFile.count("InputDir") == 0){
                    std::cerr << "LatticeQCD Error: no InputDir specified\n";
                    exit(1);
                }
                if(jsonFile.count("InputDir") != 1){
                    std::cerr << "LatticeQCD Error: too many InputDir specified\n";
                    exit(1);
                }
            }
        }

        void validateOutputDir(){
            // check action
            if(jsonFile.count("OutputDir") == 0){
                std::cerr << "LatticeQCD Error: no OutputDir specified\n";
                exit(1);
            }
            if(jsonFile.count("OutputDir") != 1){
                std::cerr << "LatticeQCD Error: too many OutputDir specified\n";
                exit(1);
            }
        }

        void registerOutputDir(){
            LatticeIO::OutputObs::setOutputDir(jsonFile["OutputDir"]);
            LatticeIO::OutputConf::setOutputDir(jsonFile["OutputDir"]);
        }

        void registerInputDir(){
            if(jsonFile["App"]["type"] == "GFR" || jsonFile["App"]["type"] == "FLOW"){
                LatticeIO::InputConf::setInputDir(jsonFile["InputDir"]);
            }
        }

    } // end JsonInput
} // end LatticeIO
