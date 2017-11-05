#include <cstdio>
#include <chrono>
#include "lqcd.h"

int main(int argn, char* argv[]) {

    // Start timer
    auto start = std::chrono::system_clock::now();

    // Initialize MPI
    Parallel::initialize();

    // Read and validate the input file
    LatticeIO::JsonInput::parseInput(argn, argv);

    // Create Application
    App* lqcd;
    LatticeIO::JsonInput::initApp(lqcd);

    // Execute app
    lqcd->execute();

    // Finalize MPI and delete objects
    Parallel::finalize();

    // Stop timer and output
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedTime = end-start;
    if(Parallel::rank() == 0)
        printf("Total time taken: %.2fs\n", elapsedTime.count());

    // End
    //delete input;
    return 0;
}
