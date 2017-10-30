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
WilsonFlow::WilsonFlow(InputParser* input, Parallel* parallel){
    addObservable(new Plaquette());
    addObservable(new EnergyDensity());
    addObservable(new TopologicalCharge());
    m_size = input->subLatticeSize;

    m_outDir = input->outDir;
    m_inputConfList = input->inputConfList;

    m_parallel = parallel;
    m_outputObs  = new OutputObs(this);
    m_outputTerm = new OutputTerm(this);
    m_inputConf  = new InputConf(this);

    m_lat = new Lattice(m_size, m_parallel);
    m_Z0 = new Lattice(m_size, m_parallel);
    m_Z1 = new Lattice(m_size, m_parallel);
    m_Z2 = new Lattice(m_size, m_parallel);

    for(int i = 0; i < m_obs.size(); i++){
        m_obs[i]->initObservable(m_lat);
    }

    // initialize action
    if(std::string(input->actionTag) == "puregauge")
        m_act = new PureGauge(m_lat, input->beta);
}

// MAIN FUNCTION OF CLASS. GENERATES GAUGE FIELD CONFIGURATION USING METROPILIS'
// ALGORITHM AND SAVES THEM FOR FUTURE ANALISYS.
void WilsonFlow::flowConfigurations(){
    // check that current processor should be active
    int confNum = 0;
    double epsilon = 0.005;
    if(m_parallel->isActive){
        for(auto& conf : m_inputConfList){
            m_inputConf->readConfiguration(conf.c_str());
            computeObservables();
            m_outputTerm->writeObservables(confNum);
            m_outputObs->writeObservables(confNum);
            applyWilsonFlow(confNum, epsilon);
            confNum++;
        }
    }
}

void WilsonFlow::applyWilsonFlow(int confNum, double epsilon){
    std::vector<std::vector<double>> flowObsMatrix;
    flowObsMatrix.resize(int(0.1/epsilon));
    for(auto& slice : flowObsMatrix)
        slice.resize(m_obs.size()+1);
    flowObsMatrix[0][0] = 0.0;
    for(int i = 0; i < m_obs.size(); i++)
        flowObsMatrix[0][i+1] = m_obsValues[i];

    for(int t = 1; t < flowObsMatrix.size(); t++){
        flowStep(epsilon);
        flowObsMatrix[t][0] = flowObsMatrix[t-1][0] + epsilon;
        computeObservables();
        for(int i = 0; i < m_obs.size(); i++)
            flowObsMatrix[t][i+1] = m_obsValues[i];
        m_outputTerm->writeFlowObservables(flowObsMatrix[t]);
    }
    m_outputObs->writeFlowObservables(confNum, flowObsMatrix);
}

