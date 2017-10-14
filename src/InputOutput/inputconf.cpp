#include <cstdio>
#include <cstdlib>
#include <mpi.h>
#include "InputOutput/inputconf.h"
#include "Apps/app.h"
#include "Math/lattice.h"
#include "ParallelTools/parallel.h"

// CONSTRUCTOR
InputConf::InputConf(App *app){
    m_app = app;
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

// WRITE OUT A FILE FOR EACH TIME SLICE
void InputConf::readConfiguration(int confNum){
    MPI_File input;
    MPI_Offset startPointT, startPointZ, startPointY, startPointX, end;

    // Identify the block the processor belongs to
    int* subBlock = m_app->getParallel()->getRankSubBlock();
    int* fullSize = m_app->getParallel()->getFullSize();
    std::vector<int> subSize = m_app->getSize();

    // create filename and open
    char fileName [50];
    //sprintf(fileName, "conf/%s%c/beta%.2lf_conf%d.bin", m_app->getOutDir(), m_app->getStartType(), 6.0, confNum);
    sprintf(fileName, "conf/conf21.bin", m_app->getOutDir(), m_app->getStartType(), 6.0, confNum);
    MPI_File_open(m_app->getParallel()->getComm(), fileName, MPI_MODE_CREATE|MPI_MODE_RDWR,
                  MPI_INFO_NULL, &input);

    //end = 72 * sizeof(double) * fullSize[0] * fullSize[1] * fullSize[2] * fullSize[3];
    //MPI_File_set_size(input, end);
    for(int t = 0; t < m_app->getSize()[3]; t++){
        startPointT = ( fullSize[0]*fullSize[1]*fullSize[2] ) * ( subBlock[3]*subSize[3] + t) * 72 * sizeof(double);
    for(int z = 0; z < m_app->getSize()[2]; z++){
        startPointZ = startPointT + ( fullSize[0]*fullSize[1] ) * ( subBlock[2]*subSize[2] + z) * 72 * sizeof(double);
    for(int y = 0; y < m_app->getSize()[1]; y++){
        startPointY = startPointZ + ( fullSize[0] ) * ( subBlock[1]*subSize[1] + y) * 72 * sizeof(double);
    for(int x = 0; x < m_app->getSize()[0]; x++){
        startPointX = startPointY + ( subBlock[0]*subSize[0] + x) * 72 * sizeof(double);
        MPI_File_read_at(input, startPointX, m_app->getLatticeSite(x,y,z,t).m_links, 72, MPI_DOUBLE, MPI_STATUS_IGNORE);
    }}}}

    MPI_File_close(&input);
}

void InputConf::readConfiguration(std::string inputFile){
    MPI_File input;
    MPI_Offset startPointT, startPointZ, startPointY, startPointX, end;

    // Identify the block the processor belongs to
    int* subBlock = m_app->getParallel()->getRankSubBlock();
    int* fullSize = m_app->getParallel()->getFullSize();
    std::vector<int> subSize = m_app->getSize();

    // create filename and open
    MPI_File_open(m_app->getParallel()->getComm(), inputFile.c_str(), MPI_MODE_CREATE|MPI_MODE_RDWR,
                  MPI_INFO_NULL, &input);

    //end = 72 * sizeof(double) * fullSize[0] * fullSize[1] * fullSize[2] * fullSize[3];
    //MPI_File_set_size(input, end);
    for(int t = 0; t < m_app->getSize()[3]; t++){
        startPointT = ( fullSize[0]*fullSize[1]*fullSize[2] ) * ( subBlock[3]*subSize[3] + t) * 72 * sizeof(double);
    for(int z = 0; z < m_app->getSize()[2]; z++){
        startPointZ = startPointT + ( fullSize[0]*fullSize[1] ) * ( subBlock[2]*subSize[2] + z) * 72 * sizeof(double);
    for(int y = 0; y < m_app->getSize()[1]; y++){
        startPointY = startPointZ + ( fullSize[0] ) * ( subBlock[1]*subSize[1] + y) * 72 * sizeof(double);
    for(int x = 0; x < m_app->getSize()[0]; x++){
        startPointX = startPointY + ( subBlock[0]*subSize[0] + x) * 72 * sizeof(double);
        MPI_File_read_at(input, startPointX, m_app->getLatticeSite(x,y,z,t).m_links, 72, MPI_DOUBLE, MPI_STATUS_IGNORE);
    }}}}

    MPI_File_close(&input);
}
