#include "Math/su3.h"
#include "Math/complex.h"
#include "Math/random.h"
#include <ctime>
#include <cmath>
#include <cstdio>
#include <utility>

/*******************************************************
* Functions between SU3 elements, defined as arrays.
********************************************************/
std::random_device rd;
std::mt19937_64 randomGen(rd());
std::uniform_real_distribution<double> randomUniformInterval(-1.0, 1.0);


SU3 R, S, T;
SU3 I, Q2, Q3;
SU3 F0(0.0), F1(0.0), F2(0.0);
double x[4];
double norm;

SU3::SU3() noexcept{
}

SU3::SU3(double value) noexcept{
    for(auto& i : mat)
        i = value;
}

SU3::SU3(SU3 const & source) noexcept : mat(source.mat){
}

SU3::SU3(SU3&& source) noexcept{
    *this = std::move(source);
}

// assignment
SU3& SU3::operator=(SU3&& other) noexcept{
    std::swap(mat, other.mat);
    return *this;
}

SU3& SU3::operator=(const SU3& other) noexcept{
    mat = other.mat;
    return *this;
}

// sum
SU3& SU3::operator+=(SU3&& other) noexcept{
    for(int i = 0; i < 18; i++)
        mat[i] += other.mat[i];
    return *this;
}

SU3& SU3::operator+=(const SU3& other) noexcept{
    for(int i = 0; i < 18; i++)
        mat[i] += other.mat[i];
    return *this;
}

// subtraction
SU3& SU3::operator-=(SU3&& other) noexcept{
    for(int i = 0; i < 18; i++)
        mat[i] -= other.mat[i];
    return *this;
}

SU3& SU3::operator-=(const SU3& other) noexcept{
    for(int i = 0; i < 18; i++)
        mat[i] -= other.mat[i];
    return *this;
}

// multiplication
SU3& SU3::operator*=(const SU3& other) noexcept{
    SU3 result;
    /*
    //REAL
    result.mat[0] =  mat[0] * other.mat[0] - mat[1]*other.mat[1] + mat[2] * other.mat[6] - mat[3]*other.mat[7] + mat[4] * other.mat[12] - mat[5]*other.mat[13];
    result.mat[2] =  mat[0] * other.mat[2] - mat[1]*other.mat[3] + mat[2] * other.mat[8] - mat[3]*other.mat[9] + mat[4] * other.mat[14] - mat[5]*other.mat[15];
    result.mat[4] =  mat[0] * other.mat[4] - mat[1]*other.mat[5] + mat[2] * other.mat[10] - mat[3]*other.mat[11] + mat[4] * other.mat[16] - mat[5]*other.mat[17];
    result.mat[6] =  mat[6] * other.mat[0] - mat[7]*other.mat[1] + mat[8] * other.mat[6] - mat[9]*other.mat[7] + mat[10] * other.mat[12] - mat[11]*other.mat[13];
    result.mat[8] =  mat[6] * other.mat[2] - mat[7]*other.mat[3] + mat[8] * other.mat[8] - mat[9]*other.mat[9] + mat[10] * other.mat[14] - mat[11]*other.mat[15];
    result.mat[10] =  mat[6] * other.mat[4] - mat[7]*other.mat[5] + mat[8] * other.mat[10] - mat[9]*other.mat[11] + mat[10] * other.mat[16] - mat[11]*other.mat[17];
    result.mat[12] =  mat[12] * other.mat[0] - mat[13]*other.mat[1] + mat[14] * other.mat[6] - mat[15]*other.mat[7] + mat[16] * other.mat[12] - mat[17]*other.mat[13];
    result.mat[14] =  mat[12] * other.mat[2] - mat[13]*other.mat[3] + mat[14] * other.mat[8] - mat[15]*other.mat[9] + mat[16] * other.mat[14] - mat[17]*other.mat[15];
    result.mat[16] =  mat[12] * other.mat[4] - mat[13]*other.mat[5] +  mat[14] * other.mat[10] - mat[15]*other.mat[11] + mat[16] * other.mat[16] - mat[17]*other.mat[17];


    // COMPLEX
    result.mat[1] =  mat[0] * other.mat[1] + mat[1]*other.mat[0] + mat[2] * other.mat[7] + mat[3]*other.mat[6] + mat[4] * other.mat[13] + mat[5]*other.mat[12];
    result.mat[3] =  mat[0] * other.mat[3] + mat[1]*other.mat[2] + mat[2] * other.mat[9] + mat[3]*other.mat[8] + mat[4] * other.mat[15] + mat[5]*other.mat[14];
    result.mat[5] =  mat[0] * other.mat[5] + mat[1]*other.mat[4] + mat[2] * other.mat[11] + mat[3]*other.mat[10] + mat[4] * other.mat[17] + mat[5]*other.mat[16];
    result.mat[7] =  mat[6] * other.mat[1] + mat[7]*other.mat[0] + mat[8] * other.mat[7] + mat[9]*other.mat[6] + mat[10] * other.mat[13] + mat[11]*other.mat[12];
    result.mat[9] =  mat[6] * other.mat[3] + mat[7]*other.mat[2] + mat[8] * other.mat[9] + mat[9]*other.mat[8] + mat[10] * other.mat[15] + mat[11]*other.mat[14];
    result.mat[11] =  mat[6] * other.mat[5] + mat[7]*other.mat[4] + mat[8] * other.mat[11] + mat[9]*other.mat[10] + mat[10] * other.mat[17] + mat[11]*other.mat[16];
    result.mat[13] =  mat[12] * other.mat[1] + mat[13]*other.mat[0] + mat[14] * other.mat[7] + mat[15]*other.mat[6] + mat[16] * other.mat[13] + mat[17]*other.mat[12];
    result.mat[15] =  mat[12] * other.mat[3] + mat[13]*other.mat[2] + mat[14] * other.mat[9] + mat[15]*other.mat[8] + mat[16] * other.mat[15] + mat[17]*other.mat[14];
    result.mat[17] =  mat[12] * other.mat[5] + mat[13]*other.mat[4] + mat[14] * other.mat[11] +  mat[15]*other.mat[10] + mat[16] * other.mat[17] + mat[17]*other.mat[16];
*/
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            result.mat[2 * (3*i + j)] = 0;
            result.mat[2 * (3*i + j) + 1] = 0;
            for(int k = 0; k < 3; k++){
                result.mat[2 * (3*i + j)] += mat[2 * (3*i + k)] * other.mat[2 * (3*k + j)]
                                       -mat[2 * (3*i + k) + 1] * other.mat[2 * (3*k + j) + 1];
                result.mat[2 * (3*i + j) + 1] += mat[2 * (3*i + k)] * other.mat[2 * (3*k + j) + 1]
                                           +mat[2 * (3*i + k) + 1] * other.mat[2 * (3*k + j)];

            }
        }
    }
    return *this = result;
}

