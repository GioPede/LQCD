#pragma once

struct complex{
    complex();
    double real;
    double imag;

    // Print Function
    void printComplex();

    // Norm
    double norm();
};

complex operator+(complex a, complex b);
void operator+=(complex &a, complex b);

complex operator-(complex a, complex b);
void operator-=(complex &a, complex b);

complex operator*(complex a, complex b);
void operator*=(complex &a, complex b);

complex operator/(complex a, double b);

complex operator~(complex a);
