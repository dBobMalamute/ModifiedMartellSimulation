#include "MartellSimulationParamsEtc.h"

MartellSimulationParamsEtc::MartellSimulationParamsEtc() :
    percentMen(0.5),
    percentSubgroupMen(0.00),
    percentWomen(0.5),
    percentSubgroupWomen(0.00)
{

}

void MartellSimulationParamsEtc::prepareMaleCDF(std::vector<double> xValues, std::vector<double> yValues)
{
    m_MaleXValues = xValues;

    double runningSum = 0.0;
    m_MaleCDF.clear();
    m_MaleCDF.reserve(xValues.size());
    m_MaleCDF.push_back(runningSum);
    //Trapezoids.
    for(size_t i = 0; i < xValues.size() -1; i++)
    {
        runningSum += 0.5*(yValues[i+1] + yValues[i]) * (xValues[i+1] - xValues[i]);
        m_MaleCDF.push_back(runningSum);
    }

    //Normalize. In a perfect world, this would not be necessary, but here the error can be a fraction of
    // a percent. This throws a index out of bounds error if not fixed. Therefore normalize the distribution.
    double norm = m_MaleCDF[m_MaleCDF.size() - 1];
    for(size_t i = 0; i < m_MaleCDF.size(); i++)
    {
        m_MaleCDF[i] /= norm;
    }
}

void MartellSimulationParamsEtc::prepareFemaleCDF(std::vector<double> xValues, std::vector<double> yValues)
{
    m_FemaleXValues = xValues;

    double runningSum = 0.0;
    m_FemaleCDF.clear();
    m_FemaleCDF.reserve(xValues.size());
    m_FemaleCDF.push_back(runningSum);
    //Trapezoids.
    for(size_t i = 0; i < xValues.size() -1; i++)
    {
        runningSum += 0.5*(yValues[i+1] + yValues[i]) * (xValues[i+1] - xValues[i]);
        m_FemaleCDF.push_back(runningSum);
    }

    //Normalize. In a perfect world, this would not be necessary, but here the error can be a fraction of
    // a percent. This throws a index out of bounds error if not fixed. Therefore normalize the distribution.
    double norm = m_FemaleCDF[m_FemaleCDF.size() - 1];
    for(size_t i = 0; i < m_FemaleCDF.size(); i++)
    {
        m_FemaleCDF[i] /= norm;
    }
}

void MartellSimulationParamsEtc::prepareSubgroupMaleCDF(std::vector<double> xValues, std::vector<double> yValues)
{
    m_SubgroupMaleXValues = xValues;

    double runningSum = 0.0;
    m_SubgroupMaleCDF.clear();
    m_SubgroupMaleCDF.reserve(xValues.size());
    m_SubgroupMaleCDF.push_back(runningSum);
    //Trapezoids.
    for(size_t i = 0; i < xValues.size() -1; i++)
    {
        runningSum += 0.5*(yValues[i+1] + yValues[i]) * (xValues[i+1] - xValues[i]);
        m_SubgroupMaleCDF.push_back(runningSum);
    }

    //Normalize. In a perfect world, this would not be necessary, but here the error can be a fraction of
    // a percent. This throws a index out of bounds error if not fixed. Therefore normalize the distribution.
    double norm = m_SubgroupMaleCDF[m_SubgroupMaleCDF.size() - 1];
    for(size_t i = 0; i < m_SubgroupMaleCDF.size(); i++)
    {
        m_SubgroupMaleCDF[i] /= norm;
    }
}

void MartellSimulationParamsEtc::prepareSubgroupFemaleCDF(std::vector<double> xValues, std::vector<double> yValues)
{
    m_SubgroupFemaleXValues = xValues;

    double runningSum = 0.0;
    m_SubgroupFemaleCDF.clear();
    m_SubgroupFemaleCDF.reserve(xValues.size());
    m_SubgroupFemaleCDF.push_back(runningSum);
    //Trapezoids.
    for(size_t i = 0; i < xValues.size() -1; i++)
    {
        runningSum += 0.5*(yValues[i+1] + yValues[i]) * (xValues[i+1] - xValues[i]);
        m_SubgroupFemaleCDF.push_back(runningSum);
    }

    //Normalize. In a perfect world, this would not be necessary, but here the error can be a fraction of
    // a percent. This throws a index out of bounds error if not fixed. Therefore normalize the distribution.
    double norm = m_SubgroupFemaleCDF[m_SubgroupFemaleCDF.size() - 1];
    for(size_t i = 0; i < m_SubgroupFemaleCDF.size(); i++)
    {
        m_SubgroupFemaleCDF[i] /= norm;
    }
}

