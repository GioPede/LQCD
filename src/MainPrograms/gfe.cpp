#include "Utils/clusterspecifier.h"
#include "Math/point.h"
#include "Math/lattice.h"
#include "InputOutput/inputconf.h"
#include <stdio.h>
#include <string>

int main(int argc, char* argv[]){
    if(argc < 2){
        printf("Specify Filename\n");
        exit(1);
    }

    int N, Nt;
    printf("Size of Lattice\n");
    scanf("%i %i", &N, &Nt);

    std::vector<int> size = {N, N, N, Nt};
    Lattice* lattice = new Lattice(size);
    InputConf* input = new InputConf(lattice);

    input->readLattice(std::string(argv[1]));
    while(1){
        int x, y, z, t;
        printf("Enter a location on the lattice:\n");
        scanf("%i %i %i %i", &x, &y, &z, &t);

        for(int mu = 0; mu < 4; mu++)
            (*lattice)(x,y,z,t)[mu].printSU3();
    }
}
