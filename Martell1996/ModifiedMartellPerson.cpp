#include "ModifiedMartellPerson.h"

ModifiedMartellPerson::ModifiedMartellPerson(QColor color, double score, bool original, SimulationGender::GENDER gender, QObject *parent) :
    QObject(parent),
    m_color(color),
    m_score(score),
    m_original(original),
    m_gender(gender),
    m_removed(false)
{

}

double ModifiedMartellPerson::score() const
{
    return m_score;
}

bool ModifiedMartellPerson::original() const
{
    return m_original;
}

SimulationGender::GENDER ModifiedMartellPerson::gender() const
{
    return m_gender;
}

void ModifiedMartellPerson::setRemoved()
{
    m_removed = true;
    m_color = QColor("transparent");
    m_score = -1000;
    m_original = false;
    m_gender = SimulationGender::UNSPECIFIED;
}

bool ModifiedMartellPerson::removed() const
{
    return m_removed;
}

QColor ModifiedMartellPerson::color() const
{
    return m_color;
}

