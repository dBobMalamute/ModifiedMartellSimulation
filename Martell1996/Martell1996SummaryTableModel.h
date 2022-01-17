#ifndef MARTELL1996SUMMARYTABLEMODEL_H
#define MARTELL1996SUMMARYTABLEMODEL_H

#include <QObject>
#include <QDebug>
#include <QAbstractListModel>
#include "Martell1996TableLevel.h"

class Martell1996SummaryTableModel : public QAbstractListModel
{
    Q_OBJECT

    enum TableLevelRoles
    {
        LevelRole = Qt::UserRole + 1,
        MeanScoreRole,
        NumPositionsRole,
        PercentWomenRole
    };

public:
    explicit Martell1996SummaryTableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addLevel(int level, double meanScore, int numPositions, double percentWomen);
    void clearData();

signals:

public slots:

private:
    QList<Martell1996TableLevel*> m_levels;
};

#endif // MARTELL1996SUMMARYTABLEMODEL_H