// multiplication
SU3& SU3::operator*=(SU3&& other) noexcept{
    SU3 result;
    /*
    //REAL
    result.mat[0] =  mat[0] * other.mat[0] - mat[1]*other.mat[1] + mat[2] * other.mat[6] - mat[3]*other.mat[7] + mat[4] * other.mat[12] - mat[5]*other.mat[13];
    result.mat[2] =  mat[0] * other.mat[2] - mat[1]*other.mat[3] + mat[2] * other.mat[8] - mat[3]*other.mat[9] + mat[4] * other.mat[14] - mat[5]*other.mat[15];
    result.mat[4] =  mat[0] * other.mat[4] - mat[1]*other.mat[5] + mat[2] * other.mat[10] - mat[3]*other.mat[11] + mat[4] * other.mat[16] - mat[5]*other.mat[17];
    result.mat[6] =  mat[6] * other.mat[0] - mat[7]*other.mat[1] + mat[8] * other.mat[6] - mat[9]*other.mat[7] + mat[10] * other.mat[12] - mat[11]*other.mat[13];
    result.mat[8] =  mat[6] * other.mat[2] - mat[7]*other.mat[3] + mat[8] * other.mat[8] - mat[9]*other.mat[9] + mat[10] * other.mat[14] - mat[11]*other.mat[15];
    result.mat[10] =  mat[6] * other.mat[4] - mat[7]*other.mat[5] + mat[8] * other.mat[10] - mat[9]*other.mat[11] + mat[10] * other.mat[16] - mat[11]*other.mat[17];
    result.mat[12] =  mat[12] * other.mat[0] - mat[13]*other.mat[1] + mat[14] * other.mat[6] - mat[15]*other.mat[7] + mat[16] * other.mat[12] - mat[17]*other.mat[13];
    result.mat[14] =  mat[12] * other.mat[2] - mat[13]*other.mat[3] + mat[14] * other.mat[8] - mat[15]*other.mat[9] + mat[16] * other.mat[14] - mat[17]*other.mat[15];
    result.mat[16] =  mat[12] * other.mat[4] - mat[13]*other.mat[5] +  mat[14] * other.mat[10] - mat[15]*other.mat[11] + mat[16] * other.mat[16] - mat[17]*other.mat[17];


    // COMPLEX
    result.mat[1] =  mat[0] * other.mat[1] + mat[1]*other.mat[0] + mat[2] * other.mat[7] + mat[3]*other.mat[6] + mat[4] * other.mat[13] + mat[5]*other.mat[12];
    result.mat[3] =  mat[0] * other.mat[3] + mat[1]*other.mat[2] + mat[2] * other.mat[9] + mat[3]*other.mat[8] + mat[4] * other.mat[15] + mat[5]*other.mat[14];
    result.mat[5] =  mat[0] * other.mat[5] + mat[1]*other.mat[4] + mat[2] * other.mat[11] + mat[3]*other.mat[10] + mat[4] * other.mat[17] + mat[5]*other.mat[16];
    result.mat[7] =  mat[6] * other.mat[1] + mat[7]*other.mat[0] + mat[8] * other.mat[7] + mat[9]*other.mat[6] + mat[10] * other.mat[13] + mat[11]*other.mat[12];
    result.mat[9] =  mat[6] * other.mat[3] + mat[7]*other.mat[2] + mat[8] * other.mat[9] + mat[9]*other.mat[8] + mat[10] * other.mat[15] + mat[11]*other.mat[14];
    result.mat[11] =  mat[6] * other.mat[5] + mat[7]*other.mat[4] + mat[8] * other.mat[11] + mat[9]*other.mat[10] + mat[10] * other.mat[17] + mat[11]*other.mat[16];
    result.mat[13] =  mat[12] * other.mat[1] + mat[13]*other.mat[0] + mat[14] * other.mat[7] + mat[15]*other.mat[6] + mat[16] * other.mat[13] + mat[17]*other.mat[12];
    result.mat[15] =  mat[12] * other.mat[3] + mat[13]*other.mat[2] + mat[14] * other.mat[9] + mat[15]*other.mat[8] + mat[16] * other.mat[15] + mat[17]*other.mat[14];
    result.mat[17] =  mat[12] * other.mat[5] + mat[13]*other.mat[4] + mat[14] * other.mat[11] +  mat[15]*other.mat[10] + mat[16] * other.mat[17] + mat[17]*other.mat[16];

    */
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            result.mat[2 * (3*i + j)] = 0;
            result.mat[2 * (3*i + j) + 1] = 0;
            for(int k = 0; k < 3; k++){
                result.mat[2 * (3*i + j)] += mat[2 * (3*i + k)] * other.mat[2 * (3*k + j)]
                                       -mat[2 * (3*i + k) + 1] * other.mat[2 * (3*k + j) + 1];
                result.mat[2 * (3*i + j) + 1] += mat[2 * (3*i + k)] * other.mat[2 * (3*k + j) + 1]
                                           +mat[2 * (3*i + k) + 1] * other.mat[2 * (3*k + j)];

            }
        }
    }
    return *this = result;
}


// sum with scalar
SU3& SU3::operator+=( const double scalar ) noexcept{
    for(auto& i : mat)
        i += scalar;
    return *this;
}

// subtraction with scalar
SU3& SU3::operator-=( const double scalar ) noexcept{
    for(auto& i : mat)
        i -= scalar;
    return *this;
}

// sum with scalar
SU3& SU3::operator*=( const double scalar ) noexcept{
    for(auto& i : mat)
        i *= scalar;
    return *this;
}



// Hermitean Conjugate
SU3& operator~(const SU3& a){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            R.mat[2 * (3*i + j)] = a.mat[2 * (3*j + i)];
            R.mat[2 * (3*i + j) + 1] = -a.mat[2 * (3*j + i) + 1];
        }
    }
    return R;
}

SU3& operator~(SU3&& a){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            R.mat[2 * (3*i + j)] = a.mat[2 * (3*j + i)];
            R.mat[2 * (3*i + j) + 1] = -a.mat[2 * (3*j + i) + 1];
        }
    }
    return R;
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

double SU3::imagTrace(){
    return mat[1] + mat[9] + mat[17];
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
