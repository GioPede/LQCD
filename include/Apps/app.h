#pragma once
#include <vector>
#include <random>

class App{
    public:
	    App();

		// setters-getters
        virtual void addObservable(class Observable* observable) = 0;
        virtual const char* getOutDir() { return m_outDir; }
        virtual char getStartType() { return m_startType; }
        virtual class Point& getLatticeSite(int x, int y, int z, int t) = 0;
        virtual class Parallel* getParallel() { return m_parallel; }
        virtual std::vector<int>& getSize() { return m_size; }

    private:
		// member classes
		class Lattice* m_lat = nullptr;
        class Parallel* m_parallel = nullptr;
		class OutputConf* m_outputConf = nullptr;
		class InputConf* m_inputConf = nullptr;
		std::vector<class Observable*> m_obs;
		std::mt19937* m_random = nullptr;

		// member variables
		long m_seed;
		const char* m_outDir;
		char m_startType;
        std::vector<int> m_size;

};
