#include "Math/su3.h"
#include "Math/complex.h"
#include "Math/random.h"
#include <ctime>
#include <cmath>
#include <stdio.h>

/*******************************************************
* Functions between SU3 elements, defined as arrays.
********************************************************/
std::random_device rd;
std::mt19937_64 randomGen(rd());
std::uniform_real_distribution<double> randomUniformInterval(-1.0, 1.0);


SU3 R, S, T;
double x[4];
double norm;

//////  BINARY OPERATIONS  //////

// Sum between SU3
SU3 operator+(SU3 a, SU3 b){
    SU3 c;
    for(int i = 0; i < 18; i++)
        c.mat[i] = a.mat[i] + b.mat[i];
    return c;
}

void operator+=(SU3&a, SU3 b){
    for(int i = 0; i < 18; i++)
        a.mat[i] += b.mat[i];
}

// Subtraction between SU3
SU3 operator-(SU3 a, SU3 b){
    SU3 c;
    for(int i = 0; i < 18; i++)
        c.mat[i] = a.mat[i] - b.mat[i];
    return c;
}

void operator-=(SU3&a, SU3 b){
    for(int i = 0; i < 18; i++)
        a.mat[i] -= b.mat[i];
}

// Multiplication between SU3
SU3 operator*(SU3 a, SU3 b){
    SU3 c;
    //REAL
    c.mat[0] =  a.mat[0] * b.mat[0] - a.mat[1]*b.mat[1] + a.mat[2] * b.mat[6] - a.mat[3]*b.mat[7] + a.mat[4] * b.mat[12] - a.mat[5]*b.mat[13];
    c.mat[2] =  a.mat[0] * b.mat[2] - a.mat[1]*b.mat[3] + a.mat[2] * b.mat[8] - a.mat[3]*b.mat[9] + a.mat[4] * b.mat[14] - a.mat[5]*b.mat[15];
    c.mat[4] =  a.mat[0] * b.mat[4] - a.mat[1]*b.mat[5] + a.mat[2] * b.mat[10] - a.mat[3]*b.mat[11] + a.mat[4] * b.mat[16] - a.mat[5]*b.mat[17];
    c.mat[6] =  a.mat[6] * b.mat[0] - a.mat[7]*b.mat[1] + a.mat[8] * b.mat[6] - a.mat[9]*b.mat[7] + a.mat[10] * b.mat[12] - a.mat[11]*b.mat[13];
    c.mat[8] =  a.mat[6] * b.mat[2] - a.mat[7]*b.mat[3] + a.mat[8] * b.mat[8] - a.mat[9]*b.mat[9] + a.mat[10] * b.mat[14] - a.mat[11]*b.mat[15];
    c.mat[10] =  a.mat[6] * b.mat[4] - a.mat[7]*b.mat[5] + a.mat[8] * b.mat[10] - a.mat[9]*b.mat[11] + a.mat[10] * b.mat[16] - a.mat[11]*b.mat[17];
    c.mat[12] =  a.mat[12] * b.mat[0] - a.mat[13]*b.mat[1] + a.mat[14] * b.mat[6] - a.mat[15]*b.mat[7] + a.mat[16] * b.mat[12] - a.mat[17]*b.mat[13];
    c.mat[14] =  a.mat[12] * b.mat[2] - a.mat[13]*b.mat[3] + a.mat[14] * b.mat[8] - a.mat[15]*b.mat[9] + a.mat[16] * b.mat[14] - a.mat[17]*b.mat[15];
    c.mat[16] =  a.mat[12] * b.mat[4] - a.mat[13]*b.mat[5] +  a.mat[14] * b.mat[10] - a.mat[15]*b.mat[11] + a.mat[16] * b.mat[16] - a.mat[17]*b.mat[17];


    // COMPLEX
    c.mat[1] =  a.mat[0] * b.mat[1] + a.mat[1]*b.mat[0] + a.mat[2] * b.mat[7] + a.mat[3]*b.mat[6] + a.mat[4] * b.mat[13] + a.mat[5]*b.mat[12];
    c.mat[3] =  a.mat[0] * b.mat[3] + a.mat[1]*b.mat[2] + a.mat[2] * b.mat[9] + a.mat[3]*b.mat[8] + a.mat[4] * b.mat[15] + a.mat[5]*b.mat[14];
    c.mat[5] =  a.mat[0] * b.mat[5] + a.mat[1]*b.mat[4] + a.mat[2] * b.mat[11] + a.mat[3]*b.mat[10] + a.mat[4] * b.mat[17] + a.mat[5]*b.mat[16];
    c.mat[7] =  a.mat[6] * b.mat[1] + a.mat[7]*b.mat[0] + a.mat[8] * b.mat[7] + a.mat[9]*b.mat[6] + a.mat[10] * b.mat[13] + a.mat[11]*b.mat[12];
    c.mat[9] =  a.mat[6] * b.mat[3] + a.mat[7]*b.mat[2] + a.mat[8] * b.mat[9] + a.mat[9]*b.mat[8] + a.mat[10] * b.mat[15] + a.mat[11]*b.mat[14];
    c.mat[11] =  a.mat[6] * b.mat[5] + a.mat[7]*b.mat[4] + a.mat[8] * b.mat[11] + a.mat[9]*b.mat[10] + a.mat[10] * b.mat[17] + a.mat[11]*b.mat[16];
    c.mat[13] =  a.mat[12] * b.mat[1] + a.mat[13]*b.mat[0] + a.mat[14] * b.mat[7] + a.mat[15]*b.mat[6] + a.mat[16] * b.mat[13] + a.mat[17]*b.mat[12];
    c.mat[15] =  a.mat[12] * b.mat[3] + a.mat[13]*b.mat[2] + a.mat[14] * b.mat[9] + a.mat[15]*b.mat[8] + a.mat[16] * b.mat[15] + a.mat[17]*b.mat[14];
    c.mat[17] =  a.mat[12] * b.mat[5] + a.mat[13]*b.mat[4] + a.mat[14] * b.mat[11] + a.mat[15]*b.mat[10] + a.mat[16] * b.mat[17] + a.mat[17]*b.mat[16];


    /*
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            c.mat[2 * (3*i + j)] = 0;
            c.mat[2 * (3*i + j) + 1] = 0;
            for(int k = 0; k < 3; k++){
                c.mat[2 * (3*i + j)] += a.mat[2 * (3*i + k)] * b.mat[2 * (3*k + j)]
                                       -a.mat[2 * (3*i + k) + 1] * b.mat[2 * (3*k + j) + 1];
                c.mat[2 * (3*i + j) + 1] += a.mat[2 * (3*i + k)] * b.mat[2 * (3*k + j) + 1]
                                           +a.mat[2 * (3*i + k) + 1] * b.mat[2 * (3*k + j)];

            }
        }
    }*/
    return c;
}

