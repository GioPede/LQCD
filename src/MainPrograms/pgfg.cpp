#include "Utils/clusterspecifier.h"
#include "Math/su3.h"
#include "Math/point.h"
#include "Math/lattice.h"
#include "ParallelTools/parallel.h"
#include "InputOutput/inputparser.h"
#include "Apps/gaugefieldfactory.h"
#include "Action/puregauge.h"
#include "Observables/plaquette.h"
#include <stdio.h>
#include <vector>
#include <string>
#include <mpi.h>


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
    clock_t tStart = clock();

    // Start MPI
    Parallel* parallelGeom = new Parallel(argn, argv);

    // Check argument count, read and parse the input file
    InputParser* input;
    input = new InputParser(argn, argv);

    // Initialize MPI and organize sublattices
    parallelGeom->createNeighborLists(input->latticeSize, input->subLatticeSize);

    // Send Parameters to the Factory and init the system
    GaugeFieldFactory GFF(input, parallelGeom);

    //Init the System
    GFF.initGFF();

    // Start Generating Gauges
    GFF.generateConfigurations();

    // Finalize MPI and delete objects
    MPI_Barrier(MPI_COMM_WORLD);
    parallelGeom->finalize();

    if(parallelGeom->getRank() == 0)  printf("Total time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    delete input;
    delete parallelGeom;

    return 0;
}

