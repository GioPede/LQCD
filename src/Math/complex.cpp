#include "complex.h"
#include <cmath>
#include <cstdio>

/*******************************************************
* Functions between complex elements, defined as arrays.
********************************************************/


//////  BINARY OPERATIONS  //////

// Sum between complex
complex operator+(complex a, complex b){
    complex c;
    c.real = a.real + b.real;
    c.imag = a.imag + b.imag;
    return c;
}

void operator+=(complex&a, complex b){
    a.real += b.real;
    a.imag += b.imag;
}

// Subtraction between complex
complex operator-(complex a, complex b){
    complex c;
    c.real = a.real - b.real;
    c.imag = a.imag - b.imag;
    return c;
}

void operator-=(complex&a, complex b){
    a.real -= b.real;
    a.imag -= b.imag;
}

// Multiplication between complex
complex operator*(complex a, complex b){
    complex c;
    c.real = a.real*b.real - a.imag*b.imag;
    c.imag = a.real*b.imag + a.imag*b.real;
    return c;
}

void operator*=(complex&a, complex b){
    complex c;
    c.real = a.real*b.real - a.imag*b.imag;
    c.imag = a.real*b.imag + a.imag*b.real;
    a = c;
}

// Division by a scalar
complex operator/(complex a, double b){
    complex c;
    c.real = a.real/b;
    c.imag = a.imag/b;
    return c;
}


// Conjugate
complex operator~(complex a){
    complex c;
    c.real = a.real;
    c.imag = -a.imag;
    return c;
}

// Constructor
complex::complex(){
    real = 0;
    imag = 0;
}

// Norm
double complex::norm(){
    return sqrt(real*real + imag*imag);
}

// Print
void complex::printComplex(){
    printf("( %.5f , %.5f )\n", real, imag);
}
