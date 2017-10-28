#include <cstdio>
#include <mpi/mpi.h>
#include "InputOutput/inputconf.h"
#include "Apps/app.h"
#include "Math/lattice.h"
#include "ParallelTools/parallel.h"
#include "Utils/clusterspecifier.h"

// CONSTRUCTOR
InputConf::InputConf(App *app){
    m_app = app;
    m_subSize.resize(4); m_subBlock.resize(4); m_fullSize.resize(4);
    for(int i = 0; i < 4; i++){
        m_subBlock[i] = m_app->getParallel()->getRankSubBlock()[i];
        m_fullSize[i] = m_app->getParallel()->getFullSize()[i];
        m_subSize [i] = m_app->getSize()[i];
    }
    m_linkSize = 72 * sizeof(double);
}

// FUNCTION TO WRITE THE WHOLE SUBLATICE OF A PROCESSOR (FOR TESTING)
void InputConf::readSubLattice(int update){
    char fileName [50];
    sprintf(fileName, "conf/r%du%d", m_app->getParallel()->getRank(), update);
    FILE* input = fopen(fileName, "rb");
    for(int t = 0; t < m_app->getSize()[3]; t++){
    for(int z = 0; z < m_app->getSize()[2]; z++){
    for(int y = 0; y < m_app->getSize()[1]; y++){
    for(int x = 0; x < m_app->getSize()[0]; x++){
        fread(m_app->getLatticeSite(x,y,z,t).m_links, sizeof(double), 72, input);
    }}}}
    fclose(input);
}

// READ CONFIGURATION FROM AN OUTPUT FOLDER TO RESTART GENERATING
void InputConf::readConfiguration(int confNum){
    char fileName[128];
    sprintf(fileName, "%s/%s/conf%04d.bin", OUT_PREFIX, m_app->getOutDir(), confNum);
    readConfiguration(fileName);
}

// READ A CONFIGURATION AND STORE IT IN THE LATTICE
void InputConf::readConfiguration(const char* inputFile){
    MPI_File input;
    MPI_Offset startPointT, startPointZ, startPointY, startPointX;

    // Open file
    m_app->getParallel()->openFile(input, inputFile);

    for(int t = 0; t < m_subSize[3]; t++){
        startPointT = ( m_fullSize[0]*m_fullSize[1]*m_fullSize[2] ) * ( m_subBlock[3]*m_subSize[3] + t);
    for(int z = 0; z < m_subSize[2]; z++){
        startPointZ = startPointT + ( m_fullSize[0]*m_fullSize[1] ) * ( m_subBlock[2]*m_subSize[2] + z);
    for(int y = 0; y < m_subSize[1]; y++){
        startPointY = startPointZ + ( m_fullSize[0] ) * ( m_subBlock[1]*m_subSize[1] + y);
    for(int x = 0; x < m_subSize[0]; x++){
        startPointX = startPointY + ( m_subBlock[0]*m_subSize[0] + x);
        startPointX *= m_linkSize;
        MPI_File_read_at(input, startPointX, m_app->getLatticeSite(x,y,z,t).m_links, 72, MPI_DOUBLE, MPI_STATUS_IGNORE);
    }}}}
    m_app->getParallel()->closeFile(input);
}

