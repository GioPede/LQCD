#pragma once

// Prototype for class Observable. Every implementation of a physical observable
// should inherit from this class.
class Observable{
public:
    // virtual methods
    virtual void compute() = 0;
    virtual const char* getName() = 0;
    virtual void initObservable(class Lattice *lattice) = 0;
    double value();

protected:
    void gatherResults();
    class Lattice* m_lat = nullptr;
    double m_value;
};

