#ifndef MODIFIEDMARTELLGROUPDATATYPE_H
#define MODIFIEDMARTELLGROUPDATATYPE_H

#include "../MalamuteCore/InheritedClasses/DataType.h"
#include "SimulationGender.h"

struct ModifiedMartellGroupDataTypeAttributes : public DataTypeAttributes
{
    ModifiedMartellGroupDataTypeAttributes();
};

class ModifiedMartellGroupDataType : public DataType
{
public:
    ModifiedMartellGroupDataType(QString groupName, int groupColor, SimulationGender::GENDER groupGender,
                                 double groupPercentage, std::vector<double> xValues,
                                 std::vector<double> yValues,
                                 double mean, double variance);

    DataTypeAttributes getAttributes() override;

    QString groupName() const;
    int groupColor() const;
    SimulationGender::GENDER groupGender() const;
    double groupPercentage() const;

    std::vector<double> xValues() const;
    std::vector<double> yValues() const;

    double getGroupMean() const;
    double getGroupVariance() const;

private:
    std::vector<double> m_xValues;
    std::vector<double> m_yValues;

    QString m_groupName;
    int m_groupColor;
    SimulationGender::GENDER m_groupGender;
    double m_groupPercentage;

    double m_groupMean;
    double m_groupVariance;
};

#endif // MODIFIEDMARTELLGROUPDATATYPE_H
