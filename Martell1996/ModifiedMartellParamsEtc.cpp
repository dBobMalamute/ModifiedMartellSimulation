#include "ModifiedMartellParamsEtc.h"
#include <QtMath>

ModifiedMartellParamsEtc::ModifiedMartellParamsEtc()
{

}

QColor ModifiedMartellParamsEtc::getColorFromIndex(int index)
{
    switch (index)
    {
    case 0:
        return QColor("#80545E");
    case 1:
        return QColor("#379388");
    case 2:
        return QColor("#CEC4AB");
    case 3:
        return QColor("#BDA598");
    case 4:
        return QColor("#5086A9");
    case 5:
        return QColor("#187520");
    case 6:
        return QColor("#a87f22");
    default:
        return QColor("black");
    }
}

int ModifiedMartellParamsEtc::getIndexFromColor(QColor color)
{
    if(color == QColor("#80545E"))
        return 0;
    else if (color == QColor("#379388"))
        return 1;
    else if (color == QColor("#CEC4AB"))
        return 2;
    else if (color == QColor("#BDA598"))
        return 3;
    else if (color == QColor("#5086A9"))
        return 4;
    else if (color == QColor("#187520"))
        return 5;
    else if (color == QColor("#a87f22"))
        return 6;
    return -1;
}

void ModifiedMartellParamsEtc::resetGroups()
{
    m_numGroups = 0.0;
    m_groupPercentages.clear();
    m_cumulativeGroupPercentages.clear();
    m_groupNames.clear();
    m_groupColors.clear();
    m_groupGenders.clear();

    m_groupMeans.clear();
    m_groupVariances.clear();
    m_groupXValues.clear();
    m_groupYValues.clear();
}

void ModifiedMartellParamsEtc::addGroup(std::shared_ptr<ModifiedMartellGroupDataType> group)
{
    m_numGroups++;
    m_groupPercentages.append(group->groupPercentage());
    m_groupNames.append(group->groupName());
    m_groupColors.append(getColorFromIndex(group->groupColor()));
    m_groupGenders.append(group->groupGender());

    m_groupMeans.append(group->getGroupMean());
    m_groupVariances.append(group->getGroupVariance());

    m_groupXValues.append(group->xValues());
    m_groupYValues.append(group->yValues());
}

bool ModifiedMartellParamsEtc::setupCumulativePercentages()
{
    double totalPercentage = 0.0;
    for(int i = 0; i < m_numGroups; i++)
    {
        totalPercentage += m_groupPercentages.at(i);
    }

    bool totalIs100Percent = false;
    if(qFuzzyCompare(totalPercentage, 100.0))
        totalIs100Percent = true;

    double runningCumulative = 0.0;
    for(int i = 0; i < m_numGroups; i++)
    {
        runningCumulative += m_groupPercentages.at(i) * 100.0 / totalPercentage;
        m_cumulativeGroupPercentages.append(runningCumulative);
    }

    return totalIs100Percent;
}

void ModifiedMartellParamsEtc::setupScoreCDFs()
{
    m_groupCDFs.clear();

    for(int i = 0; i < m_numGroups; i++)
    {
        m_groupCDFs.append(std::vector<double>());
        m_groupCDFs[i].reserve(m_groupXValues[i].size());
        double runningSum = 0.0;
        m_groupCDFs[i].push_back(runningSum);

        for(size_t j = 0; j < m_groupXValues[i].size(); j++)
        {
            runningSum += 0.5 * (m_groupYValues[i][j+1] +m_groupYValues[i][j]) *
                    (m_groupXValues[i][j+1] - m_groupXValues[i][j]);
            m_groupCDFs[i].push_back(runningSum);
        }

        //Normalize. This might be necessary.
        double norm = m_groupCDFs[i][m_groupCDFs[i].size() - 1];
        for(size_t j = 0; j < m_groupCDFs[i].size(); j++)
        {
            m_groupCDFs[i][j] /= norm;
        }
    }
}

