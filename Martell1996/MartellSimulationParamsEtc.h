#ifndef MARTELLSIMULATIONPARAMSETC_H
#define MARTELLSIMULATIONPARAMSETC_H

#include <QRandomGenerator>
#include <QList>


class MartellSimulationParamsEtc
{
public:
    MartellSimulationParamsEtc();

    void prepareMaleCDF(std::vector<double> xValues, std::vector<double> yValues);
    void prepareFemaleCDF(std::vector<double> xValues, std::vector<double> yValues);

    double sampleMaleDistribution();
    double sampleFemaleDistribution();

    void prepareSubgroupMaleCDF(std::vector<double> xValues, std::vector<double> yValues);
    void prepareSubgroupFemaleCDF(std::vector<double> xValues, std::vector<double> yValues);

    double sampleSubgroupMaleDistribution();
    double sampleSubgroupFemaleDistribution();

    double getPercentMen() const;
    void setPercentMen(double value);

    double getPercentSubgroupMen() const;
    void setPercentSubgroupMen(double value);

    double getPercentWomen() const;
    void setPercentWomen(double value);

    double getPercentSubgroupWomen() const;
    void setPercentSubgroupWomen(double value);

    double totalPercent();

private:
    std::vector<double> m_MaleCDF;
    std::vector<double> m_MaleXValues;

    std::vector<double> m_FemaleCDF;
    std::vector<double> m_FemaleXValues;

    std::vector<double> m_SubgroupMaleCDF;
    std::vector<double> m_SubgroupMaleXValues;

    std::vector<double> m_SubgroupFemaleCDF;
    std::vector<double> m_SubgroupFemaleXValues;

    double percentMen;
    double percentSubgroupMen;
    double percentWomen;
    double percentSubgroupWomen;
};

#endif // MARTELLSIMULATIONPARAMSETC_H
