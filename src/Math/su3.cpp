#include "Math/su3.h"
#include "Math/complex.h"
#include <ctime>
#include <cmath>
#include <cstdio>
#include <utility>
#include <random>

SU3 I, Q2, Q3;
SU3 F0(0.0), F1(0.0), F2(0.0);


SU3::SU3() noexcept{
}

SU3::SU3(double value) noexcept{
    for(auto& i : mat)
        i = value;
}

SU3::SU3(SU3 const& source) noexcept : mat(source.mat){
}

SU3::SU3(SU3&& source) noexcept : mat(source.mat){
}

// Hermitean Conjugate
SU3 operator~(const SU3& a){
    SU3 c;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            c.mat[2 * (3*i + j)] = a.mat[2 * (3*j + i)];
            c.mat[2 * (3*i + j) + 1] = -a.mat[2 * (3*j + i) + 1];
        }
    }
    return std::move(c);
}

SU3 operator~(SU3&& a){
    SU3 c;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            c.mat[2 * (3*i + j)] = a.mat[2 * (3*j + i)];
            c.mat[2 * (3*i + j) + 1] = -a.mat[2 * (3*j + i) + 1];
        }
    }
    return std::move(c);
}

void SU3::printSU3(){
    int idx = 0;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            printf("( %10.9f, %10.9f )\t", mat[idx], mat[idx+1]);
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

double SU3::imagTrace(){
    return mat[1] + mat[9] + mat[17];
}
double getMultSumTrace(SU3 &a, SU3 &b){
    return a.mat[0] * b.mat[0] - a.mat[1]*b.mat[1] + a.mat[2] * b.mat[6] - a.mat[3]*b.mat[7] + a.mat[4] * b.mat[12] - a.mat[5]*b.mat[13]
         + a.mat[6] * b.mat[2] - a.mat[7]*b.mat[3] + a.mat[8] * b.mat[8] - a.mat[9]*b.mat[9] + a.mat[10] * b.mat[14] - a.mat[11]*b.mat[15]
         + a.mat[12] * b.mat[4] - a.mat[13]*b.mat[5] +  a.mat[14] * b.mat[10] - a.mat[15]*b.mat[11] + a.mat[16] * b.mat[16] - a.mat[17]*b.mat[17];

}

double getMultSumTrace(SU3&& a, SU3 &b){
    return a.mat[0] * b.mat[0] - a.mat[1]*b.mat[1] + a.mat[2] * b.mat[6] - a.mat[3]*b.mat[7] + a.mat[4] * b.mat[12] - a.mat[5]*b.mat[13]
         + a.mat[6] * b.mat[2] - a.mat[7]*b.mat[3] + a.mat[8] * b.mat[8] - a.mat[9]*b.mat[9] + a.mat[10] * b.mat[14] - a.mat[11]*b.mat[15]
         + a.mat[12] * b.mat[4] - a.mat[13]*b.mat[5] +  a.mat[14] * b.mat[10] - a.mat[15]*b.mat[11] + a.mat[16] * b.mat[16] - a.mat[17]*b.mat[17];

}

SU3 exp(const SU3& Q){
    Q2 = Q*Q;
    Q3 = Q*Q*Q;

    double c0 = Q3.realTrace() / 3.0;
    double c1 = Q2.realTrace() / 2.0;

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
    double u2 = u*u;
    double w2 = w*w;

    if(fabs(w) < 0.05){
        xi = 1.0 - w2/6.0 * (1 - w2/20.0 * (1 - w2/42.0));
    }
    else
        xi = sin(w)/w;

    double c2u = cos(2*u);
    double s2u = sin(2*u);
    double cmu = cos(-u);
    double smu = sin(-u);
    double cw  = cos(w);
    complex h0;
    h0.real = (u*u-w2)*c2u + 8.0*u2*cw*cmu - 2.0*u*(3.0*u2+w2)*xi*smu;
    h0.imag = (u*u-w2)*s2u + 8.0*u2*cw*smu + 2.0*u*(3.0*u2+w2)*xi*cmu;

    complex h1;
    h1.real = 2*u*c2u - 2*u*cw*cmu - (3.0*u2-w2)*xi*smu;
    h1.imag = 2*u*s2u - 2*u*cw*smu + (3.0*u2-w2)*xi*cmu;

    complex h2;
    h2.real = c2u - cw*cmu + 3*u*xi*smu;
    h2.imag = s2u - cw*smu - 3*u*xi*cmu;

    if (c0negative){
        h0.imag = -h0.imag;
        h1.real = -h1.real;
        h2.imag = -h2.imag;
    }
    double norm = 1.0/(9.0*u2-w2);

    for(int i = 0; i < 18; i+=8){
        F0.mat[i] = h0.real*norm;
        F1.mat[i] = h1.real*norm;
        F2.mat[i] = h2.real*norm;

        F0.mat[i+1] = h0.imag*norm;
        F1.mat[i+1] = h1.imag*norm;
        F2.mat[i+1] = h2.imag*norm;
    }
    return std::move(F0 + F1*Q + F2*Q2);
}

SU3 exp(SU3&& Q){
    Q2 = Q*Q;
    Q3 = Q*Q2;

    double c0 = Q3.realTrace() / 3.0;
    double c1 = Q2.realTrace() / 2.0;
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
    double u2 = u*u;
    double w2 = w*w;

    if(fabs(w) < 0.05){
        xi = 1.0 - w2/6.0 * (1 - w2/20.0 * (1 - w2/42.0));

    }
    else
        xi = sin(w)/w;
    double c2u = cos(2*u);
    double s2u = sin(2*u);
    double cmu = cos(-u);
    double smu = sin(-u);
    double cw  = cos(w);
    complex h0;
    h0.real = (u*u-w2)*c2u + 8.0*u2*cw*cmu - 2.0*u*(3.0*u2+w2)*xi*smu;
    h0.imag = (u*u-w2)*s2u + 8.0*u2*cw*smu + 2.0*u*(3.0*u2+w2)*xi*cmu;

    complex h1;
    h1.real = 2*u*c2u - 2*u*cw*cmu - (3.0*u2-w2)*xi*smu;
    h1.imag = 2*u*s2u - 2*u*cw*smu + (3.0*u2-w2)*xi*cmu;

    complex h2;
    h2.real = c2u - cw*cmu + 3*u*xi*smu;
    h2.imag = s2u - cw*smu - 3*u*xi*cmu;

    if (c0negative){
        h0.imag = -h0.imag;
        h1.real = -h1.real;
        h2.imag = -h2.imag;
    }
    double norm = 1.0/(9.0*u2-w2);



    for(int i = 0; i < 18; i+=8){
        F0.mat[i] = h0.real*norm;
        F1.mat[i] = h1.real*norm;
        F2.mat[i] = h2.real*norm;

        F0.mat[i+1] = h0.imag*norm;
        F1.mat[i+1] = h1.imag*norm;
        F2.mat[i+1] = h2.imag*norm;
    }
    return std::move(F0 + F1*Q + F2*Q2);
}
