#include "Martell1996TableLevel.h"

Martell1996TableLevel::Martell1996TableLevel(QObject *parent) : QObject(parent)
{

}

Martell1996TableLevel::Martell1996TableLevel(const int level, const double meanScore,
                                             const int numPositions, const double percentWomen, QObject *parent) :
    QObject(parent),
    m_level(level),
    m_meanScore(meanScore),
    m_numPositions(numPositions),
    m_percentWomen(percentWomen)
{

}

int Martell1996TableLevel::level() const
{
    return m_level;
}

double Martell1996TableLevel::meanScore() const
{
    return m_meanScore;
}

double Martell1996TableLevel::percentWomen() const
{
    return m_percentWomen;
}

void Martell1996TableLevel::setLevel(int level)
{
    if (m_level == level)
        return;

    m_level = level;
    emit levelChanged(m_level);
}

void Martell1996TableLevel::setMeanScore(double meanScore)
{
    if (qFuzzyCompare(m_meanScore, meanScore))
        return;

    m_meanScore = meanScore;
    emit meanScoreChanged(m_meanScore);
}

void Martell1996TableLevel::setPercentWomen(double percentWomen)
{
    if (qFuzzyCompare(m_percentWomen, percentWomen))
        return;

    m_percentWomen = percentWomen;
    emit percentWomenChanged(m_percentWomen);
}

void Martell1996TableLevel::setNumPositions(int numPositions)
{
    if (m_numPositions == numPositions)
        return;

    m_numPositions = numPositions;
    emit numPositionsChanged(m_numPositions);
}

int Martell1996TableLevel::numPositions() const
{
    return m_numPositions;
}
