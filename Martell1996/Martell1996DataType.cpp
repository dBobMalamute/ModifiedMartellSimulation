#include "Martell1996DataType.h"

Martell1996DataTypeAttributes::Martell1996DataTypeAttributes()
{
    m_name = "Martell 1996 simulation data output";
    m_color = "#6a6acc";
    m_abbreviation = "data";
    m_description = "Holds the data for export for Martell et al 1996.";
    m_authors << "dBob";
}

DataTypeAttributes Martell1996DataType::getAttributes()
{
    return Magic<Martell1996DataTypeAttributes>::cast();
}

QList<runData> Martell1996DataType::getRunData() const
{
    return m_runData;
}

int Martell1996DataType::getNumRuns() const
{
    return m_numRuns;
}

Martell1996DataType::Martell1996DataType()
{
    m_numRuns = 0;
    m_runData = QList<runData>();
}

Martell1996DataType::Martell1996DataType(int numRuns, QList<runData> m_runData) :
    m_numRuns(numRuns),
    m_runData(m_runData)
{

}

QList<double> runData::percentWomen() const
{
    return m_percentWomen;
}

QList<double> runData::averageScore() const
{
    return m_averageScore;
}

int runData::promotionCycles() const
{
    return m_promotionCycles;
}
