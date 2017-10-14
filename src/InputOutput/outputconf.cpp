#include <cstdio>
#include <cstdlib>
#include <mpi.h>
#include "InputOutput/outputconf.h"
#include "Apps/app.h"
#include "Math/lattice.h"
#include "ParallelTools/parallel.h"

// CONSTRUCTOR
OutputConf::OutputConf(App *app){
    m_app = app;
}

// FUNCTION TO WRITE THE WHOLE SUBLATICE OF A PROCESSOR (FOR TESTING)
void OutputConf::writeSubLattice(int update){
    char fileName [50];
    sprintf(fileName, "conf/r%du%d", m_app->getParallel()->getRank(), update);
    FILE* output = fopen(fileName, "wb");
    for(int t = 0; t < m_app->getSize()[3]; t++){
    for(int z = 0; z < m_app->getSize()[2]; z++){
    for(int y = 0; y < m_app->getSize()[1]; y++){
    for(int x = 0; x < m_app->getSize()[0]; x++){
        fwrite(m_app->getLatticeSite(x,y,z,t).m_links, sizeof(double), 72, output);
    }}}}
    fclose(output);
}

// WRITE OUT A FILE FOR EACH TIME SLICE
void OutputConf::writeConfiguration(int confNum){
    MPI_File output;
    MPI_Offset startPointT, startPointZ, startPointY, startPointX, end;

    // Identify the block the processor belongs to
    int* subBlock = m_app->getParallel()->getRankSubBlock();
    int* fullSize = m_app->getParallel()->getFullSize();
    std::vector<int> subSize = m_app->getSize();

    // create filename and open
    char fileName [50];
    sprintf(fileName, "conf/%s%c/beta%.2lf_conf%04d.bin", m_app->getOutDir(), m_app->getStartType(), 6.0, confNum);
    m_app->getParallel()->openFile(output, fileName);

    //end = 72 * sizeof(double) * fullSize[0] * fullSize[1] * fullSize[2] * fullSize[3];
    //MPI_File_set_size(output, end);
    for(int t = 0; t < subSize[3]; t++){
        startPointT = ( fullSize[0]*fullSize[1]*fullSize[2] ) * ( subBlock[3]*subSize[3] + t) * 72 * sizeof(double);
    for(int z = 0; z < subSize[2]; z++){
        startPointZ = startPointT + ( fullSize[0]*fullSize[1] ) * ( subBlock[2]*subSize[2] + z) * 72 * sizeof(double);
    for(int y = 0; y < subSize[1]; y++){
        startPointY = startPointZ + ( fullSize[0] ) * ( subBlock[1]*subSize[1] + y) * 72 * sizeof(double);
    for(int x = 0; x < subSize[0]; x++){
        startPointX = startPointY + ( subBlock[0]*subSize[0] + x) * 72 * sizeof(double);
        MPI_File_write_at(output, startPointX, m_app->getLatticeSite(x,y,z,t).m_links, 72, MPI_DOUBLE, MPI_STATUS_IGNORE);
    }}}}
    m_app->getParallel()->closeFile(output);
}
