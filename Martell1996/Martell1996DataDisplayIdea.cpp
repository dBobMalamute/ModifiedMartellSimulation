#include "Martell1996DataDisplayIdea.h"

Martell1996DataDisplayAttributes::Martell1996DataDisplayAttributes()
{
    m_listName = "Martell 1996 Data Display";
    m_displayName = "Martell 1996 Data Display";
    m_authors << "dBob";
    m_description = "Handles data exporting for Martell et al 1996";
    m_iconPath = "qrc:/Icons/Martell1996DataDisplay.png";

    m_dataTypesIn << Magic<Martell1996DataTypeAttributes>::cast().id();

    m_embeddedQMLPath = ":/QML/Martell1996DataDisplay.qml";
}

const IdeaAttributes Martell1996DataDisplayIdea::attributes()
{
    return Magic<Martell1996DataDisplayAttributes>::cast();
}

Martell1996DataDisplayIdea::Martell1996DataDisplayIdea()
{
    m_model = new Martell1996SummaryTableModel(this);
}

void Martell1996DataDisplayIdea::finishSetupQmlContext()
{
    setContextProperty("dataModel", m_model);
}

void Martell1996DataDisplayIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    m_input = std::dynamic_pointer_cast<Martell1996DataType>(dataType);

    validate();
}

void Martell1996DataDisplayIdea::validate()
{
    auto n = m_input.lock();

    m_model->clearData();

    if(n)
    {
        if(n->getNumRuns() == 0)
        {
            setStatus(IdeaStatus::StandBy, "Awaiting Data", "This box is currently not recieving any data. Run"
                                                            " the Martell et al 1996 simultaion in bulk mode to"
                                                            " send data to this box.");
        }
        else
        {
            setStatus(IdeaStatus::Working, "Holding data", "Ready to export/display data");
            prepareData();
        }
    }
    else
    {
        setDefaultStatus();
    }
}

void Martell1996DataDisplayIdea::prepareData()
{
    auto n = m_input.lock();

    //Get the data from the dataType;
    QList<runData> runsData = n->getRunData();

    //Setup initial containers to contain 0's
    QList<double> averageScore;
    QList<double> averagePercentWomen;
    averagePercentWomen.reserve(8);
    averageScore.reserve(8);
    for(int i = 0; i < 8; i++)
    {
        averageScore.append(0);
        averagePercentWomen.append(0);
    }

    //Calculate the average for each level.
    for(int i = 0; i < runsData.length(); i++)
    {
        for(int j = 0; j < 8; j++)
        {
            averageScore[j] += runsData[i].averageScore()[j];
            averagePercentWomen[j] += runsData[i].percentWomen()[j];
        }
    }
    for(int i = 0; i < 8; i++)
    {
        averageScore[i] /= runsData.length();
        averagePercentWomen[i] /= runsData.length();
    }

    m_model->addLevel(8,averageScore[7],10,averagePercentWomen[7]);
    m_model->addLevel(7,averageScore[6],40,averagePercentWomen[6]);
    m_model->addLevel(6,averageScore[5],75,averagePercentWomen[5]);
    m_model->addLevel(5,averageScore[4],100,averagePercentWomen[4]);
    m_model->addLevel(4,averageScore[3],150,averagePercentWomen[3]);
    m_model->addLevel(3,averageScore[2],200,averagePercentWomen[2]);
    m_model->addLevel(2,averageScore[1],300,averagePercentWomen[1]);
    m_model->addLevel(1,averageScore[0],500,averagePercentWomen[0]);

    //Promotion Cycles
    double averagePromotionCycles = 0.0;
    for(int i = 0; i < runsData.length(); i++)
    {
        averagePromotionCycles += runsData[i].promotionCycles();
    }
    averagePromotionCycles /= runsData.length();
    emit displayAveragePromotionCycles(averagePromotionCycles);
}


