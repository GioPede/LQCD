#include "Math/su3.h"
#include "Math/random.h"
#include "Math/complex.h"
#include <random>

std::random_device Random::rd;
std::mt19937 Random::randomGen(Random::rd());
std::uniform_real_distribution<double> Random::randomUniformInterval(0.0, 1.0);

double Random::randUniform(){
    return randomUniformInterval(randomGen);
}


SU3 Random::randSU3(){
    SU3 result;
    complex v1[3], v2[3];
    complex u1[3], u2[3], u3[3];

    // Create 2 random vectors
    for(int i = 0; i < 3; i++){
        v1[i].real = randUniform();
        v1[i].imag = randUniform();
        v2[i].real = randUniform();
        v2[i].imag = randUniform();
    }

    // Normalize vector v1 into u1
    double w1 = 0;
    for(int i = 0; i < 3; i++)
        w1 += v1[i].real*v1[i].real + v1[i].imag*v1[i].imag;
    w1 = sqrt(w1);
    for(int i = 0; i < 3; i++)
        u1[i] = v1[i]/w1;

    // Orthogonalize vector v2 to vector u1
    complex v2u1;
    for(int i = 0; i < 3; i++){
        v2u1 += v2[i]*(~u1[i]);
    }
    for(int i = 0; i < 3; i++){
        v2[i] = v2[i] - v2u1*u1[i];
    }

    // Normalize vector v2 into u2
    double w2 = 0;
    for(int i = 0; i < 3; i++)
        w2 += v2[i].real*v2[i].real + v2[i].imag*v2[i].imag;
    w2 = sqrt(w2);
    for(int i = 0; i < 3; i++)
        u2[i] = v2[i]/w2;

    // Create vector u3 as u1xu2
    u3[0] = ~(u1[1]*u2[2] - u1[2]*u2[1]);
    u3[1] = ~(u1[2]*u2[0] - u1[0]*u2[2]);
    u3[2] = ~(u1[0]*u2[1] - u1[1]*u2[0]);

    for(int i = 0; i < 3; i++){
        result.mat[2*i] = u1[i].real;
        result.mat[2*i+1] = u1[i].imag;
        result.mat[6+2*i] = u2[i].real;
        result.mat[6+2*i+1] = u2[i].imag;
        result.mat[12+2*i] = u3[i].real;
        result.mat[12+2*i+1] = u3[i].imag;
    }
    return std::move(result);
}


SU3 Random::randSU3Transf(double epsilon){
    SU3 R, S, T;
    double x[4];
    double norm;
    // Create 3 SU2 matrices and map them on SU3 elements
    R.setSU3Identity();
    S.setSU3Identity();
    T.setSU3Identity();

    norm = 0;
    x[0] = randUniform() - 0.5;
    for(int i = 1; i < 4; i++){
        x[i] = randUniform() - 0.5;
        norm += x[i]*x[i];
    }
    norm = sqrt(norm);

    if(x[0] < 0)
        x[0] = -sqrt(1-epsilon*epsilon);
    else
        x[0] = sqrt(1-epsilon*epsilon);
    for(int i = 1; i < 4; i++)
        x[i] = x[i] * epsilon / norm;
    R.mat[0] = x[0];
    R.mat[8] = x[0];
    R.mat[1] = x[3];
    R.mat[9] = -x[3];

    R.mat[2] = x[2];
    R.mat[6] = -x[2];
    R.mat[3] = x[1];
    R.mat[7] = x[1];


    norm = 0;
    x[0] = randUniform() - 0.5;
    for(int i = 1; i < 4; i++){
        x[i] = randUniform() - 0.5;
        norm += x[i]*x[i];
    }
    norm = sqrt(norm);

    if(x[0] < 0)
        x[0] = -sqrt(1-epsilon*epsilon);
    else
        x[0] = sqrt(1-epsilon*epsilon);
    for(int i = 1; i < 4; i++)
        x[i] = x[i] * epsilon / norm;
    S.mat[0] = x[0];
    S.mat[16] = x[0];
    S.mat[1] = x[3];
    S.mat[17] = -x[3];

    S.mat[4] = x[2];
    S.mat[12] = -x[2];
    S.mat[5] = x[1];
    S.mat[13] = x[1];



    norm = 0;
    x[0] = randUniform() - 0.5;
    for(int i = 1; i < 4; i++){
        x[i] = randUniform() - 0.5;
        norm += x[i]*x[i];
    }
    norm = sqrt(norm);

    if(x[0] < 0)
        x[0] = -sqrt(1-epsilon*epsilon);
    else
        x[0] = sqrt(1-epsilon*epsilon);
    for(int i = 1; i < 4; i++)
        x[i] = x[i] * epsilon / norm;
    T.mat[8] = x[0];
    T.mat[16] = x[0];
    T.mat[9] = x[3];
    T.mat[17] = -x[3];

    T.mat[10] = x[2];
    T.mat[14] = -x[2];
    T.mat[11] = x[1];
    T.mat[15] = x[1];

    return std::move(R*S*T);
}
