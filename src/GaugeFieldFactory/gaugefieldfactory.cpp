#include <mpi/mpi.h>
#include <vector>
#include <cstdio>
#include <string>
#include <ctime>
#include <cmath>
#include <random>
#include "../Math/random.h"
#include "gaugefieldfactory.h"
#include "../InputOutput/inputparser.h"
#include "../InputOutput/outputconf.h"
#include "../InputOutput/inputconf.h"
#include "../Utils/lattice.h"
#include "../Action/action.h"
#include "../Action/actionlist.h"
#include "../Math/random.h"
#include "../Observables/observable.h"
#include "../ParallelTools/parallel.h"

std::uniform_real_distribution<double> randomUniform(0.0, 1.0);

// CONSTRUCT CLASS BASED ON PARALLEL GEOMETRY AND INPUT PARAMETERS
GaugeFieldFactory::GaugeFieldFactory(InputParser* input, Parallel* parallel){

    // set parameters from input
    m_size = input->subLatticeSize;
    m_MCSteps = input->MCSteps;
    m_epsilon = input->MCUpdateParameter;
    m_thermSteps = input->thermSteps;
    m_confs = input->nConf;
    m_correlationSteps = m_MCSteps / m_confs;
    m_outDir = input->outDir;
    m_startType = input->startType;

    // assign or create sublbclasses
    m_parallel = parallel;
    m_outputConf = new OutputConf(this);
    m_inputConf = new InputConf(this);

    // PRNG initialization
    std::random_device rd;
    m_random = new std::mt19937(m_parallel->getRank()+rd());

    // initialize action
    if(std::string(input->actionTag) == "puregauge")
        m_act = new PureGauge(input->beta);

    // initialize observables list
    addObservable(new Plaquette());

}


// CREATE THE LATTICE AND INITIALIZE OBJECTS
void GaugeFieldFactory::initGFF(){
    m_seed = -time(NULL) + m_parallel->getRank();
    m_lat = new Lattice(m_size, m_parallel);
    if(m_startType == 'H' || m_startType == 'h'){
        m_lat->setToRandom();
        m_startType = 'H';
    }
    else if(m_startType == 'C' || m_startType == 'c'){
        m_lat->setToUnity();
        m_startType = 'C';
    }
	
    m_act->initAction(m_lat);
    for(int i = 0; i < m_obs.size(); i++){
        m_obs[i]->initObservable(m_lat);
    }
}