int ModifiedMartellParamsEtc::getNumGroups() const
{
    return m_numGroups;
}

QList<double> ModifiedMartellParamsEtc::getPieChartSweeps()
{
    QList<double> sweeps;
    sweeps.reserve(m_numGroups);
    for(int i = 0; i < m_numGroups; i++)
        sweeps.append(3.600 * m_cumulativeGroupPercentages.at(i)); // 100 percent, 360 degrees. Conversion is 3.6;

    return sweeps;
}

QList<QColor> ModifiedMartellParamsEtc::getGroupColors() const
{
    return m_groupColors;
}

QList<QString> ModifiedMartellParamsEtc::getGroupNames() const
{
    return m_groupNames;
}

SimulationGender::GENDER ModifiedMartellParamsEtc::getGender(int i)
{
    return m_groupGenders.at(i);
}

ModifiedMartellPerson *ModifiedMartellParamsEtc::generatePerson(bool original)
{
    //Random identity from the samples.
    double rand = QRandomGenerator::global()->bounded(100.0);
    int i = 0;
    while(m_cumulativeGroupPercentages.at(i) < rand)
        i++;

    return new ModifiedMartellPerson(m_groupColors.at(i), sampleScoreCDF(i), original, m_groupGenders.at(i));
}

bool ModifiedMartellParamsEtc::calculateCohensD(double &d)
{
    bool hasMen = false; bool hasWomen = false;
    for(int i = 0; i < m_numGroups; i++)
    {
        if(m_groupGenders.at(i) == SimulationGender::UNSPECIFIED)
            return false;
        else if(m_groupGenders.at(i) == SimulationGender::MEN)
            hasMen = true;
        else
            hasWomen = true;
    }

    if(!hasMen || !hasWomen)
        return false;

    double totalMaleEx = 0.0; double totalMaleFraction = 0.0; double totalMaleEx2 = 0.0;
    double totalFemaleEx = 0.0; double totalFemaleFraction = 0.0; double totalFemaleEx2 = 0.0;

    for(int i = 0; i < m_numGroups; i++)
    {
        double frac = m_groupPercentages.at(i) / 100;
        if(m_groupGenders.at(i) == SimulationGender::MEN)
        {
            totalMaleEx += frac * m_groupMeans.at(i) ;
            totalMaleEx2 += frac * (m_groupVariances.at(i) + m_groupMeans.at(i) * m_groupMeans.at(i));
            totalMaleFraction += frac;
        }
        else
        {
            totalFemaleEx += frac * m_groupMeans.at(i);
            totalFemaleEx2 += frac * (m_groupVariances.at(i) + m_groupMeans.at(i) * m_groupMeans.at(i));
            totalFemaleFraction += frac;
        }
    }

    totalMaleEx /= totalMaleFraction;
    totalMaleEx2 /= totalMaleFraction;
    totalFemaleEx /= totalFemaleFraction;
    totalFemaleEx2 /= totalFemaleFraction;

    double maleVariance = totalMaleEx2 - totalMaleEx * totalMaleEx;
    double femaleVariance = totalFemaleEx2 - totalFemaleEx * totalFemaleEx;

    double pooledStdev = qSqrt((totalMaleFraction * maleVariance + totalFemaleFraction * femaleVariance));
    d = (totalMaleEx - totalFemaleEx) / pooledStdev;
    return true;
}

double ModifiedMartellParamsEtc::sampleScoreCDF(int i)
{
    double random = QRandomGenerator::global()->generateDouble();
    auto lower = std::lower_bound(m_groupCDFs[i].begin(), m_groupCDFs[i].end(), random);

    int index = std::distance(m_groupCDFs[i].begin(), lower);

    return m_groupXValues[i][index-1] + (m_groupXValues[i][index] - m_groupXValues[i][index-1]) *
            (random - m_groupCDFs[i][index - 1]) / (m_groupCDFs[i][index] - m_groupCDFs[i][index - 1]);
}

