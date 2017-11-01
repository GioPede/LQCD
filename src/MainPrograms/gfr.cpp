#include <cstdio>
#include <chrono>
#include "lqcd.h"

/* ****************************************************************************
 *
 * TODO LIST:
 *  Priority MAXIMUM:
 *
 *  Priority NORMAL:
 *      - move parallel stuff of Output classe
 *      - make output for plaquette
 *      - clean and comment
 *
 *  Priority LOW:
 *      - add a dynamic observable input list (now only one Plaquette)
 *
 * ***************************************************************************/


int main(int argn, char* argv[]) {

    // Start timer
    auto start = std::chrono::system_clock::now();

    // Check argument count, read and parse the input file
    InputParser* input;
    input = new InputParser(argn, argv);

    // Initialize MPI and organize sublattices
    Parallel::initialize(input->latticeSize, input->subLatticeSize);

    // Send Parameters to the Reader and init the system
    GaugeFieldReader GFR(input);

    //Init the System
    GFR.initGFR();

    // Start Generating Gauges
    GFR.sampleConfigurations();

    // Finalize MPI and delete objects
    Parallel::finalize();

    // Stop timer and output
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedTime = end-start;
    if(Parallel::rank() == 0)
        printf("Total time taken: %.2fs\n", elapsedTime.count());

    // End
    delete input;
    return 0;
}

