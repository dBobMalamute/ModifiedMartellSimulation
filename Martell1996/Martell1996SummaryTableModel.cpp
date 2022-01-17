#include "Martell1996SummaryTableModel.h"

Martell1996SummaryTableModel::Martell1996SummaryTableModel(QObject *parent) : QAbstractListModel(parent)
{

}


int Martell1996SummaryTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_levels.size();
}

QVariant Martell1996SummaryTableModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= m_levels.size())
        return QVariant();

    Martell1996TableLevel* level = m_levels.at(index.row());
    if(role == LevelRole)
        return QVariant(level->level());
    if(role == MeanScoreRole)
        return QVariant(level->meanScore());
    if(role == NumPositionsRole)
        return QVariant(level->numPositions());
    if(role == PercentWomenRole)
        return QVariant(level->percentWomen());

    return QVariant();

}

bool Martell1996SummaryTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Martell1996TableLevel* level = m_levels.at(index.row());
    bool somethingChanged = false;

    switch (role)
    {
    case LevelRole:
    {
        if(level->level() != value.toInt())
        {
            level->setLevel(value.toInt());
            somethingChanged = true;
        }
        break;
    }
    case MeanScoreRole:
    {
        if(level->meanScore() != value.toDouble())
        {
            level->setMeanScore(value.toDouble());
            somethingChanged = true;
        }
        break;
    }
    case NumPositionsRole:
    {
        if(level->numPositions() != value.toInt())
        {
            level->setNumPositions(value.toInt());
            somethingChanged = true;
        }
        break;
    }
    case PercentWomenRole:
    {
        if(level->percentWomen() != value.toDouble())
        {
            level->setPercentWomen(value.toDouble());
            somethingChanged = true;
        }
        break;
    }
    }

    if(somethingChanged)
    {
        emit dataChanged(index,index,QVector<int>() << role);
        return true;
    }

    return false;
}
Qt::ItemFlags Martell1996SummaryTableModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> Martell1996SummaryTableModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[LevelRole] = "level";
    roles[MeanScoreRole] = "meanScore";
    roles[NumPositionsRole] = "positions";
    roles[PercentWomenRole] = "percentWomen";
    return roles;
}

void Martell1996SummaryTableModel::addLevel(int level, double meanScore, int numPositions, double percentWomen)
{
    Martell1996TableLevel* tableLevel = new Martell1996TableLevel(level,meanScore,numPositions,
                                                         percentWomen,this);

    const int index = m_levels.size();
    beginInsertRows(QModelIndex(), index,index);
    m_levels.append(tableLevel);
    endInsertRows();
}

void Martell1996SummaryTableModel::clearData()
{
    beginRemoveRows(QModelIndex(),0, 7);
    m_levels.clear();
    endRemoveRows();
}
