#include "Person.h"

Person::Person(int employeeType, double score, bool original, QObject *parent) :
    QObject(parent),
    m_employeeType(employeeType),
    m_score(score),
    m_original(original)
{

}


double Person::score() const
{
    return m_score;
}

bool Person::original() const
{
    return m_original;
}

int Person::employeeType() const
{
    return m_employeeType;
}

void Person::setScore(double score)
{
    if (qFuzzyCompare(m_score, score))
        return;

    m_score = score;
    emit scoreChanged(m_score);
}

void Person::setOriginal(bool original)
{
    if (m_original == original)
        return;

    m_original = original;
    emit originalChanged(m_original);
}

int Person::lastIndex() const
{
    return m_lastIndex;
}

void Person::setLastIndex(int lastIndex)
{
    m_lastIndex = lastIndex;
}

void Person::setEmployeeType(int employeeType)
{
    if (m_employeeType == employeeType)
        return;

    m_employeeType = employeeType;
    emit employeeTypeChanged(m_employeeType);
}

