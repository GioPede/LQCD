#include <mpi/mpi.h>
#include <vector>
#include <cstdio>
#include <chrono>
#include <random>
#include "lqcd.h"

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
    m_outputObs  = new OutputObs(this);
    m_outputTerm = new OutputTerm(this);
    m_outputConf = new OutputConf(this);
    m_inputConf  = new InputConf(this);

    // PRNG initialization
    std::random_device rd;
    m_random = new std::mt19937(m_parallel->getRank()+rd());


    // initialize observables list
    addObservable(new Plaquette());
    addObservable(new TopologicalCharge());

    // CREATE THE LATTICE AND INITIALIZE OBJECTS
    m_lat = new Lattice(m_size, m_parallel);
    if(m_startType == 'H' || m_startType == 'h')
        m_lat->setToRandom();
    else if(m_startType == 'C' || m_startType == 'c')
        m_lat->setToUnity();
    for(int i = 0; i < m_obs.size(); i++){
        m_obs[i]->initObservable(m_lat);
    }

    // initialize action
    if(std::string(input->actionTag) == "puregauge")
        m_act = new PureGauge(m_lat, input->beta);
}

// MAIN FUNCTION OF CLASS. GENERATES GAUGE FIELD CONFIGURATION USING METROPILIS'
// ALGORITHM AND SAVES THEM FOR FUTURE ANALISYS.
void GaugeFieldFactory::generateConfigurations(){
    // check that current processor should be active
    if(m_parallel->isActive){

        // initial state
        computeObservables();
        m_outputTerm->printInitialConditions();

        // thermalization
        thermalize();

        // generate and save configurations
        sampleConf();

        m_outputObs->closeFile();
    }
}

void GaugeFieldFactory::thermalize(){
    for(int step = 1; step <= m_thermSteps; step++){
        // perform an update
        MCUpdate();

        // compute and print information on current state
        if(step % m_correlationSteps == 0){
            computeObservables();
            m_outputTerm->printThermSteps(step, double(m_accepted)/double(m_updates));
        }
    }
}

void GaugeFieldFactory::sampleConf(){
    int confNum = 0;
    for(int step = 1; step <= m_MCSteps; step++){
        // perform an update
        MCUpdate();

        // compute and print information on current state
        if(step % m_correlationSteps == 0){
            computeObservables();
            m_outputTerm->printGenerationStep(confNum, double(m_accepted)/double(m_updates));
            m_outputConf->writeConfiguration(confNum);
            m_outputObs->writeObservables(step);
            confNum++;
        }
    }
}

void GaugeFieldFactory::thermalizeTime(){
    auto thermStart = std::chrono::system_clock::now();
    auto thermStepStart = std::chrono::system_clock::now();
    for(int step = 1; step <= m_thermSteps; step++){
        // perform an update
        MCUpdate();

        // compute and print information on current state
        if(step % m_correlationSteps == 0){
            computeObservables();
            m_outputTerm->printThermSteps(step, double(m_accepted)/double(m_updates));
            std::chrono::duration<double> thermStepTime = std::chrono::system_clock::now()-thermStepStart;
            if(m_parallel->getRank() == 0)
                printf("\tThermalization Step Time:  %lf s\n\n", thermStepTime.count());
            thermStepStart = std::chrono::system_clock::now();
        }
    }
    std::chrono::duration<double> thermTime = std::chrono::system_clock::now()-thermStart;
    if(m_parallel->getRank() == 0){
        printf("Total Thermalization Steps: %i\n", m_thermSteps);
        printf("Total Thermalization Time:  %lf s\n\n", thermTime.count());
    }
}

void GaugeFieldFactory::sampleConfTime(){
    int confNum = 0;
    auto confUpdateStart = std::chrono::system_clock::now();
    auto writeStart = std::chrono::system_clock::now();
    for(int step = 1; step <= m_MCSteps; step++){
        // perform an update
        MCUpdate();

        // compute and print information on current state
        if(step % m_correlationSteps == 0){
            computeObservables();
            m_outputTerm->printGenerationStep(confNum, double(m_accepted)/double(m_updates));
            std::chrono::duration<double> confUpdateTime = std::chrono::system_clock::now()-confUpdateStart;
            if(m_parallel->getRank() == 0)
                printf("\tConfiguration Update:  %lf s\n", confUpdateTime.count());
            writeStart = std::chrono::system_clock::now();
            m_outputConf->writeConfiguration(confNum);
            m_outputObs->writeObservables(step);
            std::chrono::duration<double> writeTime = std::chrono::system_clock::now()-writeStart;
            if(m_parallel->getRank() == 0)
                printf("\tWrite Time:            %lf s\n\n", writeTime.count());
            confUpdateStart = std::chrono::system_clock::now();
            confNum++;
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

void GaugeFieldFactory::computeObservables(){
    for(int i = 0; i < m_obs.size(); i++){
        m_obs[i]->compute();
        double value = m_obs[i]->value();
        MPI_Allreduce(&value, &m_obsValues[i], 1, MPI_DOUBLE, MPI_SUM, m_parallel->getComm());
    }
    m_obsValues[0] /= m_parallel->getSubBlocks()[0] * m_parallel->getSubBlocks()[1] *
                      m_parallel->getSubBlocks()[2] * m_parallel->getSubBlocks()[3];
}


// GETTERS AND SETTERS
Point& GaugeFieldFactory::getLatticeSite(int x, int y, int z, int t){
    return (*m_lat)(x,y,z,t);
}

void GaugeFieldFactory::addObservable(Observable *observable){
    m_obs.push_back(observable);
    m_obsValues.push_back(0.0);
}
