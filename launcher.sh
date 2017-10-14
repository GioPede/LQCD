#!/bin/bash
#SBATCH --partition=normal
#SBATCH --ntasks=16
#SBATCH --time=24:00:00
#SBATCH --job-name=8_P16
mpirun -np 16 build/pgfg input/8_16.input 8_P16 
