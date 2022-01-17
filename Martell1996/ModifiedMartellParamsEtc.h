#ifndef MODIFIEDMARTELLPARAMSETC_H
#define MODIFIEDMARTELLPARAMSETC_H

#include <QRandomGenerator>
#include <QList>
#include <QColor>
#include "ModifiedMartellGroupDataType.h"
#include "ModifiedMartellPerson.h"

#include <memory>

class ModifiedMartellGroupDataType;

class ModifiedMartellParamsEtc
{
public:
    ModifiedMartellParamsEtc();

    static QColor getColorFromIndex(int index);
    static int getIndexFromColor(QColor color);

    void resetGroups();
    void addGroup(std::shared_ptr<ModifiedMartellGroupDataType> group);
    bool setupCumulativePercentages();
    void setupScoreCDFs();

    int getNumGroups() const;
    QList<double> getPieChartSweeps();
    QList<QColor> getGroupColors() const;
    QList<QString> getGroupNames() const;
    SimulationGender::GENDER getGender(int i);

    ModifiedMartellPerson* generatePerson(bool original = false);

    bool calculateCohensD(double &d);

private:
    int m_numGroups;
    QList<double> m_groupPercentages;
    QList<double> m_cumulativeGroupPercentages;
    QList<QString> m_groupNames;
    QList<QColor> m_groupColors;
    QList<SimulationGender::GENDER> m_groupGenders;

    QList<double> m_groupMeans;
    QList<double> m_groupVariances;

    QList<std::vector<double>> m_groupXValues;
    QList<std::vector<double>> m_groupYValues;
    QList<std::vector<double>> m_groupCDFs;
    double sampleScoreCDF(int i);
};

#endif // MODIFIEDMARTELLPARAMSETC_H
