#ifndef MODIFIEDMARTELLDATATYPE_H
#define MODIFIEDMARTELLDATATYPE_H

#include "../MalamuteCore/InheritedClasses/DataType.h"
#include "Martell1996DataType.h"

struct ModifiedMartellDataTypeAttributes : public DataTypeAttributes
{
    ModifiedMartellDataTypeAttributes();
};

class ModifiedMartellDataType : public DataType
{
public:
    explicit ModifiedMartellDataType();
    explicit ModifiedMartellDataType(int numRuns, QList<int> positionsAtLevel, QList<runData> m_runData);

    DataTypeAttributes getAttributes() override;

    QList<runData> getRunData() const;
    int getNumRuns() const;
    QList<int> getPositionsAtLevel() const;

private:
    int m_numRuns;
    QList<int> m_positionsAtLevel;
    QList<runData> m_runData;
};

#endif // MODIFIEDMARTELLDATATYPE_H
