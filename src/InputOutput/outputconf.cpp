#include <cstdio>
#include <mpi/mpi.h>
#include "InputOutput/outputconf.h"
#include "Apps/app.h"
#include "Math/lattice.h"
#include "ParallelTools/parallel.h"
#include "Utils/clusterspecifier.h"

// CONSTRUCTOR
OutputConf::OutputConf(App *app){
    m_app = app;
    m_subBlock = Parallel::rankCoord();
    m_fullSize = Parallel::latticeFullSize();
    m_subSize =  Parallel::latticeSubSize();
    m_linkSize = 72 * sizeof(double);
}

// FUNCTION TO WRITE THE WHOLE SUBLATICE OF A PROCESSOR (FOR TESTING)
void OutputConf::writeSubLattice(int update){
    char fileName [50];
    sprintf(fileName, "conf/r%du%d", Parallel::rank(), update);
    FILE* output = fopen(fileName, "wb");
    for(int t = 0; t < Parallel::latticeSubSize()[3]; t++){
    for(int z = 0; z < Parallel::latticeSubSize()[2]; z++){
    for(int y = 0; y < Parallel::latticeSubSize()[1]; y++){
    for(int x = 0; x < Parallel::latticeSubSize()[0]; x++){
        fwrite(m_app->getLatticeSite(x,y,z,t).m_links, sizeof(double), 72, output);
    }}}}
    fclose(output);
}

// WRITE OUT A FILE FOR EACH TIME SLICE
void OutputConf::writeConfiguration(int confNum){
    MPI_File output;
    MPI_Offset startPointT, startPointZ, startPointY, startPointX;

    // create filename and open
    char fileName [128];
    sprintf(fileName, "%s/%s/conf%04d.bin", OUT_PREFIX, m_app->getOutDir(), confNum);
    Parallel::openFile(output, fileName);

    for(int t = 0; t < m_subSize[3]; t++){
        startPointT = ( m_fullSize[0]*m_fullSize[1]*m_fullSize[2] ) * ( m_subBlock[3]*m_subSize[3] + t);
    for(int z = 0; z < m_subSize[2]; z++){
        startPointZ = startPointT + ( m_fullSize[0]*m_fullSize[1] ) * ( m_subBlock[2]*m_subSize[2] + z);
    for(int y = 0; y < m_subSize[1]; y++){
        startPointY = startPointZ + ( m_fullSize[0] ) * ( m_subBlock[1]*m_subSize[1] + y);
    for(int x = 0; x < m_subSize[0]; x++){
        startPointX = startPointY + ( m_subBlock[0]*m_subSize[0] + x);
        startPointX *= m_linkSize;
        MPI_File_write_at(output, startPointX, m_app->getLatticeSite(x,y,z,t).m_links, 72, MPI_DOUBLE, MPI_STATUS_IGNORE);
    }}}}
    Parallel::closeFile(output);
}
