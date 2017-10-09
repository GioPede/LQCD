#pragma once

class OutputConf{
    public:
        OutputConf(class GaugeFieldFactory* gff);
        void writeSubLattice(int update);
        void writeConfiguration(int confNum);
private:
        class GaugeFieldFactory* m_gff = nullptr;
};
