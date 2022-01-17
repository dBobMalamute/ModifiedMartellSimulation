#ifndef MODIFIEDMARTELLDATADISPLAYIDEA_H
#define MODIFIEDMARTELLDATADISPLAYIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "ModifiedMartellDataType.h"

#include "Martell1996SummaryTableModel.h"

struct ModifiedMartellDataDisplayAttributes : public IdeaAttributes
{
    ModifiedMartellDataDisplayAttributes();
};

class ModifiedMartellDataDisplayIdea : public Idea
{
    Q_OBJECT
public:
    ModifiedMartellDataDisplayIdea();
    const IdeaAttributes attributes() override;

    void finishSetupQmlContext() override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;

signals:
    void displayAveragePromotionCycles(double cycles);

private:
    std::weak_ptr<ModifiedMartellDataType> m_input;

    void validate();
    void prepareData();

    Martell1996SummaryTableModel* m_model;
};

#endif // MODIFIEDMARTELLDATADISPLAYIDEA_H
