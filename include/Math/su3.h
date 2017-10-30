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
    SU3& operator=(const SU3& other) noexcept;
    SU3& operator=(SU3&& other) noexcept;

    SU3& operator+=( const SU3& other ) noexcept;
    SU3& operator+=( SU3&& other ) noexcept;

    SU3& operator-=( const SU3& other ) noexcept;
    SU3& operator-=( SU3&& other ) noexcept;

    SU3& operator*=( const SU3& other ) noexcept;
    SU3& operator*=( SU3&& other ) noexcept;

    // operations with a scalar
    SU3& operator+=( const double scalar ) noexcept;
    SU3& operator-=( const double scalar ) noexcept;
    SU3& operator*=( const double scalar ) noexcept;

    // Setters
    void setSU3Identity();
    void setSU3Zero();
    void setSU3Random();

    // Traces
    double realTrace();
    double imagTrace();

    // Print Function
    void printSU3();
};

SU3& operator~(const SU3& a);
SU3& operator~(SU3&& a);
// Get the exponential of the matrix
SU3 exp(const SU3& Q);
SU3 exp(SU3&& Q);

SU3 getRandomTransformation(double epsilon);
double getMultSumTrace(SU3& a, SU3& b);
double getMultSumTrace(SU3&& a, SU3& b);
