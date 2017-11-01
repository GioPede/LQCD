#pragma once

// Prototype for class Observable. Every implementation of a physical observable
// should inherit from this class.
class Observable{
public:
    Observable();
    class Lattice* m_lat = nullptr;

    // virtual methods
    virtual void initObservable(class Lattice *lattice) = 0;
    virtual void compute() = 0;
    virtual double value() = 0;
    virtual const char* getName() = 0;
};

