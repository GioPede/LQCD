import os
import readline
import autocompleter

def runPgfg():
    # Ask for input file
    readline.set_completer()
    inputFile = raw_input('Select Input File \t  > ')

    # Ask for queue manager
    queueCompleter = autocompleter.ListCompleter(['slurm', 'torque', 'bash', 'help'])
    readline.set_completer(queueCompleter.complete)
    queueManager = raw_input('Select Launcher Type \t  > ')

    if queueManager == 'slurm':
        launcher = open('launcher.sh', 'w+')
        launcher.write('#!/bin/bash\n')
        launcher.write('#SBATCH --partition=normal\n')
        launcher.write('#SBATCH --ntasks="$PROCS\n')
        launcher.write('#SBATCH --time="$TIME\n')
        launcher.write('#SBATCH --job-name="$NAME\n')

    readline.set_completer()
    email = raw_input('Select Notification Email > ')

    if email != '':
    	launcher.write('#SBATCH --mail-type=ALL\n')
    	launcher.write('#SBATCH --mail-user=' + email + '\n')

    # Write the main command line
    launcher.write('mpirun -np $PROCS build/pgfg $INPUT $NAME\n')
    launcher.close()

    a = os.system('bash launcher.sh')
    print a
