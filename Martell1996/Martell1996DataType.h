#ifndef MARTELL1996DATATYPE_H
#define MARTELL1996DATATYPE_H

#include "../MalamuteCore/InheritedClasses/DataType.h"

struct runData
{
public:
    runData(QList<double> percentWomen, QList<double> averageScore, int promotionCycles) :
        m_percentWomen(percentWomen),
        m_averageScore(averageScore),
        m_promotionCycles(promotionCycles)
    {

    }

    QList<double> percentWomen() const;
    QList<double> averageScore() const;
    int promotionCycles() const;

private:
    QList<double> m_percentWomen;
    QList<double> m_averageScore;
    int m_promotionCycles;
};

struct Martell1996DataTypeAttributes : public DataTypeAttributes
{
    Martell1996DataTypeAttributes();
};

class Martell1996DataType : public DataType
{
public:
    explicit Martell1996DataType();
    explicit Martell1996DataType(int numRuns, QList<runData> m_runData);

    DataTypeAttributes getAttributes() override;

    QList<runData> getRunData() const;
    int getNumRuns() const;

private:
    int m_numRuns;
    QList<runData> m_runData;
};

#endif // MARTELL1996OUTPUTDATATYPE_H
