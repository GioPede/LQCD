#pragma once
#include <string>

// Prototype for class Observable. Every implementation of a physical observable
// should inherit from this class.
class Observable{
public:
    // virtual methods
    virtual void compute() = 0;
    virtual void initObservable(class Lattice *lattice) = 0;
    const char* getName();
    double value();

    double plaq, energy, topc;
protected:
    void gatherResults();
    class Lattice* m_lat = nullptr;
    double m_value;
    std::string m_name;
};

