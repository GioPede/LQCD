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
#include<random>

extern std::mt19937_64 randomGen;
extern std::uniform_real_distribution<double> randomUniformInterval;

int main(int argn, char* argv[]) {

    Parallel* parallelGeom = new Parallel(argn, argv);

    if(parallelGeom->getRank() == 0){
        SU3 g[8];
        SU3 a;
        for(int i = 0; i < 8; i++)
            g[i].setSU3Zero();

        g[0].mat[3] = -0.5;
        g[0].mat[7] = -0.5;

        g[1].mat[2] = -0.5;
        g[1].mat[6] =  0.5;

        g[2].mat[1] = -0.5;
        g[2].mat[9] = 0.5;

        g[3].mat[5] = -0.5;
        g[3].mat[13] = -0.5;

        g[4].mat[4] = -0.5;
        g[4].mat[12] = 0.5;

        g[5].mat[11] = -0.5;
        g[5].mat[15] = -0.5;

        g[6].mat[10] = -0.5;
        g[6].mat[14] = 0.5;

        g[7].mat[1] = -0.5*sqrt(3);
        g[7].mat[9] = -0.5*sqrt(3);
        g[7].mat[17] = 1.0*sqrt(3);

        SU3 Q;
        Q.setSU3Zero();

        for(int i = 0; i < 8; i++){
            Q += g[i]*randomUniformInterval(randomGen);
        }
        Q.printSU3();
        SU3 Q2 = Q*Q;

        Q2.printSU3();
        (Q2*Q).printSU3();
        (Q*Q2).printSU3();

    }



    parallelGeom->finalize();
    delete parallelGeom;
    return 0;
}

    /*

int main(int argn, char* argv[]) {

    // Start timer
    auto start = std::chrono::system_clock::now();

    // Start MPI
    Parallel* parallelGeom = new Parallel(argn, argv);

    // Check argument count, read and parse the input file
    InputParser* input;
    input = new InputParser(argn, argv);

    // Initialize MPI and organize sublattices
    parallelGeom->createNeighborLists(input->latticeSize, input->subLatticeSize);

    // Send Parameters to the Factory and init the system
    GaugeFieldFactory GFF(input, parallelGeom);

    // Start Generating Gauges
    GFF.generateConfigurations();

    // Finalize MPI and delete objects
    parallelGeom->finalize();

    // Stop timer and output
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedTime = end-start;
    if(parallelGeom->getRank() == 0)
        printf("Total time taken: %.2fs\n", elapsedTime.count());

    // End
    delete input;
    delete parallelGeom;
    return 0;
}
*/

