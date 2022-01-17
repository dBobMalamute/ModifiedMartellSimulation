#include "ModifiedMartellOrganizationLevel.h"
#include <QtMath>

ModifiedMartellOrganizationLevel::ModifiedMartellOrganizationLevel(ModifiedMartellParamsEtc *params, QObject *parent) :
    QAbstractListModel(parent),
    m_paramsEtc(params),
    m_numberPositions(1),
    m_attritionRate(0.15)
{

}

ModifiedMartellOrganizationLevel::~ModifiedMartellOrganizationLevel()
{
    qDeleteAll(m_persons);
}

QJsonObject ModifiedMartellOrganizationLevel::save() const
{
    QJsonObject saveJson = QJsonObject();
    saveJson["ar"] = m_attritionRate;

    return saveJson;
}

void ModifiedMartellOrganizationLevel::load(const QJsonObject &obj)
{
    QJsonValue v = obj["ar"];

    if(!v.isUndefined())
    {
        m_attritionRate = v.toDouble();
    }
}

int ModifiedMartellOrganizationLevel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_persons.size();
}

QVariant ModifiedMartellOrganizationLevel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() > m_persons.count())
        return QVariant();

    //Valid index
    ModifiedMartellPerson* person = m_persons.at(index.row());
    if(role == ColorRole)
        return QVariant(person->color());
    if(role == ScoreRole)
        return QVariant(person->score());
    if(role == OriginalRole)
        return QVariant(person->original());
    return QVariant();
}

Qt::ItemFlags ModifiedMartellOrganizationLevel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEditable;}

QHash<int, QByteArray> ModifiedMartellOrganizationLevel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ColorRole] = "color";
    roles[ScoreRole] = "score";
    roles[OriginalRole] = "original";

    return roles;
}

int ModifiedMartellOrganizationLevel::positions() const
{
    return m_numberPositions;
}

void ModifiedMartellOrganizationLevel::setPositions(int positions)
{
    if (m_numberPositions == positions)
        return;

    m_numberPositions = positions;
    emit positionsChanged(m_numberPositions);
}

void ModifiedMartellOrganizationLevel::setAttrition(double attrition)
{
    if (qFuzzyCompare(m_attritionRate, attrition))
        return;

    m_attritionRate = attrition;
    emit attritionChanged(m_attritionRate);
}

void ModifiedMartellOrganizationLevel::reset()
{
    beginResetModel();
    qDeleteAll(m_persons);
    m_persons.clear();

    for(int i = 0; i < m_numberPositions; i++)
    {
        m_persons.append(m_paramsEtc->generatePerson(true));
    }
    endResetModel();
}

void ModifiedMartellOrganizationLevel::attrition(bool animate)
{
    if(!animate)
    {
        for(int i = 0; i < m_numberPositions; i++)
        {
            double rand = QRandomGenerator::global()->generateDouble();
            if(rand < m_attritionRate)
            {
                m_persons[i]->setRemoved();
            }
        }
    }
    else
    {
        for(int i = 0; i < m_numberPositions; i++)
        {
            double rand = QRandomGenerator::global()->generateDouble();
            if(rand < m_attritionRate)
            {
                emit removePerson(i, m_persons.at(i)->color());
                m_persons[i]->setRemoved();
            }
        }
    }
}

int ModifiedMartellOrganizationLevel::numEmptyPositions()
{
    int emptyPositions = 0;
    for(int i = 0; i < m_numberPositions; i++)
    {
        if(m_persons.at(i)->removed())
            emptyPositions++;
    }
    return emptyPositions;
}

QList<ModifiedMartellPerson *> ModifiedMartellOrganizationLevel::promote(int n, bool animate)
{
    QList<ModifiedMartellPerson*> promotees;

    for(int i = 0; i < n; i++)
    {
        //Find the best person
        double highestScore = -900.0;
        int highestScoreIndex = -1;

        for(int j = 0; j < m_numberPositions; j++)
        {
            if(m_persons[j]->score() > highestScore)
            {
                highestScore = m_persons[j]->score();
                highestScoreIndex = j;
            }
        }

        if(highestScoreIndex  == -1) //Everyone is promoted
            break;

        promotees.append(m_persons[highestScoreIndex]);
        m_persons[highestScoreIndex] = new ModifiedMartellPerson(QColor("transparent"), -1000, false, SimulationGender::UNSPECIFIED);
        m_persons[highestScoreIndex]->setRemoved();
    }

    if(animate)
        updateDisplay();

    return promotees;
}

void ModifiedMartellOrganizationLevel::addPromoted(QList<ModifiedMartellPerson *> newPersons, bool animate)
{
    int j = 0;
    int leftToAdd = newPersons.length();
    if(!animate)
    {
        for(int i = 0; i < m_numberPositions; i++)
        {
            if(m_persons.at(i)->removed())
            {
                leftToAdd--;
                delete m_persons[i];
                m_persons[i] = newPersons[j];
                j++;

                if(leftToAdd == 0)
                    return;
            }
        }
    }
    else
    {
        for(int i = 0; i < m_numberPositions; i++)
        {
            if(m_persons.at(i)->removed())
            {
                leftToAdd--;
                delete m_persons[i];
                m_persons[i] = newPersons[j];
                emit addNewHire(i, newPersons.at(j)->color());
                j++;

                if(leftToAdd == 0)
                    return;
            }
        }
    }

}

void ModifiedMartellOrganizationLevel::addNewHires(bool animate)
{
    if(!animate)
    {
        for(int i = 0; i < m_numberPositions; i++)
        {
            if(m_persons.at(i)->removed())
            {
                delete m_persons[i];
                m_persons[i] = m_paramsEtc->generatePerson(false);
            }
        }
    }
    else
    {
        for(int i = 0; i < m_numberPositions; i++)
        {
            if(m_persons.at(i)->removed())
            {
                delete m_persons[i];
                m_persons[i] = m_paramsEtc->generatePerson(false);
                emit addNewHire(i, m_persons.at(i)->color());
            }
        }
    }

}

void ModifiedMartellOrganizationLevel::updateDisplay()
{
    QModelIndex first = QAbstractItemModel::createIndex(0, 0);
    QModelIndex last = QAbstractItemModel::createIndex(m_persons.length(), 0);
    emit dataChanged(first, last);
}

double ModifiedMartellOrganizationLevel::percentWomen()
{
    if(m_numberPositions == 0)
        return 0.0;

    int numWomen = 0;
    int numPeople = 0;
    for(int i = 0; i < m_numberPositions; i++)
    {
        if(m_persons.at(i)->score() > -999)
        {
            numPeople++;
            if(m_persons.at(i)->gender() == SimulationGender::WOMEN)
                numWomen++;
        }
    }

    return 100.0 * numWomen / numPeople;
}

double ModifiedMartellOrganizationLevel::averageScore()
{
    if(m_numberPositions == 0)
        return 0.0;

    double avgScore = 0.0;
    int numPeople = 0;
    for(int i = 0; i < m_numberPositions; i++)
    {
        if(m_persons.at(i)->score() > -999)
        {
            numPeople++;
            avgScore += m_persons.at(i)->score();
        }
    }

    avgScore /= numPeople;
    return avgScore;
}

int ModifiedMartellOrganizationLevel::originalEmployees()
{
    int originals = 0;
    for(int i = 0; i < m_numberPositions; i++)
    {
        if(m_persons.at(i) && m_persons.at(i)->original())
            originals++;
    }
    return originals;
}

