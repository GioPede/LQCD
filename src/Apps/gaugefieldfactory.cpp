#include <vector>
#include <cstdio>
#include <chrono>
#include "lqcd.h"

GaugeFieldFactory::GaugeFieldFactory(int MCSteps, int thermSteps,
                                     int NConf, double epsilon,
                                     std::string startType)
                 : App() {
    m_MCSteps = MCSteps;
    m_thermSteps = thermSteps;
    m_confs = NConf;
    m_correlationSteps = m_MCSteps / m_confs;
    m_epsilon = epsilon;
    m_startType = startType;
}

void GaugeFieldFactory::execute(){
    initialize();
    generateConfigurations();
}

void GaugeFieldFactory::initialize(){
    if(m_startType == "Cold")
        m_lat->setToUnity();
    else if (m_startType == "Hot")
        m_lat->setToRandom();

    for(auto& obs : m_obs)
        obs->initObservable(m_lat);
    m_act->initAction(m_lat);
    m_obsValues.resize(m_obs.size());

    LatticeIO::OutputTerm::printInitialConditions();
    LatticeIO::OutputObs::initialize(m_obs);
    LatticeIO::InputConf::getInputList(m_inputConfList);

}

// MAIN FUNCTION OF CLASS. GENERATES GAUGE FIELD CONFIGURATION USING METROPILIS'
// ALGORITHM AND SAVES THEM FOR FUTURE ANALISYS.
void GaugeFieldFactory::generateConfigurations(){
    // check that current processor should be active
    if(Parallel::isActive()){

        // thermalization
        thermalize();

        // generate and save configurations
        sampleConf();
    }
}

void GaugeFieldFactory::thermalize(){
    for(int step = 1; step <= m_thermSteps; step++){
        // perform an update
        MCUpdate();

        // compute and print information on current state
        if(step % m_correlationSteps == 0){
            computeObservables();
            LatticeIO::OutputTerm::printThermStep(step, m_obs, double(m_accepted)/double(m_updates));
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
            LatticeIO::OutputObs::writeObs(m_obs, step);
            LatticeIO::OutputTerm::printGenStep(confNum, m_obs, double(m_accepted)/double(m_updates));
            LatticeIO::OutputConf::writeConf(*m_lat, confNum);
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
            LatticeIO::OutputTerm::printThermStep(step, m_obs, double(m_accepted)/double(m_updates));
            std::chrono::duration<double> thermStepTime = std::chrono::system_clock::now()-thermStepStart;
            if(Parallel::rank() == 0)
                printf("\tThermalization Step Time:  %lf s\n\n", thermStepTime.count());
            thermStepStart = std::chrono::system_clock::now();
        }
    }
    std::chrono::duration<double> thermTime = std::chrono::system_clock::now()-thermStart;
    if(Parallel::rank() == 0){
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
            LatticeIO::OutputTerm::printThermStep(confNum, m_obs, double(m_accepted)/double(m_updates));
            std::chrono::duration<double> confUpdateTime = std::chrono::system_clock::now()-confUpdateStart;
            if(Parallel::rank() == 0)
                printf("\tConfiguration Update:  %lf s\n", confUpdateTime.count());
            writeStart = std::chrono::system_clock::now();
            LatticeIO::OutputConf::writeConf(*m_lat, confNum);
            LatticeIO::OutputObs::writeObs(m_obs, step);
            std::chrono::duration<double> writeTime = std::chrono::system_clock::now()-writeStart;
            if(Parallel::rank() == 0)
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
    m_act->computeStaples(x, y, z, t, mu);

    // try 10 hits on the current link
    for(int i = 0; i < 30; i++){
        newLink = Random::randSU3Transf(m_epsilon) * (*m_lat)(x,y,z,t)[mu] ;

        // metropolis accept/reject
        if( exp(-m_act->compute(x, y, z, t, mu, newLink)) > Random::randUniform()){
            (*m_lat)(x,y,z,t)[mu] = newLink;
            m_accepted++;
        }
        m_updates++;
    }
}

void GaugeFieldFactory::computeObservables(){
    for(auto& obs : m_obs)
        obs->compute();
}

// GETTERS AND SETTERS
Point& GaugeFieldFactory::getLatticeSite(int x, int y, int z, int t){
    return (*m_lat)(x,y,z,t);
}
