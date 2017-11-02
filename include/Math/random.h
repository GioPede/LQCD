#include <random>
#include "Math/su3.h"

class Random{
public:
    static double randUniform();
    static SU3 randSU3();
    static SU3 randSU3Transf(double epsilon);
private:
    static std::random_device rd;
    static std::mt19937 randomGen;
    static std::uniform_real_distribution<double> randomUniformInterval;
};
