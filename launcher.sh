#!/bin/bash
#SBATCH --partition=normal
#SBATCH --ntasks="$PROCS
#SBATCH --time="$TIME
#SBATCH --job-name="$NAME
#SBATCH --mail-type=ALL
#SBATCH --mail-user=aklsjgyr
mpirun -np $PROCS build/pgfg $INPUT $NAME
