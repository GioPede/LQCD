#include <cstdio>
#include <chrono>
#include "lqcd.h"

/* ****************************************************************************
 *
 * TODO LIST:
 *  Priority MAXIMUM:
 *
 *  Priority NORMAL:
 *      - move parallel stuff of Output classe
 *      - make output for plaquette
 *      - clean and comment
 *
 *  Priority LOW:
 *      - add a dynamic observable input list (now only one Plaquette)
 *
 * ***************************************************************************/


/*
#include<random>

extern std::mt19937_64 randomGen;
extern std::uniform_real_distribution<double> randomUniformInterval;

int main(int argn, char* argv[]) {

    Parallel* parallelGeom = new Parallel(argn, argv);

    if(parallelGeom->getRank() == 0){
//        SU3 g[8];
//        for(int i = 0; i < 8; i++)
//            g[i].setSU3Zero();

//        g[0].mat[3] = -0.5;
//        g[0].mat[7] = -0.5;

//        g[1].mat[2] = -0.5;
//        g[1].mat[6] =  0.5;

//        g[2].mat[1] = -0.5;
//        g[2].mat[9] = 0.5;

//        g[3].mat[5] = -0.5;
//        g[3].mat[13] = -0.5;

//        g[4].mat[4] = -0.5;
//        g[4].mat[12] = 0.5;

//        g[5].mat[11] = -0.5;
//        g[5].mat[15] = -0.5;

//        g[6].mat[10] = -0.5;
//        g[6].mat[14] = 0.5;

//        g[7].mat[1] = -0.5*sqrt(3);
//        g[7].mat[9] = -0.5*sqrt(3);
//        g[7].mat[17] = 1.0*sqrt(3);

//        SU3 Q;
//        Q.setSU3Zero();

//        for(int i = 0; i < 8; i++){
//            Q += g[i]*randomUniformInterval(randomGen);
//        }
//        Q.printSU3();
//        SU3 Q2 = Q*Q;

//        Q2.printSU3();
//        (Q2*Q).printSU3();
//        (Q*Q2).printSU3();


        SU3 Q, Q2, Q3;
        Q.setSU3Random();
        double tr = (Q-~Q).imagTrace()/6.0;
        Q = (Q-~Q) * 0.5;
        for(int i = 1; i < 18; i+=2)
            Q.mat[i] -= tr;
        for(int i = 0; i < 18; i+=2){
            double temp = Q.mat[i];
            Q.mat[i] = Q.mat[i+1];
            Q.mat[i+1] = -temp;
        }
        Q2 = Q*Q;
        Q3 = Q*Q*Q;

        printf("Q=\n");
        Q.printSU3();
        printf("Q2=\n");
        Q2.printSU3();
        printf("Q3=\n");
        Q3.printSU3();

        double c0 = Q3.realTrace() / 3.0;
        double c1 = Q2.realTrace() / 2.0;

        SU3 I;
        I.setSU3Identity();
        (Q3-Q*c1-I*c0).printSU3();

        bool c0negative = false;
        if(c0 < 0){
            c0negative = true;
            c0 = -c0;
        }
        double c0max = pow(c1/3.0,1.5)*2.0;
        double theta = acos(c0/c0max);
        double w = sqrt(c1)*sin(theta/3.0);
        double u = sqrt(c1/3.0)*cos(theta/3.0);
        double xi;

        if(fabs(w) < 0.05){
            xi = 1.0 - w*w/6.0 * (1 - w*w/20.0 * (1 - w*w/42.0));
            printf("small w\n");
        }
        else
            xi = sin(w)/w;

        complex h0;
        h0.real = (u*u-w*w)*cos(2*u);
        h0.imag = (u*u-w*w)*sin(2*u);
        h0.real += 8.0*u*u*cos(w)*cos(-u);
        h0.imag += 8.0*u*u*cos(w)*sin(-u);
        h0.real -= 2.0*u*(3.0*u*u+w*w)*xi*sin(-u);
        h0.imag += 2.0*u*(3.0*u*u+w*w)*xi*cos(-u);

        complex h1;
        h1.real = 2*u*cos(2*u);
        h1.imag = 2*u*sin(2*u);
        h1.real -= 2*u*cos(w)*cos(-u);
        h1.imag -= 2*u*cos(w)*sin(-u);
        h1.real -= (3.0*u*u-w*w)*xi*sin(-u);
        h1.imag += (3.0*u*u-w*w)*xi*cos(-u);

        complex h2;
        h2.real = cos(2*u);
        h2.imag = sin(2*u);
        h2.real -= cos(w)*cos(-u);
        h2.imag -= cos(w)*sin(-u);
        h2.real += 3*u*xi*sin(-u);
        h2.imag -= 3*u*xi*cos(-u);

        if (c0negative){
            h0.real = -h0.real;
            h1.imag = -h1.imag;
            h2.real = -h2.real;
        }
        printf("h0, h1, h2=\n");
        h0.printComplex();
        h1.printComplex();
        h2.printComplex();
        SU3 F0, F1, F2;
        F0.setSU3Zero();
        F1.setSU3Zero();
        F2.setSU3Zero();
        double norm = 1.0/(9.0*u*u-w*w);

        for(int i = 0; i < 18; i+=8){
            F0.mat[i] = h0.real*norm;
            F1.mat[i] = h1.real*norm;
            F2.mat[i] = h2.real*norm;

            F0.mat[i+1] = h0.imag*norm;
            F1.mat[i+1] = h1.imag*norm;
            F2.mat[i+1] = h2.imag*norm;
        }
        SU3 exp = F0*I + F1*Q + F2*Q2;

        printf("A=exp(iQ)=\n");
        exp.printSU3();

        printf("A*Adagger=\n");
        (exp*~exp).printSU3();

        SU3 B,C,D,E;
        B.setSU3Random();
        C.setSU3Random();
        D.setSU3Random();
        E.setSU3Random();

        SU3 G = (B*C*D*E);
        G.printSU3();
        SU3 H = (~E*~D*~C*~B);
        H.printSU3();
        (G*H).printSU3();
        printf("%lf    %lf\n",(B*C).realTrace(), (C*B).realTrace() );

    }



    parallelGeom->finalize();
    delete parallelGeom;
    return 0;
}
*/

int main(int argn, char* argv[]) {

    // Start timer
    auto start = std::chrono::system_clock::now();

    // Start MPI
    Parallel* parallelGeom = new Parallel(argn, argv);

    // Check argument count, read and parse the input file
    InputParser* input;
    input = new InputParser(argn, argv);

    // Initialize MPI and organize sublattices
    parallelGeom->createNeighborLists(input->latticeSize, input->subLatticeSize);

    // Send Parameters to the Factory and init the system
    GaugeFieldFactory GFF(input, parallelGeom);

    // Start Generating Gauges
    GFF.generateConfigurations();

    // Finalize MPI and delete objects
    parallelGeom->finalize();

    // Stop timer and output
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedTime = end-start;
    if(parallelGeom->getRank() == 0)
        printf("Total time taken: %.2fs\n", elapsedTime.count());

    // End
    delete input;
    delete parallelGeom;
    return 0;
}
