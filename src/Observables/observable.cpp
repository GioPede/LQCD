#include "Utils/clusterspecifier.h"
#ifndef LACONIA
#include <mpi/mpi.h>
#else
#include <mpi.h>
#endif
#include "Observables/observable.h"
#include "Math/lattice.h"
#include "ParallelTools/parallel.h"

void Observable::gatherResults(){
    MPI_Allreduce(&m_value, &m_value, 1, MPI_DOUBLE, MPI_SUM, Parallel::cartCoordComm());
}

double Observable::value(){
    return m_value;
}
