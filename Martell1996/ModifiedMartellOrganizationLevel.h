#ifndef MODIFIEDMARTELLORGANIZATIONLEVEL_H
#define MODIFIEDMARTELLORGANIZATIONLEVEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QDebug>
#include <QJsonObject>
#include <QRectF>

#include "ModifiedMartellPerson.h"
#include "ModifiedMartellParamsEtc.h"

class ModifiedMartellOrganizationLevel : public QAbstractListModel
{
    Q_OBJECT

    enum PersonRoles
    {
        ColorRole = Qt::UserRole + 1,
        ScoreRole,
        OriginalRole
    };

public:
    explicit ModifiedMartellOrganizationLevel(ModifiedMartellParamsEtc* params, QObject *parent = nullptr);
    ~ModifiedMartellOrganizationLevel();

    QJsonObject save() const;
    void load(const QJsonObject &obj);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QHash<int, QByteArray> roleNames() const override;

    int positions() const;
    double attrition() const;

    void reset();
    void attrition(bool animate);
    int numEmptyPositions();
    QList<ModifiedMartellPerson*> promote(int n, bool animate);
    void addPromoted(QList<ModifiedMartellPerson*> newPersons, bool animate);
    void addNewHires(bool animate);
    void updateDisplay();

    double percentWomen();
    double averageScore();
    int originalEmployees();

signals:
    void positionsChanged(int positions);
    void attritionChanged(double attrition);

    void removePerson(int index, QColor color);
    void addNewHire(int index, QColor color);

public slots:
    void setPositions(int positions);
    void setAttrition(double attrition);

private:
    ModifiedMartellParamsEtc* m_paramsEtc;

    int m_numberPositions;
    double m_attritionRate;

    QList<ModifiedMartellPerson*> m_persons;
};

#endif // MODIFIEDMARTELLORGANIZATIONLEVEL_H