double MartellSimulationParamsEtc::sampleMaleDistribution()
{
    double random = QRandomGenerator::global()->generateDouble();
    auto lower = std::lower_bound(m_MaleCDF.begin(),m_MaleCDF.end(), random);

    int index = std::distance(m_MaleCDF.begin(), lower);

    double xVal = m_MaleXValues[index-1] + (m_MaleXValues[index] - m_MaleXValues[index - 1]) *
            (random - m_MaleCDF[index - 1]) / (m_MaleCDF[index] - m_MaleCDF[index - 1]);

    return xVal;
}

double MartellSimulationParamsEtc::sampleFemaleDistribution()
{
    double random = QRandomGenerator::global()->generateDouble();
    auto lower = std::lower_bound(m_FemaleCDF.begin(),m_FemaleCDF.end(), random);

    int index = std::distance(m_FemaleCDF.begin(), lower);

    double xVal = m_FemaleXValues[index-1] + (m_FemaleXValues[index] - m_FemaleXValues[index - 1]) *
            (random - m_FemaleCDF[index - 1]) / (m_FemaleCDF[index] - m_FemaleCDF[index - 1]);

    return xVal;
}

double MartellSimulationParamsEtc::sampleSubgroupMaleDistribution()
{
    double random = QRandomGenerator::global()->generateDouble();
    auto lower = std::lower_bound(m_SubgroupMaleCDF.begin(),m_SubgroupMaleCDF.end(), random);

    int index = std::distance(m_SubgroupMaleCDF.begin(), lower);

    double xVal = m_SubgroupMaleXValues[index-1] + (m_SubgroupMaleXValues[index] - m_SubgroupMaleXValues[index - 1]) *
            (random - m_SubgroupMaleCDF[index - 1]) / (m_SubgroupMaleCDF[index] - m_SubgroupMaleCDF[index - 1]);

    return xVal;
}

double MartellSimulationParamsEtc::sampleSubgroupFemaleDistribution()
{
    double random = QRandomGenerator::global()->generateDouble();
    auto lower = std::lower_bound(m_SubgroupFemaleCDF.begin(),m_SubgroupFemaleCDF.end(), random);

    int index = std::distance(m_SubgroupFemaleCDF.begin(), lower);

    double xVal = m_SubgroupFemaleXValues[index-1] + (m_SubgroupFemaleXValues[index] - m_SubgroupFemaleXValues[index - 1]) *
            (random - m_SubgroupFemaleCDF[index - 1]) / (m_SubgroupFemaleCDF[index] - m_SubgroupFemaleCDF[index - 1]);

    return xVal;
}

double MartellSimulationParamsEtc::getPercentMen() const
{
    return percentMen;
}

void MartellSimulationParamsEtc::setPercentMen(double value)
{
    percentMen = value;
}

double MartellSimulationParamsEtc::getPercentSubgroupMen() const
{
    return percentSubgroupMen;
}

void MartellSimulationParamsEtc::setPercentSubgroupMen(double value)
{
    percentSubgroupMen = value;
}

double MartellSimulationParamsEtc::getPercentWomen() const
{
    return percentWomen;
}

void MartellSimulationParamsEtc::setPercentWomen(double value)
{
    percentWomen = value;
}

double MartellSimulationParamsEtc::getPercentSubgroupWomen() const
{
    return percentSubgroupWomen;
}

void MartellSimulationParamsEtc::setPercentSubgroupWomen(double value)
{
    percentSubgroupWomen = value;
}

double MartellSimulationParamsEtc::totalPercent()
{
    return percentMen + percentSubgroupMen + percentWomen + percentSubgroupWomen;
}

