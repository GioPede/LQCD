#include <cstdio>
#include <cstdlib>
#include <mpi.h>
#include "inputconf.h"
#include "../GaugeFieldFactory/gaugefieldfactory.h"
#include "../GaugeFieldReader/gaugefieldreader.h"
#include "../Utils/lattice.h"
#include "../ParallelTools/parallel.h"

// CONSTRUCTOR
InputConf::InputConf(GaugeFieldFactory *gff){
    m_gff = gff;
}

InputConf::InputConf(GaugeFieldReader *gfr){
    m_gfr = gfr;
}

// FUNCTION TO WRITE THE WHOLE SUBLATICE OF A PROCESSOR (FOR TESTING)
void InputConf::readSubLattice(int update){
    char fileName [50];
    sprintf(fileName, "conf/r%du%d", m_gff->getParallel()->getRank(), update);
    FILE* input = fopen(fileName, "rb");
    for(int t = 0; t < m_gff->m_size[3]; t++){
    for(int z = 0; z < m_gff->m_size[2]; z++){
    for(int y = 0; y < m_gff->m_size[1]; y++){
    for(int x = 0; x < m_gff->m_size[0]; x++){
        fread(m_gff->getLatticeSite(x,y,z,t).m_links, sizeof(double), 72, input);
    }}}}
    fclose(input);
}

// WRITE OUT A FILE FOR EACH TIME SLICE
void InputConf::readConfiguration(int confNum){
    MPI_File input;
    MPI_Offset startPointT, startPointZ, startPointY, startPointX, end;

    // Identify the block the processor belongs to
    int* subBlock = m_gff->getParallel()->getRankSubBlock();
    int* fullSize = m_gff->getParallel()->getFullSize();
    std::vector<int> subSize = m_gff->m_size;

    // create filename and open
    char fileName [50];
    //sprintf(fileName, "conf/%s%c/beta%.2lf_conf%d.bin", m_gff->getOutDir(), m_gff->getStartType(), 6.0, confNum);
    sprintf(fileName, "conf/conf21.bin", m_gff->getOutDir(), m_gff->getStartType(), 6.0, confNum);
    MPI_File_open(m_gff->getParallel()->getComm(), fileName, MPI_MODE_CREATE|MPI_MODE_RDWR,
                  MPI_INFO_NULL, &input);

    //end = 72 * sizeof(double) * fullSize[0] * fullSize[1] * fullSize[2] * fullSize[3];
    //MPI_File_set_size(input, end);
    for(int t = 0; t < m_gff->m_size[3]; t++){
        startPointT = ( fullSize[0]*fullSize[1]*fullSize[2] ) * ( subBlock[3]*subSize[3] + t) * 72 * sizeof(double);
    for(int z = 0; z < m_gff->m_size[2]; z++){
        startPointZ = startPointT + ( fullSize[0]*fullSize[1] ) * ( subBlock[2]*subSize[2] + z) * 72 * sizeof(double);
    for(int y = 0; y < m_gff->m_size[1]; y++){
        startPointY = startPointZ + ( fullSize[0] ) * ( subBlock[1]*subSize[1] + y) * 72 * sizeof(double);
    for(int x = 0; x < m_gff->m_size[0]; x++){
        startPointX = startPointY + ( subBlock[0]*subSize[0] + x) * 72 * sizeof(double);
        MPI_File_read_at(input, startPointX, m_gff->getLatticeSite(x,y,z,t).m_links, 72, MPI_DOUBLE, MPI_STATUS_IGNORE);
    }}}}

    MPI_File_close(&input);
}

void InputConf::readConfiguration(std::string inputFile){
    MPI_File input;
    MPI_Offset startPointT, startPointZ, startPointY, startPointX, end;

    // Identify the block the processor belongs to
    int* subBlock = m_gfr->getParallel()->getRankSubBlock();
    int* fullSize = m_gfr->getParallel()->getFullSize();
    std::vector<int> subSize = m_gfr->m_size;

    // create filename and open
    MPI_File_open(m_gfr->getParallel()->getComm(), inputFile.c_str(), MPI_MODE_CREATE|MPI_MODE_RDWR,
                  MPI_INFO_NULL, &input);

    //end = 72 * sizeof(double) * fullSize[0] * fullSize[1] * fullSize[2] * fullSize[3];
    //MPI_File_set_size(input, end);
    for(int t = 0; t < m_gfr->m_size[3]; t++){
        startPointT = ( fullSize[0]*fullSize[1]*fullSize[2] ) * ( subBlock[3]*subSize[3] + t) * 72 * sizeof(double);
    for(int z = 0; z < m_gfr->m_size[2]; z++){
        startPointZ = startPointT + ( fullSize[0]*fullSize[1] ) * ( subBlock[2]*subSize[2] + z) * 72 * sizeof(double);
    for(int y = 0; y < m_gfr->m_size[1]; y++){
        startPointY = startPointZ + ( fullSize[0] ) * ( subBlock[1]*subSize[1] + y) * 72 * sizeof(double);
    for(int x = 0; x < m_gfr->m_size[0]; x++){
        startPointX = startPointY + ( subBlock[0]*subSize[0] + x) * 72 * sizeof(double);
        MPI_File_read_at(input, startPointX, m_gfr->getLatticeSite(x,y,z,t).m_links, 72, MPI_DOUBLE, MPI_STATUS_IGNORE);
    }}}}

    MPI_File_close(&input);
}
