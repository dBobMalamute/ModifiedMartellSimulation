#include "ModifiedMartellGroupDataType.h"

ModifiedMartellGroupDataTypeAttributes::ModifiedMartellGroupDataTypeAttributes()
{
    m_name = "Modified Martell Group Data";
    m_color = "#095bb5";
    m_abbreviation = "data";
    m_description = "This data type holds the group data for the modified simulation";
    m_authors << "dBob";

    m_defaultSource = "Modified Martell Group";
    m_defaultDisplay = "Modified Martell Simulation";
}

ModifiedMartellGroupDataType::ModifiedMartellGroupDataType(QString groupName, int groupColor,
                                                           SimulationGender::GENDER groupGender, double groupPercentage,
                                                           std::vector<double> xValues, std::vector<double> yValues, double mean, double variance):
    m_xValues(xValues),
    m_yValues(yValues),
    m_groupName(groupName),
    m_groupColor(groupColor),
    m_groupGender(groupGender),
    m_groupPercentage(groupPercentage),
    m_groupMean(mean),
    m_groupVariance(variance)
{

}

QString ModifiedMartellGroupDataType::groupName() const
{
    return m_groupName;
}

int ModifiedMartellGroupDataType::groupColor() const
{
    return m_groupColor;
}

SimulationGender::GENDER ModifiedMartellGroupDataType::groupGender() const
{
    return m_groupGender;
}

double ModifiedMartellGroupDataType::groupPercentage() const
{
    return m_groupPercentage;
}

std::vector<double> ModifiedMartellGroupDataType::xValues() const
{
    return m_xValues;
}

std::vector<double> ModifiedMartellGroupDataType::yValues() const
{
    return m_yValues;
}

double ModifiedMartellGroupDataType::getGroupMean() const
{
    return m_groupMean;
}

double ModifiedMartellGroupDataType::getGroupVariance() const
{
    return m_groupVariance;
}

DataTypeAttributes ModifiedMartellGroupDataType::getAttributes()
{
    return Magic<ModifiedMartellGroupDataTypeAttributes>::cast();
}