void WilsonFlow::flowStep(double epsilon){
    // compute Z0
    for(int x = 0; x < m_size[0]; x++){
    for(int y = 0; y < m_size[1]; y++){
    for(int z = 0; z < m_size[2]; z++){
    for(int t = 0; t < m_size[3]; t++){
        for(int mu = 0; mu < 4; mu++){
            omega = (*m_lat)(x,y,z,t)[mu]*m_act->computeConstant(x, y, z, t, mu);
            double tr = (omega-~omega).imagTrace()/6.0;
            omega = (omega-~omega) * 0.5;
            for(int i = 1; i < 18; i+=2)
                omega.mat[i] -= tr;
            for(int i = 0; i < 18; i+=2){
                double temp = omega.mat[i];
                omega.mat[i] = -omega.mat[i+1] * 0.5;
                omega.mat[i+1] = temp * 0.5;
            }
            (*m_Z0)(x,y,z,t)[mu] = omega;
        }
    }}}}

    // compute W1
    for(int x = 0; x < m_size[0]; x++){
    for(int y = 0; y < m_size[1]; y++){
    for(int z = 0; z < m_size[2]; z++){
    for(int t = 0; t < m_size[3]; t++){
        for(int mu = 0; mu < 4; mu++){
            (*m_lat)(x,y,z,t)[mu] *= exp((*m_Z0)(x,y,z,t)[mu] * epsilon * (1.0/4.0));
        }
    }}}}

    // compute Z1
    for(int x = 0; x < m_size[0]; x++){
    for(int y = 0; y < m_size[1]; y++){
    for(int z = 0; z < m_size[2]; z++){
    for(int t = 0; t < m_size[3]; t++){
        for(int mu = 0; mu < 4; mu++){
            omega = (*m_lat)(x,y,z,t)[mu]*m_act->computeConstant(x, y, z, t, mu);
            double tr = (omega-~omega).imagTrace()/6.0;
            omega = (omega-~omega) * 0.5;
            for(int i = 1; i < 18; i+=2)
                omega.mat[i] -= tr;
            for(int i = 0; i < 18; i+=2){
                double temp = omega.mat[i];
                omega.mat[i] = -omega.mat[i+1] * 0.5;
                omega.mat[i+1] = temp * 0.5;
            }
            (*m_Z1)(x,y,z,t)[mu] = omega;
        }
    }}}}

    // compute W2
    for(int x = 0; x < m_size[0]; x++){
    for(int y = 0; y < m_size[1]; y++){
    for(int z = 0; z < m_size[2]; z++){
    for(int t = 0; t < m_size[3]; t++){
        for(int mu = 0; mu < 4; mu++){
            (*m_lat)(x,y,z,t)[mu] *= exp(((*m_Z1)(x,y,z,t)[mu]*(8.0/9.0)
                                         -(*m_Z0)(x,y,z,t)[mu]*(17.0/36.0))*epsilon);
        }
    }}}}

    // compute Z2
    for(int x = 0; x < m_size[0]; x++){
    for(int y = 0; y < m_size[1]; y++){
    for(int z = 0; z < m_size[2]; z++){
    for(int t = 0; t < m_size[3]; t++){
        for(int mu = 0; mu < 4; mu++){
            omega = (*m_lat)(x,y,z,t)[mu]*m_act->computeConstant(x, y, z, t, mu);
            double tr = (omega-~omega).imagTrace()/6.0;
            omega = (omega-~omega) * 0.5;
            for(int i = 1; i < 18; i+=2)
                omega.mat[i] -= tr;
            for(int i = 0; i < 18; i+=2){
                double temp = omega.mat[i];
                omega.mat[i] = -omega.mat[i+1] * 0.5;
                omega.mat[i+1] = temp * 0.5;
            }
            (*m_Z2)(x,y,z,t)[mu] = omega;
        }
    }}}}

    // compute V_t+eps
    for(int x = 0; x < m_size[0]; x++){
    for(int y = 0; y < m_size[1]; y++){
    for(int z = 0; z < m_size[2]; z++){
    for(int t = 0; t < m_size[3]; t++){
        for(int mu = 0; mu < 4; mu++){
            (*m_lat)(x,y,z,t)[mu] *= exp(((*m_Z2)(x,y,z,t)[mu]*(3.0/4.0)
                                         -(*m_Z1)(x,y,z,t)[mu]*(8.0/9.0)
                                         +(*m_Z0)(x,y,z,t)[mu]*(17.0/36.0))*epsilon);
        }
    }}}}
}



void WilsonFlow::computeObservables(){
    for(int i = 0; i < m_obs.size(); i++){
        m_obs[i]->compute();
        double value = m_obs[i]->value();
        MPI_Allreduce(&value, &m_obsValues[i], 1, MPI_DOUBLE, MPI_SUM, m_parallel->getComm());
    }
    m_obsValues[0] /= m_parallel->getSubBlocks()[0] * m_parallel->getSubBlocks()[1] *
                      m_parallel->getSubBlocks()[2] * m_parallel->getSubBlocks()[3];
    m_obsValues[1] /= m_parallel->getSubBlocks()[0] * m_parallel->getSubBlocks()[1] *
                      m_parallel->getSubBlocks()[2] * m_parallel->getSubBlocks()[3];
}

// GETTERS AND SETTERS
Point& WilsonFlow::getLatticeSite(int x, int y, int z, int t){
    return (*m_lat)(x,y,z,t);
}

void WilsonFlow::setSize(std::vector<int> size){
    m_size = size;
}

void WilsonFlow::setAction(Action* action){
    m_act = action;
}

void WilsonFlow::addObservable(Observable *observable){
    m_obs.push_back(observable);
    m_obsValues.push_back(0.0);
}
