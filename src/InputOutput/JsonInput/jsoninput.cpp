#include <iostream>
#include <fstream>
#include "Utils/clusterspecifier.h"
#ifdef LACONIA
#include <mpi.h>
#else
#include <mpi/mpi.h>
#endif

#include "lqcd.h"

namespace LatticeIO {
    namespace JsonInput {

        nlohmann::json jsonFile;

        // Validate inputfile
        void parseInput(int argn, char* argv[]){
            if(argn != 2){
                std::cerr << "LatticeQCD Error: wrong numper of "
                            "command line arguments provided.\n";
                exit(1);
            }

            // read JSON file
            std::ifstream inputFile(argv[1]);
            inputFile >> jsonFile;

            // Validate app type, app and instantiate class
            validateAppType();

            // Validate lattice, sublattice and initialize Parallel geometry
            validateLatticeInput();
            validateSubLatticeInput();

            // Validate action (if needed) and instantiate
            validateAction();

            // Validate observables list and instantiate
            validateObservables();

            // Validate directories and register
            validateDirectories();
        }

        void initApp(App*& app){
            registerApp(app);
            registerAction(app);
            registerObservables(app);
            registerDirectories();
        }

    } // end JsonInput
} // end LatticeIO      
