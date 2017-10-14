#pragma once

struct SU3{
    double mat[18];
    void operator=(SU3 b){
        for(int i = 0; i < 18; i++)
            this->mat[i] = b.mat[i];
    }

    // Print Function
    void printSU3();

    // Identity Set
    void setSU3Identity();

    // Zero Set
    void setSU3Zero();

    // Random Set
    void setSU3Random();

    // Get Real Component of Trace
    double realTrace();
};

SU3 operator+(SU3 a, SU3 b);
void operator+=(SU3 &a, SU3 b);

SU3 operator-(SU3 a, SU3 b);
void operator-=(SU3 &a, SU3 b);

SU3 operator*(SU3 a, SU3 b);
void operator*=(SU3 &a, SU3 b);

SU3 operator~(SU3 a);

SU3 operator*(SU3 a, double b);

SU3 getRandomTransformation(double epsilon);
double getMultSumTrace(SU3& a, SU3& b);