void operator*=(SU3&a, SU3 b){
    SU3 c;
    //REAL
    c.mat[0] =  a.mat[0] * b.mat[0] - a.mat[1]*b.mat[1] + a.mat[2] * b.mat[6] - a.mat[3]*b.mat[7] + a.mat[4] * b.mat[12] - a.mat[5]*b.mat[13];
    c.mat[2] =  a.mat[0] * b.mat[2] - a.mat[1]*b.mat[3] + a.mat[2] * b.mat[8] - a.mat[3]*b.mat[9] + a.mat[4] * b.mat[14] - a.mat[5]*b.mat[15];
    c.mat[4] =  a.mat[0] * b.mat[4] - a.mat[1]*b.mat[5] + a.mat[2] * b.mat[10] - a.mat[3]*b.mat[11] + a.mat[4] * b.mat[16] - a.mat[5]*b.mat[17];
    c.mat[6] =  a.mat[6] * b.mat[0] - a.mat[7]*b.mat[1] + a.mat[8] * b.mat[6] - a.mat[9]*b.mat[7] + a.mat[10] * b.mat[12] - a.mat[11]*b.mat[13];
    c.mat[8] =  a.mat[6] * b.mat[2] - a.mat[7]*b.mat[3] + a.mat[8] * b.mat[8] - a.mat[9]*b.mat[9] + a.mat[10] * b.mat[14] - a.mat[11]*b.mat[15];
    c.mat[10] =  a.mat[6] * b.mat[4] - a.mat[7]*b.mat[5] + a.mat[8] * b.mat[10] - a.mat[9]*b.mat[11] + a.mat[10] * b.mat[16] - a.mat[11]*b.mat[17];
    c.mat[12] =  a.mat[12] * b.mat[0] - a.mat[13]*b.mat[1] + a.mat[14] * b.mat[6] - a.mat[15]*b.mat[7] + a.mat[16] * b.mat[12] - a.mat[17]*b.mat[13];
    c.mat[14] =  a.mat[12] * b.mat[2] - a.mat[13]*b.mat[3] + a.mat[14] * b.mat[8] - a.mat[15]*b.mat[9] + a.mat[16] * b.mat[14] - a.mat[17]*b.mat[15];
    c.mat[16] =  a.mat[12] * b.mat[4] - a.mat[13]*b.mat[5] +  a.mat[14] * b.mat[10] - a.mat[15]*b.mat[11] + a.mat[16] * b.mat[16] - a.mat[17]*b.mat[17];


    // COMPLEX
    c.mat[1] =  a.mat[0] * b.mat[1] + a.mat[1]*b.mat[0] + a.mat[2] * b.mat[7] + a.mat[3]*b.mat[6] + a.mat[4] * b.mat[13] + a.mat[5]*b.mat[12];
    c.mat[3] =  a.mat[0] * b.mat[3] + a.mat[1]*b.mat[2] + a.mat[2] * b.mat[9] + a.mat[3]*b.mat[8] + a.mat[4] * b.mat[15] + a.mat[5]*b.mat[14];
    c.mat[5] =  a.mat[0] * b.mat[5] + a.mat[1]*b.mat[4] + a.mat[2] * b.mat[11] + a.mat[3]*b.mat[10] + a.mat[4] * b.mat[17] + a.mat[5]*b.mat[16];
    c.mat[7] =  a.mat[6] * b.mat[1] + a.mat[7]*b.mat[0] + a.mat[8] * b.mat[7] + a.mat[9]*b.mat[6] + a.mat[10] * b.mat[13] + a.mat[11]*b.mat[12];
    c.mat[9] =  a.mat[6] * b.mat[3] + a.mat[7]*b.mat[2] + a.mat[8] * b.mat[9] + a.mat[9]*b.mat[8] + a.mat[10] * b.mat[15] + a.mat[11]*b.mat[14];
    c.mat[11] =  a.mat[6] * b.mat[5] + a.mat[7]*b.mat[4] + a.mat[8] * b.mat[11] + a.mat[9]*b.mat[10] + a.mat[10] * b.mat[17] + a.mat[11]*b.mat[16];
    c.mat[13] =  a.mat[12] * b.mat[1] + a.mat[13]*b.mat[0] + a.mat[14] * b.mat[7] + a.mat[15]*b.mat[6] + a.mat[16] * b.mat[13] + a.mat[17]*b.mat[12];
    c.mat[15] =  a.mat[12] * b.mat[3] + a.mat[13]*b.mat[2] + a.mat[14] * b.mat[9] + a.mat[15]*b.mat[8] + a.mat[16] * b.mat[15] + a.mat[17]*b.mat[14];
    c.mat[17] =  a.mat[12] * b.mat[5] + a.mat[13]*b.mat[4] + a.mat[14] * b.mat[11] + a.mat[15]*b.mat[10] + a.mat[16] * b.mat[17] + a.mat[17]*b.mat[16];

    /*
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            c.mat[2 * (3*i + j)] = 0;
            c.mat[2 * (3*i + j) + 1] = 0;
            for(int k = 0; k < 3; k++){
                c.mat[2 * (3*i + j)] += a.mat[2 * (3*i + k)] * b.mat[2 * (3*k + j)]
                                       -a.mat[2 * (3*i + k) + 1] * b.mat[2 * (3*k + j) + 1];
                c.mat[2 * (3*i + j) + 1] += a.mat[2 * (3*i + k)] * b.mat[2 * (3*k + j) + 1]
                                           +a.mat[2 * (3*i + k) + 1] * b.mat[2 * (3*k + j)];

            }
        }
    }*/
    a = c;
}

