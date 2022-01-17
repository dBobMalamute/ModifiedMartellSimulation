#include "OrganizationLevelModel.h"
#include <QRandomGenerator>

OrganizationLevelModel::OrganizationLevelModel(QObject *parent) :
    QAbstractListModel(parent),
    m_numberPersons(10),
    m_attritionRate(0.15)
{

}

OrganizationLevelModel::~OrganizationLevelModel()
{
    qDeleteAll(m_persons);
}

QJsonObject OrganizationLevelModel::save() const
{
    QJsonObject saveJson = QJsonObject();
    saveJson["ar"] = m_attritionRate;

    return saveJson;
}

void OrganizationLevelModel::load(const QJsonObject &obj)
{
    QJsonValue v = obj["ar"];

    if(!v.isUndefined())
    {
        m_attritionRate = v.toDouble();
    }
}

int OrganizationLevelModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_persons.size();
}

QVariant OrganizationLevelModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() > m_persons.count())
        return QVariant();

    //Valid index
    Person* person = m_persons.at(index.row());
    if(role == EmployeeTypeRole)
        return QVariant(person->employeeType());
    if(role == ScoreRole)
        return QVariant(person->score());
    if(role == OriginalRole)
        return QVariant(person->original());
    return QVariant();
}

bool OrganizationLevelModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Person* person = m_persons.at(index.row());
    bool somethingChanged = false;

    switch (role)
    {
    case EmployeeTypeRole:
    {
        if(person->employeeType() != value.toInt())
        {
            person->setEmployeeType(value.toInt());
            somethingChanged = true;
        }
    }
        break;
    case ScoreRole:
    {
        if(person->score() != value.toDouble())
        {
            person->setScore(value.toDouble());
            somethingChanged = true;
        }
    }
        break;
    case OriginalRole:
    {
        if(person->original() != value.toBool())
        {
            person->setOriginal(value.toBool());
            somethingChanged = true;
        }
    }
    }

    if(somethingChanged)
    {
        emit dataChanged(index,index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags OrganizationLevelModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> OrganizationLevelModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[EmployeeTypeRole] = "type";
    roles[ScoreRole] = "score";
    roles[OriginalRole] = "original";

    return roles;
}

int OrganizationLevelModel::numberPersons() const
{
    return m_numberPersons;
}

double OrganizationLevelModel::attritionRate() const
{
    return m_attritionRate;
}

void OrganizationLevelModel::setAttritionRate(double attritionRate)
{
    m_attritionRate = attritionRate;
}

void OrganizationLevelModel::setNumberPersons(int numberPersons)
{
    m_numberPersons = numberPersons;
}

void OrganizationLevelModel::setNull()
{
    qDeleteAll(m_persons);
    m_persons.clear();

    for(int i = 0; i < m_numberPersons; i++)
    {
        Person* newPerson = new Person(Person::None, 0.00, true);
        m_persons.append(newPerson);
    }
    updateDisplay();
}

void OrganizationLevelModel::reset()
{
    qDeleteAll(m_persons);
    m_persons.clear();

    for(int i = 0; i < m_numberPersons; i++)
    {
        double rand = QRandomGenerator::global()->generateDouble();
        int gender = 0;
        double score = 0.0;

        if(rand < m_cdfHolder->getPercentMen())
        {
            gender = Person::Male;
            score = m_cdfHolder->sampleMaleDistribution();
        }
        else if (rand < m_cdfHolder->getPercentMen() + m_cdfHolder->getPercentWomen())
        {
            gender = Person::Female;
            score = m_cdfHolder->sampleFemaleDistribution();
        }
        else if (rand < m_cdfHolder->getPercentMen() + m_cdfHolder->getPercentWomen() +
                 m_cdfHolder->getPercentSubgroupMen())
        {
            gender = Person::SubgroupMale;
            score = m_cdfHolder->sampleSubgroupMaleDistribution();
        }
        else
        {
            gender = Person::SubgroupFemale;
            score = m_cdfHolder->sampleSubgroupFemaleDistribution();
        }

        Person* newPerson = new Person(gender, score, true);
        m_persons.append(newPerson);
    }
    QModelIndex first = QAbstractItemModel::createIndex(0, 0);
    QModelIndex last = QAbstractItemModel::createIndex(m_persons.length(), 0);
    emit dataChanged(first, last);
}

void OrganizationLevelModel::removeAtAttritionRate(bool animate)
{
    for(int i = 0; i < m_numberPersons; i++)
    {
        double rand = QRandomGenerator::global()->generateDouble();
        if(rand < m_attritionRate)
        {
            //Remove this person.
            if(animate)
                emit removePerson(i, m_persons.at(i)->employeeType());

            m_persons[i]->setEmployeeType(Person::None);
            m_persons[i]->setOriginal(false);
            m_persons[i]->setScore(-1000.0);
        }
    }
}

int OrganizationLevelModel::numEmptyPositions()
{
    int emptyPositions = 0;
    for(int i = 0; i < m_numberPersons; i++)
    {
        if(m_persons[i]->employeeType() == Person::None)
            emptyPositions++;
    }
    return emptyPositions;
}

void OrganizationLevelModel::addNewlyPromotedPersons(QList<Person *> newPeople, bool animate)
{
    int j = 0;
    for(int i = 0; i < m_numberPersons; i++)
    {
        if(m_persons[i]->employeeType() == Person::None)
        {
            delete m_persons[i];
            m_persons[i] = newPeople[j];

            if(animate)
            {
                emit promotePerson(newPeople.at(j)->lastIndex(), i,
                                   newPeople.at(j)->employeeType());
            }
            j++;
        }
    }
}

QList<Person *> OrganizationLevelModel::promoteBestNPeople(int n, bool animate)
{
    QList<Person*> promotees;
    promotees.reserve(n);

    for(int i = 0; i < n; i++)
    {
        //Find the best Person;
        double highestScore = -10000;
        int highestScoreIndex = -1;

        for(int j = 0; j < m_numberPersons; j++)
        {
            if(m_persons[j]->score() > highestScore)
            {
                highestScore = m_persons[j]->score();
                highestScoreIndex = j;
            }
        }
        m_persons.at(highestScoreIndex)->setLastIndex(highestScoreIndex);
        promotees.append(m_persons[highestScoreIndex]);
        m_persons[highestScoreIndex] = new Person(Person::None, -1000, false);
    }

    if(animate)
        updateDisplay();

    return promotees;
}

void OrganizationLevelModel::fillLevelWithNewHires(bool animate)
{
    for(int i = 0; i < m_numberPersons; i++)
    {
        if(m_persons[i]->employeeType() == Person::None)
        {
            double rand = QRandomGenerator::global()->generateDouble();
            int gender = 0;
            double score = 0.0;

            if(rand < m_cdfHolder->getPercentMen())
            {
                gender = Person::Male;
                score = m_cdfHolder->sampleMaleDistribution();
            }
            else if (rand < m_cdfHolder->getPercentMen() + m_cdfHolder->getPercentWomen())
            {
                gender = Person::Female;
                score = m_cdfHolder->sampleFemaleDistribution();
            }
            else if (rand < m_cdfHolder->getPercentMen() + m_cdfHolder->getPercentWomen() +
                     m_cdfHolder->getPercentSubgroupMen())
            {
                gender = Person::SubgroupMale;
                score = m_cdfHolder->sampleSubgroupMaleDistribution();
            }
            else
            {
                gender = Person::SubgroupFemale;
                score = m_cdfHolder->sampleSubgroupFemaleDistribution();
            }
            //TODO fix percentages.
            if(animate)
            {
                emit newHire(i, gender);
            }
            m_persons[i] = new Person(gender, score, false);
        }
    }
}

void OrganizationLevelModel::updateDisplay()
{
    QModelIndex first = QAbstractItemModel::createIndex(0, 0);
    QModelIndex last = QAbstractItemModel::createIndex(m_persons.length(), 0);
    emit dataChanged(first, last);
}

double OrganizationLevelModel::percentWomen()
{
    int numWomen = 0;
    int numMen = 0;
    for(int i = 0; i < m_numberPersons; i++)
    {
        if(m_persons[i]->employeeType() == Person::Male ||
                m_persons[i]->employeeType() == Person::SubgroupMale)
        {
            numMen++;
        }
        else if(m_persons[i]->employeeType() == Person::Female ||
                m_persons[i]->employeeType() == Person::SubgroupFemale)
        {
            numWomen++;
        }
    }

    return 100.0 * numWomen / (numMen + numWomen);
}

double OrganizationLevelModel::averageScore()
{
    int numPeople = 0;
    double totalScore = 0.0;

    for(int i = 0; i < m_numberPersons; i++)
    {
        if(m_persons[i]->employeeType() != Person::None)
        {
            numPeople++;
            totalScore += m_persons[i]->score();
        }
    }

    return totalScore / numPeople;
}

int OrganizationLevelModel::originalEmployees()
{
    int numOriginals = 0;

    for(int i = 0; i < m_numberPersons; i++)
    {
        if(m_persons[i]->original())
        {
            numOriginals++;
        }
    }

    return numOriginals;
}

void OrganizationLevelModel::setCdfHolder(MartellSimulationParamsEtc *cdfHolder)
{
    m_cdfHolder = cdfHolder;
}
