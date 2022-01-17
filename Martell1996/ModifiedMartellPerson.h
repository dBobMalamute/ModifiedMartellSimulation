#ifndef MODIFIEDMARTELLPERSON_H
#define MODIFIEDMARTELLPERSON_H

#include <QObject>
#include <QPointF>
#include <QColor>
#include "SimulationGender.h"

class ModifiedMartellPerson : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ color)
    Q_PROPERTY(double score READ score)
    Q_PROPERTY(bool original READ original)

public:
    ModifiedMartellPerson(QColor color, double score, bool original, SimulationGender::GENDER gender, QObject* parent = nullptr);

    QColor color() const;
    double score() const;
    bool original() const;
    SimulationGender::GENDER gender() const;

    void setRemoved();
    bool removed() const;

private:
    QColor m_color;
    double m_score;
    bool m_original;
    SimulationGender::GENDER m_gender;
    bool m_removed;
};

#endif // MODIFIEDMARTELLPERSON_H
