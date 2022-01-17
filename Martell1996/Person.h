#ifndef PERSON_H
#define PERSON_H

#include <QObject>

class Person : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int employeeType READ employeeType WRITE setEmployeeType NOTIFY employeeTypeChanged)
    Q_PROPERTY(double score READ score WRITE setScore NOTIFY scoreChanged)
    Q_PROPERTY(bool original READ original WRITE setOriginal NOTIFY originalChanged)

public:
    enum EmployeeType
    {
        None,
        Male,
        Female,
        SubgroupMale,
        SubgroupFemale
    };

    Person(int employeeType, double score, bool original, QObject* parent = nullptr);

    int employeeType() const;
    double score() const;
    bool original() const;

    int lastIndex() const;
    void setLastIndex(int lastIndex);

public slots:
    void setEmployeeType(int employeeType);
    void setScore(double score);
    void setOriginal(bool original); 

signals:
    void employeeTypeChanged(int employeeType);
    void scoreChanged(double score);
    void originalChanged(bool original); 

private:
    int m_employeeType;
    double m_score;
    bool m_original;
    int m_lastIndex;
};

#endif // PERSON_H
