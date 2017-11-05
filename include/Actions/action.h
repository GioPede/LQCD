#pragma once
struct SU3;

// Prototype for class Action. Every implementation of a physical action should
// inherit from this class.
class Action{
public:
    // virtual methods for the action class
    virtual double compute(int x, int y, int z, int t, int mu, SU3& newLink) = 0;
    virtual void   computeStaples(int x, int y, int z, int t, int mu) = 0;
    virtual SU3    computeDerivative(int x, int y, int z, int t, int mu) = 0;
    void initAction(class Lattice *lattice);

protected:
    class Lattice* m_lat = nullptr;
};
