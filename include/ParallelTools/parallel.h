#pragma once
#include <vector>
#include <mpi/mpi.h>

class Parallel{
    public:
        Parallel(int argn, char* argv[]);
        void finalize();
        void createNeighborLists(std::vector<int> &latticeSize,
                                 std::vector<int> &subLatticeSize);
        bool isActive = true;
        int getRank() { return m_rank; }
        MPI_Comm getComm() const { return m_cartCoordComm; }
        int* getFullSize() { return m_fullSize; }
        int* getSubBlocks() { return m_subBlocks; }
        int* getRankSubBlock() { return m_rankCoord; }

        int getNeighbor(int direction, int sign);
        int getSecondNeighbor(int direction1, int sign1, int direction2, int sign2);

        void openFile(MPI_File &file, const char *fileName);
        void closeFile(MPI_File &file);

private:
        MPI_Comm m_cartCoords, m_cartCoordComm;
        int m_rank, m_numProcs;


        int** m_neighbor = new int*[4];
        int**** m_neighbor2 = new int***[4]; //LOL
        int* m_parity = new int[4];
        int* m_fullSize = new int[4];
        int* m_subSize = new int[4];
        int* m_subBlocks = new int[4];
        int* m_rankCoord = new int[4];

        void assignNeighbor(int direction);
        void assignSecondNeighbor(int dir1, int dir2);
        void MyMPI_Cart_shift2(MPI_Comm comm, int dir1, int shift1,
                               int dir2, int shift2, int &source, int &dest);


};
