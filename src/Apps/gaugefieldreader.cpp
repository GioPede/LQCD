#include "Utils/clusterspecifier.h"
#ifndef LACONIA
#include <mpi/mpi.h>
#else
//#include <mpi.h>
#endif
#include <vector>
#include <cstdio>
#include <string>
#include <ctime>
#include <cmath>
#include <random>
#include "lqcd.h"


// CONSTRUCT CLASS BASED ON PARALLEL GEOMETRY AND INPUT PARAMETERS
GaugeFieldReader::GaugeFieldReader(InputParser* input, Parallel* parallel){
    setSize(input->subLatticeSize);
    addObservable(new Plaquette());
    m_outDir = input->outDir;
    m_inputConfList = input->inputConfList;

    m_parallel = parallel;
    m_inputConf = new InputConf(this);
}


// CREATE THE LATTICE AND INITIALIZE OBJECTS
void GaugeFieldReader::initGFR(){
    m_lat = new Lattice(m_size, m_parallel);
    for(int i = 0; i < m_obs.size(); i++){
        m_obs[i]->initObservable(m_lat);
    }
}

// MAIN FUNCTION OF CLASS. GENERATES GAUGE FIELD CONFIGURATION USING METROPILIS'
// ALGORITHM AND SAVES THEM FOR FUTURE ANALISYS.
void GaugeFieldReader::sampleConfigurations(){
    // check that current processor should be active
    if(m_parallel->isActive){
        for(int conf = 0; conf < m_inputConfList.size(); conf++){
            m_inputConf->readConfiguration(m_inputConfList[conf].c_str());
            for(int i = 0; i < m_obs.size(); i++)
                m_obs[i]->compute();
            if(m_parallel->getRank() == 0) {
                for(int i = 0; i < m_obs.size(); i++){
                    double temp, value = 0.0;
                    for(int rank = 1; rank <  m_parallel->getSubBlocks()[0] * m_parallel->getSubBlocks()[1] * m_parallel->getSubBlocks()[2] * m_parallel->getSubBlocks()[3] ; rank++){
                        MPI_Recv(&temp, 1, MPI_DOUBLE, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        value += temp;
                    }
                    value += m_obs[i]-> value();
                    printf("Conf %i\t Plaq = %f \n", conf, value / m_parallel->getSubBlocks()[0] / m_parallel->getSubBlocks()[1] / m_parallel->getSubBlocks()[2] / m_parallel->getSubBlocks()[3] );
                }
            }

            else{
                for(int i = 0; i < m_obs.size(); i++){
                    double temp = m_obs[i]-> value();
                    MPI_Send(&temp, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
                }
            }
        }
    }
}


// GETTERS AND SETTERS
Point& GaugeFieldReader::getLatticeSite(int x, int y, int z, int t){
    return (*m_lat)(x,y,z,t);
}

void GaugeFieldReader::setSize(std::vector<int> size){
    m_size = size;
}
void GaugeFieldReader::addObservable(Observable *observable){
    m_obs.push_back(observable);
    m_obsValues.push_back(0.0);
}
