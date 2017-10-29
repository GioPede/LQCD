#include "Math/lattice.h"
#include "Math/point.h"
#include "Math/su3.h"
#include "ParallelTools/parallel.h"
#include <cstdio>
#include <mpi/mpi.h>

Lattice::Lattice(std::vector<int> size){
    m_size = size;
    m_lattice.resize(m_size[0]);
    for(int i = 0; i < m_size[0]; i++){
        m_lattice[i].resize(m_size[1]);
    }
    for(int i = 0; i < m_size[0]; i++){
        for(int j = 0; j < m_size[1]; j++){
            m_lattice[i][j].resize(m_size[2]);
        }
    }
    for(int i = 0; i < m_size[0]; i++){
        for(int j = 0; j < m_size[1]; j++){
            for(int k = 0; k < m_size[2]; k++){
                m_lattice[i][j][k].resize(m_size[3]);
            }
        }
    }
}


// Initialize Lattice of size m_size
Lattice::Lattice(std::vector<int> size, Parallel* parallel){
    m_parallel = parallel;
    m_size = size;
    m_lattice.resize(m_size[0]);
    for(int i = 0; i < m_size[0]; i++){
        m_lattice[i].resize(m_size[1]);
    }
    for(int i = 0; i < m_size[0]; i++){
        for(int j = 0; j < m_size[1]; j++){
            m_lattice[i][j].resize(m_size[2]);
        }
    }
    for(int i = 0; i < m_size[0]; i++){
        for(int j = 0; j < m_size[1]; j++){
            for(int k = 0; k < m_size[2]; k++){
                m_lattice[i][j][k].resize(m_size[3]);
            }
        }
    }
}

void Lattice::setToRandom(){
    for(int x = 0; x < m_size[0]; x++){
    for(int y = 0; y < m_size[1]; y++){
    for(int z = 0; z < m_size[2]; z++){
    for(int t = 0; t < m_size[3]; t++){
        for(int mu = 0; mu < 4; mu++){
            m_lattice[x][y][z][t][mu].setSU3Random();
        }
    }}}}
}

void Lattice::setToUnity(){
    for(int x = 0; x < m_size[0]; x++){
    for(int y = 0; y < m_size[1]; y++){
    for(int z = 0; z < m_size[2]; z++){
    for(int t = 0; t < m_size[3]; t++){
        for(int mu = 0; mu < 4; mu++){
            m_lattice[x][y][z][t][mu].setSU3Identity();
        }
    }}}}
}

