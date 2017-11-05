#include "Utils/clusterspecifier.h"
#ifndef LACONIA
#include <mpi/mpi.h>
#else
#include <mpi.h>
#endif
#include "ParallelTools/parallel.h"
#include <array>

// Initialize static variables
int Parallel::m_rank;
int Parallel::m_numProcs;
int Parallel::m_activeProcs;
bool Parallel::m_isActive;
MPI_Comm Parallel::m_cartCoords;
MPI_Comm Parallel::m_cartCoordComm;
std::array<int, 4> Parallel::m_subBlocks;
std::array<int, 4> Parallel::m_rankCoord;
std::array<int, 4> Parallel::m_latticeSubSize;
std::array<int, 4> Parallel::m_latticeFullSize;
std::array<int, 4> Parallel::m_parity;
std::array< std::array<int, 2>, 4> Parallel::m_neighbor;
std::array< std::array<std::array<std::array<int, 2>, 4>, 2>, 4> Parallel::m_secondNeighbor;


// Initialization
void Parallel::initialize(){
    // initialize MPI
    int argn = 1;
    char** argv;
    MPI_Init(&argn, &argv);

    // Get rank and number of processors
    MPI_Comm_rank(MPI_COMM_WORLD, &m_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &m_numProcs);
}

void Parallel::createGeometry(std::array<int, 4> latticeSize,
                              std::array<int, 4> subLatticeSize){

    // Fix sizes from input
    m_latticeSubSize = subLatticeSize;
    m_latticeFullSize = latticeSize;

    // Create castesian coordinates neighbor lists
    createNeighborLists();

}

// Finalize Parallelization
void Parallel::finalize(){
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}

// Create castesian coordinates neighbor lists
void Parallel::createNeighborLists(){

    // Count the number of subblocks per dimension
    int subBlocksProd = 1;
    for(int i = 0; i < 4; i++){
        m_subBlocks[i] = m_latticeFullSize[i] / m_latticeSubSize[i];
        subBlocksProd *= m_subBlocks[i];
    }

    // Check if there are enough processors to fill the lattice
    if(subBlocksProd > m_numProcs && m_rank == 0){
        printf("ERROR: Too few processors given for specified lattice and sublattice sizes\n");
        MPI_Abort(MPI_COMM_WORLD, 0);
    } else {
        m_activeProcs = subBlocksProd;
    }

    // MPI_Cart required variables
    std::array<int, 4> period = {1,1,1,1}; // for periodic boundaries
    int reorder = 0;                       // no new rank naming

    // Create coordinates map
    MPI_Cart_create(MPI_COMM_WORLD, 4, m_subBlocks.data(), period.data(), reorder, &m_cartCoords);

    // Create neighbor lists and intercommunicator
    if(m_rank + 1 <= m_activeProcs){
        m_isActive = true;
        MPI_Cart_coords(m_cartCoords, m_rank, 4, m_rankCoord.data());
        // Assign neighbors along all directions
        for(int i = 0; i < 4; i++)
            assignNeighbor(i);

        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                assignSecondNeighbor(i, j);
    } else {
        // Set extra processors to inactive
        m_isActive = false;
    }

    // Create intracommunicator
    int active = m_isActive == true ? 1 : 0;
    MPI_Comm_split(MPI_COMM_WORLD, active, 1, &m_cartCoordComm);
}

void Parallel::openFile(MPI_File &file, const char* fileName){
    MPI_File_open(m_cartCoordComm, fileName, MPI_MODE_CREATE|MPI_MODE_RDWR,
                  MPI_INFO_NULL, &file);
}

void Parallel::closeFile(MPI_File &file){
    MPI_File_close(&file);
}

void Parallel::assignNeighbor(int direction){
    MPI_Cart_shift(m_cartCoords, direction, 1,
                   &m_neighbor[direction][0],     // recv rank
                   &m_neighbor[direction][1]);    // send rank
}

void Parallel::assignSecondNeighbor(int dir1, int dir2){
    if(dir1 != dir2){
        MyMPI_Cart_shift2(m_cartCoords, dir1, 1, dir2, 1,
                          m_secondNeighbor[dir1][0][dir2][0],      // recv rank
                          m_secondNeighbor[dir1][1][dir2][1]);     // send rank
        MyMPI_Cart_shift2(m_cartCoords, dir1, -1, dir2, 1,
                          m_secondNeighbor[dir1][1][dir2][0],      // recv rank
                          m_secondNeighbor[dir1][0][dir2][1]);     // send rank
    }
}

void Parallel::MyMPI_Cart_shift2(MPI_Comm comm, int dir1, int shift1,
                                                int dir2, int shift2,
                                                int& source, int& dest){
    std::array<int, 4> coords;
    for(int i = 0; i< 4; i++) coords[i] = m_rankCoord[i];
    coords[dir1] += shift1;
    coords[dir2] += shift2;
    MPI_Cart_rank(comm, coords.data(), &dest);
    for(int i = 0; i< 4; i++) coords[i] = m_rankCoord[i];
    coords[dir1] -= shift1;
    coords[dir2] -= shift2;
    MPI_Cart_rank(comm, coords.data(), &source);
}

int Parallel::getNeighbor(int direction, int sign){
    return m_neighbor[direction][sign];
}

int Parallel::getSecondNeighbor(int direction1, int sign1,
                                int direction2, int sign2){
    return m_secondNeighbor[direction1][sign1][direction2][sign2];
}
