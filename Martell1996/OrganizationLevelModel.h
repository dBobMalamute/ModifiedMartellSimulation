#ifndef ORGANIZATIONLEVELMODEL_H
#define ORGANIZATIONLEVELMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QDebug>

#include <QJsonObject>

#include "Person.h"
#include "MartellSimulationParamsEtc.h"

class OrganizationLevelModel : public QAbstractListModel
{
    Q_OBJECT

    enum PersonRoles
    {
        EmployeeTypeRole = Qt::UserRole + 1,
        ScoreRole,
        OriginalRole
    };

public:
    explicit OrganizationLevelModel(QObject *parent = nullptr);
    ~OrganizationLevelModel();

    QJsonObject save() const;
    void load(const QJsonObject &obj);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setNumberPersons(int numberPersons);
    void setNull();

    void reset();
    void removeAtAttritionRate(bool animate);
    int numEmptyPositions();
    void addNewlyPromotedPersons(QList<Person* > newPeople, bool animate);
    QList<Person*> promoteBestNPeople(int n, bool animate);
    void fillLevelWithNewHires(bool animate);
    void updateDisplay();

    double percentWomen();
    double averageScore();
    int originalEmployees();

    void setCdfHolder(MartellSimulationParamsEtc *cdfHolder);

    double attritionRate() const;
    void setAttritionRate(double attritionRate);

public slots:
    int numberPersons() const;

signals:
    void removePerson(int index, int type);
    void promotePerson(int oldIndex, int newIndex, int type);
    void newHire(int index, int type);

private:
    QList<Person *> m_persons;
    MartellSimulationParamsEtc* m_cdfHolder;

    int m_numberPersons;
    double m_attritionRate;
};

#endif // ORGANIZATIONLEVELMODEL_H