// MAIN FUNCTION OF CLASS. GENERATES GAUGE FIELD CONFIGURATION USING METROPILIS'
// ALGORITHM AND SAVES THEM FOR FUTURE ANALISYS.
void GaugeFieldFactory::generateConfigurations(){
    // check that current processor should be active
    if(m_parallel->isActive){
        /*
        clock_t start = clock();
        for(int i = 0; i < 2000; i++){
            MCUpdate();

            if((i+1) % 200 == 0){
                if(m_parallel->getRank() == 0) printf("Update Time:\t%f\n", double(clock()-start)/CLOCKS_PER_SEC);
                start = clock();
                clock_t startW = clock();
                m_outputConf->writeConfiguration(i+1);
                if(m_parallel->getRank() == 0) printf("Write Time:\t%f\n", double(clock()-startW)/CLOCKS_PER_SEC);
            }
        }

        int x = 0, y = 0, z = 0, t = 3, mu = 2;

        MCUpdate();
        m_outputConf->writeConfiguration(0);
        if(m_parallel->getRank() == 4) m_lat->m_lattice[x][y][z][t].m_links[mu].printSU3();
        MCUpdate();
        if(m_parallel->getRank() == 4) m_lat->m_lattice[x][y][z][t].m_links[mu].printSU3();

        m_inputConf->readConfiguration(0);
        if(m_parallel->getRank() == 4) m_lat->m_lattice[x][y][z][t].m_links[mu].printSU3();
        */


        // initial state
        for(int i = 0; i < m_obs.size(); i++)
            m_obs[i]->compute();
        if(m_parallel->getRank() == 0) {
            printf("Initial State:\n");
            for(int i = 0; i < m_obs.size(); i++)
                printf("\t Plaq = %lf \n", m_obs[i]->value());
        }
        clock_t start = clock();
        // thermalization
        for(int step = 0; step < m_thermSteps; step++){
            //clock_t start = clock();
            // perform an update
            MCUpdate();

            // compute and print information on current state
            if(step % m_correlationSteps == 0){
                for(int i = 0; i < m_obs.size(); i++)
                    m_obs[i]->compute();
                if(m_parallel->getRank() == 0) {
                    printf("Thermalization Step: %i\n", step);
                    for(int i = 0; i < m_obs.size(); i++){
                        double temp, value = 0.0;
                        for(int rank = 1; rank <  m_parallel->getSubBlocks()[0] * m_parallel->getSubBlocks()[1] * m_parallel->getSubBlocks()[2] * m_parallel->getSubBlocks()[3] ; rank++){
                            MPI_Recv(&temp, 1, MPI_DOUBLE, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                            value += temp;
                        }
                        value += m_obs[i]-> value();
                        printf("\t Plaq = %f \n", value / m_parallel->getSubBlocks()[0] / m_parallel->getSubBlocks()[1] / m_parallel->getSubBlocks()[2] / m_parallel->getSubBlocks()[3] );
                    }
                    printf("\t Current Acceptance Ratio: %f \n", double(m_accepted)/double(m_updates));
                }

                else{
                    for(int i = 0; i < m_obs.size(); i++){
                        double temp = m_obs[i]-> value();
                        MPI_Send(&temp, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
                    }
                }
            }
            //if(m_parallel->getRank() == 0) printf("update time: %f\n", double(clock()-start)/CLOCKS_PER_SEC);

        }
        if(m_parallel->getRank() == 0) printf("Thermalization Time: %f\n", double(clock()-start)/CLOCKS_PER_SEC);

        // generate and save configurations
        int confNum = 0;
        for(int step = 0; step < m_MCSteps; step++){
            start = clock();

            // perform an update
            MCUpdate();

            // compute and print information on current state
            if(step % m_correlationSteps == 0){
                for(int i = 0; i < m_obs.size(); i++)
                    m_obs[i]->compute();
                if(m_parallel->getRank() == 0) {
                    printf("Configuration Number: %i\n", step / m_correlationSteps);
                    for(int i = 0; i < m_obs.size(); i++){
                        double temp, value = 0.0;
                        for(int rank = 1; rank <  m_parallel->getSubBlocks()[0] * m_parallel->getSubBlocks()[1] * m_parallel->getSubBlocks()[2] * m_parallel->getSubBlocks()[3] ; rank++){
                            MPI_Recv(&temp, 1, MPI_DOUBLE, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                            value += temp;
                        }
                        value += m_obs[i]-> value();
                        printf("\t Plaq = %f \n", value / m_parallel->getSubBlocks()[0] / m_parallel->getSubBlocks()[1] / m_parallel->getSubBlocks()[2] / m_parallel->getSubBlocks()[3] );
                    }
                    printf("\t Current Acceptance Ratio: %f \n", double(m_accepted)/double(m_updates));
                }

                else{
                    for(int i = 0; i < m_obs.size(); i++){
                        double temp = m_obs[i]-> value();
                        MPI_Send(&temp, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
                    }
                }
                if(m_parallel->getRank() == 0) printf("Update Time: %f\n", double(clock()-start)/CLOCKS_PER_SEC);

                // write configuration
                //m_outputConf->writeSubLattice(confNum);
                m_outputConf->writeConfiguration(confNum);
                confNum++;
            }

        }
    }
}

// FUNCTION TO PERFORM ONE SINGLE UPDATE ON EVERY ELEMENT OF THE LATTICE
void GaugeFieldFactory::MCUpdate(){
    // loop through the whole sub-lattice
    for(int x = 0; x < m_size[0]; x++){
    for(int y = 0; y < m_size[1]; y++){
    for(int z = 0; z < m_size[2]; z++){
    for(int t = 0; t < m_size[3]; t++){
        for(int mu = 0; mu < 4; mu++){
            updateLink(x,y,z,t,mu);
        }
    }}}}
}

// TRIES TO UPDATE A SINGLE LINK 10 TIMES (HITS)
void GaugeFieldFactory::updateLink(int x,int y, int z, int t, int mu){
    SU3 newLink, constPart;
    // get the local staples' sum
    constPart = m_act->computeConstant(x, y, z, t, mu);

    // try 10 hits on the current link
    for(int i = 0; i < 10; i++){
        newLink = getRandomTransformation(m_epsilon) * (*m_lat)(x,y,z,t)[mu] ;

        // metropolis accept/reject
        if( exp(-m_act->compute(x, y, z, t, mu, newLink, constPart)) > randomUniform(*m_random)){
            (*m_lat)(x,y,z,t)[mu] = newLink;
            m_accepted++;
        }
        m_updates++;
    }
}


// GETTERS AND SETTERS
Point& GaugeFieldFactory::getLatticeSite(int x, int y, int z, int t){
    return (*m_lat)(x,y,z,t);
}

void GaugeFieldFactory::addObservable(Observable *observable){
    m_obs.push_back(observable);
}
