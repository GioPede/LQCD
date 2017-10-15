#include "ParallelTools/parallel.h"
#include <mpi/mpi.h>
#include <vector>

// CONSTRUCTOR
Parallel::Parallel(int argn, char* argv[]){
    // initialize MPI
    MPI_Init(&argn, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &m_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &m_numProcs);

    // resize vectors
    for(int i = 0; i < 4; i++){
        m_neighbor[i] = new int[2];
        m_neighbor2[i] = new int**[2];
    }
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 2; j++)
            m_neighbor2[i][j] = new int*[4];
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 2; j++)
            for(int k = 0; k < 4; k++)
            m_neighbor2[i][j][k] = new int[2];
}

// FINALIZE PARALLELIZATION
void Parallel::finalize(){
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}

// ASSIGN NEIGHBOURS BASED ON PROCESSOR RANK
void Parallel::createNeighborLists(std::vector<int>& latticeSize, std::vector<int>& subLatticeSize){

    // copy sizes
    m_fullSize = latticeSize.data();
    m_subSize = subLatticeSize.data();

    // count the number of subblocks per dimension
    int subBlocksProd = 1;
    for(int i = 0; i < 4; i++){
        m_subBlocks[i] = latticeSize[i] / subLatticeSize[i];
        subBlocksProd *= m_subBlocks[i];
    }

    // check if there are enough processors to fill the lattice
    if(subBlocksProd > m_numProcs && m_rank == 0){
        printf("ERROR: Too few processors given for specified lattice and sublattice sizes\n");
        MPI_Abort(MPI_COMM_WORLD, 0);
    }

    // MPI_Cart required variables
    int period[4] = {1,1,1,1}; // for periodic boundaries
    int reorder = 0;           // no new rank naming

    // create coordinates map
    MPI_Cart_create(MPI_COMM_WORLD, 4, m_subBlocks, period, reorder, &m_cartCoords);

    // create neighbor lists and intracommunicator
    if(m_rank + 1 <= subBlocksProd){
        MPI_Cart_coords(m_cartCoords, m_rank, 4, m_rankCoord);
        // assign neighbors along all directions
        for(int i = 0; i < 4; i++)
            assignNeighbor(i);

        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                assignSecondNeighbor(i, j);
    }
    // set useless processors to inactive
    else {
        isActive = false;

    }
    int active = isActive == true ? 1 : 0;
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
                          m_neighbor2[dir1][0][dir2][0],      // recv rank
                          m_neighbor2[dir1][1][dir2][1]);     // send rank
        MyMPI_Cart_shift2(m_cartCoords, dir1, -1, dir2, 1,
                          m_neighbor2[dir1][1][dir2][0],      // recv rank
                          m_neighbor2[dir1][0][dir2][1]);     // send rank
    }
}

void Parallel::MyMPI_Cart_shift2(MPI_Comm comm, int dir1, int shift1,
                                 int dir2, int shift2,
                                 int& source, int& dest){
    int coords[4];
    for(int i = 0; i< 4; i++) coords[i] = m_rankCoord[i];
    coords[dir1] += shift1;
    coords[dir2] += shift2;
    MPI_Cart_rank(comm, coords, &dest);
    for(int i = 0; i< 4; i++) coords[i] = m_rankCoord[i];
    coords[dir1] -= shift1;
    coords[dir2] -= shift2;
    MPI_Cart_rank(comm, coords, &source);
}

int Parallel::getNeighbor(int direction, int sign){
    return m_neighbor[direction][sign];
}

int Parallel::getSecondNeighbor(int direction1, int sign1, int direction2, int sign2){
    return m_neighbor2[direction1][sign1][direction2][sign2];
}