SU3 Lattice::shift(int x, int y, int z, int t, int mu, int shiftDir, int shiftSign){
    std::vector<int> idx = {x,y,z,t};
    int mpiShifts = 0, mpiShiftDir = -1, mpiShiftSign;

    // set the index for the new link and compute the MPI calls to perform
    for(int i = 0; i < 4; i++){
        if(shiftDir == i) idx[i] += shiftSign;

        if(idx[i] == -1) {
            mpiShifts++;
            mpiShiftDir = i;
        }
        if(idx[i] == m_size[i]){
            mpiShifts++;
            mpiShiftDir = i;
        }
    }

    // if there is no MPI call
    if(mpiShifts == 0) return m_lattice[idx[0]][idx[1]][idx[2]][idx[3]][mu];

    // if only one MPI call is needed
    if(mpiShifts == 1){
        std::vector<int> sendIdx = {x,y,z,t};
        for(int i = 0; i < 4; i++){
            if(mpiShiftDir == i && idx[i] == -1){
                sendIdx[i] = m_size[i]-1;
                mpiShiftSign = 1;
            }
            else if(mpiShiftDir == i && idx[i] == m_size[i]){
                sendIdx[i] = 0;
                mpiShiftSign = 0;
            }
            else
                sendIdx[i] = idx[i];

        }
        SU3 msg;
        MPI_Sendrecv(m_lattice[sendIdx[0]][sendIdx[1]][sendIdx[2]][sendIdx[3]][mu].mat.data(), 18, MPI_DOUBLE, m_parallel->getNeighbor(mpiShiftDir, mpiShiftSign), 0,
                     msg.mat.data(), 18, MPI_DOUBLE, m_parallel->getNeighbor(mpiShiftDir, abs(mpiShiftSign-1)), 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        return msg;
    }
}

SU3 Lattice::shift2(int x, int y, int z, int t, int mu, int shiftDir, int shiftSign, int shiftDir2, int shiftSign2){
    std::vector<int> idx = {x,y,z,t};
    int mpiShifts = 0, mpiShiftDir = -1, mpiShiftDir2, mpiShiftSign, mpiShiftSign2;

    // set the index for the new link and compute the MPI calls to perform
    for(int i = 0; i < 4; i++){
        if(shiftDir == i) idx[i] += shiftSign;
        if(shiftDir2 == i) idx[i] += shiftSign2;

        if(idx[i] == -1) {
            mpiShifts++;
            if(mpiShiftDir == -1) mpiShiftDir = i;
            else mpiShiftDir2 = i;
        }
        if(idx[i] == m_size[i]){
            mpiShifts++;
            if(mpiShiftDir == -1) mpiShiftDir = i;
            else mpiShiftDir2 = i;
        }
    }

    // if there is no MPI call
    if(mpiShifts == 0) return m_lattice[idx[0]][idx[1]][idx[2]][idx[3]][mu];

    // if only one MPI call is needed
    if(mpiShifts == 1){
        std::vector<int> sendIdx = {x,y,z,t};
        for(int i = 0; i < 4; i++){
            if(mpiShiftDir == i && idx[i] == -1){
                sendIdx[i] = m_size[i]-1;
                mpiShiftSign = 1;
            }
            else if(mpiShiftDir == i && idx[i] == m_size[i]){
                sendIdx[i] = 0;
                mpiShiftSign = 0;
            }
            else
                sendIdx[i] = idx[i];

        }
        SU3 msg;
        MPI_Sendrecv(m_lattice[sendIdx[0]][sendIdx[1]][sendIdx[2]][sendIdx[3]][mu].mat.data(), 18, MPI_DOUBLE, m_parallel->getNeighbor(mpiShiftDir, mpiShiftSign), 0,
                     msg.mat.data(), 18, MPI_DOUBLE, m_parallel->getNeighbor(mpiShiftDir, abs(mpiShiftSign-1)), 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        return msg;
    }

    if(mpiShifts == 2){
        SU3 msg;

        // first shift
        std::vector<int> sendIdx = {x,y,z,t};
        for(int i = 0; i < 4; i++){
            if(mpiShiftDir == i && idx[i] == -1){
                sendIdx[i] = m_size[i]-1;
                mpiShiftSign = 1;
            }
            else if(mpiShiftDir == i && idx[i] == m_size[i]){
                sendIdx[i] = 0;
                mpiShiftSign = 0;
            }
            else if(mpiShiftDir2 == i && idx[i] == -1){
                sendIdx[i] = m_size[i]-1;
                mpiShiftSign2 = 1;
            }
            else if(mpiShiftDir2 == i && idx[i] == m_size[i]){
                sendIdx[i] = 0;
                mpiShiftSign2 = 0;
            }
            else
                sendIdx[i] = idx[i];
        }
        MPI_Sendrecv(m_lattice[sendIdx[0]][sendIdx[1]][sendIdx[2]][sendIdx[3]][mu].mat.data(), 18, MPI_DOUBLE,
                     m_parallel->getSecondNeighbor(mpiShiftDir, mpiShiftSign, mpiShiftDir2, mpiShiftSign2), 0,
                     msg.mat.data(), 18, MPI_DOUBLE, m_parallel->getSecondNeighbor(mpiShiftDir, abs(mpiShiftSign-1), mpiShiftDir2, abs(mpiShiftSign2-1)), 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        return msg;
    }
}
