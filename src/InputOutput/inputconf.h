#pragma once
#include <string>

class InputConf{
    public:
        InputConf(class GaugeFieldFactory* gff);
        void readSubLattice(int update);
        void readConfiguration(int confNum);
        InputConf(class GaugeFieldReader *gfr);
        void readConfiguration(std::string inputFile);
private:
        class GaugeFieldFactory* m_gff = nullptr;
        class GaugeFieldReader* m_gfr = nullptr;
};
