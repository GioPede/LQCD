#pragma once
#include <vector>
#include <random>
#include <array>

class App{
    public:
	    App();
		// setters-getters
        virtual void addObservable(class Observable* observable) = 0;
        virtual const char* getOutDir() { return m_outDir; }
        virtual char getStartType() { return m_startType; }
        virtual class Point& getLatticeSite(int x, int y, int z, int t) = 0;
        virtual std::array<int, 4>& getSize() { return m_size; }
        virtual std::vector<double>& getObsValues() { return m_obsValues; }
        virtual std::vector<class Observable*>& getObs() { return m_obs; }


    private:
		// member classes
        class Lattice* m_lat = nullptr;
		class OutputConf* m_outputConf = nullptr;
		class InputConf* m_inputConf = nullptr;
		std::vector<class Observable*> m_obs;
        std::vector<double> m_obsValues;
		std::mt19937* m_random = nullptr;

		// member variables
		long m_seed;
		const char* m_outDir;
		char m_startType;
        std::array<int, 4> m_size;

};
