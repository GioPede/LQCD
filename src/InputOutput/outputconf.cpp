#include <cstdio>
#include <mpi/mpi.h>
#include "InputOutput/outputconf.h"
#include "Math/lattice.h"
#include "ParallelTools/parallel.h"
#include "Utils/clusterspecifier.h"

namespace LatticeIO {

    int OutputConf::m_linkSize = 72 * sizeof(double);

    // Write a single file for every sublattice (for testing)
    void OutputConf::writeSubLattice(Lattice& lattice, int confNum){
        char fileName [1024];
        sprintf(fileName, "%s/r%03du%03d", OUT_PREFIX, Parallel::rank(), confNum);
        FILE* output = fopen(fileName, "wb");
        for(int t = 0; t < Parallel::latticeSubSize()[3]; t++){
        for(int z = 0; z < Parallel::latticeSubSize()[2]; z++){
        for(int y = 0; y < Parallel::latticeSubSize()[1]; y++){
        for(int x = 0; x < Parallel::latticeSubSize()[0]; x++){
            fwrite(lattice(x,y,z,t).m_links, sizeof(double), 72, output);
        }}}}
        fclose(output);
    }

    // Use MPI to write a file containing the lattice
    void OutputConf::writeConf(Lattice& lattice, int confNum){
        MPI_File output;
        MPI_Offset startPointT, startPointZ, startPointY, startPointX;
        int volumeX = 1;
        int volumeY = Parallel::latticeFullSize()[0] * volumeX;
        int volumeZ = Parallel::latticeFullSize()[1] * volumeY;
        int volumeT = Parallel::latticeFullSize()[2] * volumeZ;

        // Create filename and open
        char fileName [1024];
        sprintf(fileName, "%s/conf%04d.bin", OUT_PREFIX, confNum);
        Parallel::openFile(output, fileName);

        for(int t = 0; t < Parallel::latticeSubSize()[3]; t++){
            startPointT = volumeT * ( Parallel::rankCoord()[3]*Parallel::latticeSubSize()[3] + t);
            for(int z = 0; z < Parallel::latticeSubSize()[2]; z++){
            startPointZ = startPointT + volumeZ * ( Parallel::rankCoord()[2]*Parallel::latticeSubSize()[2] + z);
                for(int y = 0; y < Parallel::latticeSubSize()[1]; y++){
                startPointY = startPointZ + volumeY * ( Parallel::rankCoord()[1]*Parallel::latticeSubSize()[1] + y);
                    for(int x = 0; x < Parallel::latticeSubSize()[0]; x++){
                        startPointX = startPointY + volumeX * ( Parallel::rankCoord()[0]*Parallel::latticeSubSize()[0] + x);
                        startPointX *= m_linkSize;
                        MPI_File_write_at(output, startPointX, lattice(x,y,z,t).m_links, 72, MPI_DOUBLE, MPI_STATUS_IGNORE);
                    }
                }
            }
        }
        Parallel::closeFile(output);
    }

} // end LatticeIO
