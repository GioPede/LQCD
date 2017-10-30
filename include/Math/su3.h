#pragma once
#include <array>
#include "operators.hpp"

struct SU3
  : tao::operators::commutative_addable< SU3 >,
    tao::operators::subtractable< SU3 >,
    tao::operators::multipliable< SU3 >,
    tao::operators::addable_left< SU3, double >,
    tao::operators::subtractable_left< SU3, double >,
    tao::operators::addable< SU3, double >,
    tao::operators::subtractable< SU3, double >,
    tao::operators::multipliable< SU3, double >
{
    std::array<double, 18> mat;

    // constructors
    SU3 () noexcept;
    SU3 (double value) noexcept;
    SU3 (const SU3 &source) noexcept;
    SU3 (SU3 && source) noexcept;

    // Operations between elements
    inline SU3& operator=(const SU3& other) noexcept;
    inline SU3& operator=(SU3&& other) noexcept;

    inline SU3& operator+=( const SU3& other ) noexcept;
    inline SU3& operator+=( SU3&& other ) noexcept;

    inline SU3& operator-=( const SU3& other ) noexcept;
    inline SU3& operator-=( SU3&& other ) noexcept;

    inline SU3& operator*=( const SU3& other ) noexcept;
    inline SU3& operator*=( SU3&& other ) noexcept;

    // operations with a scalar
    inline SU3& operator+=( const double scalar ) noexcept;
    inline SU3& operator-=( const double scalar ) noexcept;
    inline SU3& operator*=( const double scalar ) noexcept;

    // Setters
    void setSU3Identity();
    void setSU3Zero();
    void setSU3Random();

    // Traces
    double realTrace();
    double imagTrace();

    // Get the exponential of the matrix
    SU3 exp();

    // Print Function
    void printSU3();
};

// assignment
SU3& SU3::operator=(SU3&& other) noexcept{
    mat = std::move(other.mat);
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
inline SU3 operator~(const SU3& a){
    SU3 c;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            c.mat[2 * (3*i + j)] = a.mat[2 * (3*j + i)];
            c.mat[2 * (3*i + j) + 1] = -a.mat[2 * (3*j + i) + 1];
        }
    }
    return c;
}

inline SU3 operator~(SU3&& a){
    SU3 c;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            c.mat[2 * (3*i + j)] = a.mat[2 * (3*j + i)];
            c.mat[2 * (3*i + j) + 1] = -a.mat[2 * (3*j + i) + 1];
        }
    }
    return c;
}


SU3 getRandomTransformation(double epsilon);
double getMultSumTrace(SU3& a, SU3& b);
