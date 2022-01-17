#include "MartellTimeStepData.h"

MartellTimeStepData::MartellTimeStepData()
{

}

MartellTimeStepData::MartellTimeStepData(QList<QVariant> percentWomen, QList<QVariant> meanScore,
                                         double originals, double promotionCylces):
    m_percentWomen(percentWomen),
    m_meanScore(meanScore),
    m_originals(originals),
    m_promotionCylces(promotionCylces)
{

}

QList<QVariant> MartellTimeStepData::percentWomen() const
{
    return m_percentWomen;
}

QList<QVariant> MartellTimeStepData::meanScore() const
{
    return m_meanScore;
}

double MartellTimeStepData::originals() const
{
    return m_originals;
}

double MartellTimeStepData::promotionCylces() const
{
    return m_promotionCylces;
}

QString MartellTimeStepData::toString()
{
    QString returned;
    returned.append(QString::number(promotionCylces()));
    returned.append(", ");
    for (int i = 0; i < 8; i++)
    {
        returned.append(m_meanScore.at(i).toString());
        returned.append(", ");
    }
    for (int i = 0; i < 8; i++)
    {
        returned.append(m_percentWomen.at(i).toString());
        returned.append(", ");
    }
    returned.append(QString::number(originals()));

    return returned;
}