SU3 operator*(SU3 a, double b){
    SU3 c;
    for(int i = 0; i < 18; i++)
        c.mat[i] = b * a.mat[i];
    return c;
}

// Hermitean Conjugate
SU3 operator~(SU3 a){
    SU3 c;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            c.mat[2 * (3*i + j)] = a.mat[2 * (3*j + i)];
            c.mat[2 * (3*i + j) + 1] = -a.mat[2 * (3*j + i) + 1];
        }
    }
    return c;
}

void SU3::printSU3(){
    int idx = 0;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            printf("( %7.3f, %7.3f )\t", mat[idx], mat[idx+1]);
            idx += 2;
        }
        printf("\n");
    }
    printf("\n");
}

void SU3::setSU3Identity(){
    for(int i = 0; i < 18; i++)
        mat[i] = 0;
    mat[0] = 1;
    mat[8] = 1;
    mat[16] = 1;
}

void SU3::setSU3Zero(){
    for(int i = 0; i < 18; i++)
        mat[i] = 0;
}

double SU3::realTrace(){
    return mat[0] + mat[8] + mat[16];
}

void SU3::setSU3Random(){
    complex v1[3], v2[3];
    complex u1[3], u2[3], u3[3];

    // Create 2 random vectors
    for(int i = 0; i < 3; i++){
        v1[i].real = randomUniformInterval(randomGen);
        v1[i].imag = randomUniformInterval(randomGen);
        v2[i].real = randomUniformInterval(randomGen);
        v2[i].imag = randomUniformInterval(randomGen);
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
        mat[2*i] = u1[i].real;
        mat[2*i+1] = u1[i].imag;
        mat[6+2*i] = u2[i].real;
        mat[6+2*i+1] = u2[i].imag;
        mat[12+2*i] = u3[i].real;
        mat[12+2*i+1] = u3[i].imag;
    }
}



