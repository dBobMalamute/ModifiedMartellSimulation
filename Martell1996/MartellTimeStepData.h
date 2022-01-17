#ifndef MARTELLTIMESTEPDATA_H
#define MARTELLTIMESTEPDATA_H

#include <QList>
#include <QVariant>

class MartellTimeStepData
{
public:
    MartellTimeStepData();
    MartellTimeStepData(QList<QVariant> percentWomen, QList<QVariant> meanScore, double originals, double promotionCylces);

    QList<QVariant> percentWomen() const;
    QList<QVariant> meanScore() const;
    double originals() const;
    double promotionCylces() const;

    QString toString();

private:
    QList<QVariant> m_percentWomen;
    QList<QVariant> m_meanScore;
    double m_originals;
    double m_promotionCylces;
};

#endif // MARTELLTIMESTEPDATA_H
