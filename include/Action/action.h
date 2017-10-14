#pragma once

// Prototype for class Action. Every implementation of a physical action should
// inherit from this class.
class Action{

public:
    Action();
    // virtual methods for the action class
    virtual void initAction(class Lattice* lattice) = 0;
    virtual double compute(int x, int y, int z, int t, int mu,
                           struct SU3& newLink, struct SU3& constPart) = 0;
    virtual struct SU3 computeConstant(int x, int y, int z, int t, int mu) = 0;
};