SU3 getRandomTransformation(double epsilon){
    // Create 3 SU2 matrices and map them on SU3 elements
    R.setSU3Identity();
    S.setSU3Identity();
    T.setSU3Identity();

    norm = 0;
    x[0] = randomUniformInterval(randomGen) * 0.5;
    for(int i = 1; i < 4; i++){
        x[i] = randomUniformInterval(randomGen) * 0.5;
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
    x[0] = randomUniformInterval(randomGen) * 0.5;
    for(int i = 1; i < 4; i++){
        x[i] = randomUniformInterval(randomGen) * 0.5;
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
    x[0] = randomUniformInterval(randomGen) * 0.5;
    for(int i = 1; i < 4; i++){
        x[i] = randomUniformInterval(randomGen) * 0.5;
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

    return R*S*T;
}

/*
SU3 getRandomTransformation(double epsilon){
    SU3 U;
    complex v1[3], v2[3];
    complex u1[3], u2[3], u3[3];

    v1[0].real = 1;
    v1[0].imag = randomUniformInterval(randomGen) * epsilon;
    v1[1].real = randomUniformInterval(randomGen) * epsilon;
    v1[1].imag = randomUniformInterval(randomGen) * epsilon;
    v1[2].real = randomUniformInterval(randomGen) * epsilon;
    v1[2].imag = randomUniformInterval(randomGen) * epsilon;

    v2[0].real = -v1[1].real;
    v2[0].imag = v1[1].imag;
    v2[1].real = 1;
    v2[1].imag = randomUniformInterval(randomGen) * epsilon;
    v2[2].real = randomUniformInterval(randomGen) * epsilon;
    v2[2].imag = randomUniformInterval(randomGen) * epsilon;

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
        U.mat[2*i] = u1[i].real;
        U.mat[2*i+1] = u1[i].imag;
        U.mat[6+2*i] = u2[i].real;
        U.mat[6+2*i+1] = u2[i].imag;
        U.mat[12+2*i] = u3[i].real;
        U.mat[12+2*i+1] = u3[i].imag;
    }

    return U;
}*/

double getMultSumTrace(SU3 &a, SU3 &b){
    return a.mat[0] * b.mat[0] - a.mat[1]*b.mat[1] + a.mat[2] * b.mat[6] - a.mat[3]*b.mat[7] + a.mat[4] * b.mat[12] - a.mat[5]*b.mat[13]
         + a.mat[6] * b.mat[2] - a.mat[7]*b.mat[3] + a.mat[8] * b.mat[8] - a.mat[9]*b.mat[9] + a.mat[10] * b.mat[14] - a.mat[11]*b.mat[15]
         + a.mat[12] * b.mat[4] - a.mat[13]*b.mat[5] +  a.mat[14] * b.mat[10] - a.mat[15]*b.mat[11] + a.mat[16] * b.mat[16] - a.mat[17]*b.mat[17];

}

SU3 SU3::exp(){

}
