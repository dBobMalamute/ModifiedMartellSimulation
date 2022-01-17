#ifndef MARTELL1996DATADISPLAYIDEA_H
#define MARTELL1996DATADISPLAYIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "Martell1996DataType.h"

#include "Martell1996SummaryTableModel.h"

struct Martell1996DataDisplayAttributes : public IdeaAttributes
{
    Martell1996DataDisplayAttributes();
};

class Martell1996DataDisplayIdea : public Idea
{
    Q_OBJECT
public:
    Martell1996DataDisplayIdea();
    const IdeaAttributes attributes() override;

    void finishSetupQmlContext() override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;

signals:
    void displayAveragePromotionCycles(double cycles);

private:
    std::weak_ptr<Martell1996DataType> m_input;

    void validate();
    void prepareData();

    Martell1996SummaryTableModel* m_model;
};

#endif // MARTELL1996DATADISPLAYIDEA_H
