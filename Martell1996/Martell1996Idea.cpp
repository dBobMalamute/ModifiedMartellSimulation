#include "Martell1996Idea.h"
#include <QJsonArray>
#include <QQmlEngine>
#include <QFile>
#include <QDir>
#include <QtMath>

Martell1996Attributes::Martell1996Attributes()
{
    m_listName = "Martell et al. 1996 Simulation";
    m_displayName = "Martell et al. 1996 Simulation";
    m_authors << "dBob";
    m_description = "This runs and presents the 1996 Martell Simulation.";
    m_iconPath = "qrc:/Icons/Martell1996Simulation.png";

    m_dataTypesIn << Magic<DistributionDataTypeAttributes>::cast().id();
    m_dataTypesIn << Magic<DistributionDataTypeAttributes>::cast().id();
    m_labelsIn << "Women" << "Men";

    m_dataTypesOut << Magic<Martell1996DataTypeAttributes>::cast().id();
    m_labelsOut << "data";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/Martell1996Idea.qml";
    m_minimizable = true;
    m_startMinimized = false;
    m_minimizedName = "Martell...";
    m_differentNameOnMinimized = true;

    m_externalQMLPaths << ":/QML/Martell1996Controls.qml";
    m_externalQMLLocations.append({true, false, 10,0});

    m_externalQMLPaths << ":/QML/Martell1996Controls2.qml";
    m_externalQMLLocations.append({true, false, 10,230});

    m_gear = true;

    qmlRegisterType(QUrl("qrc:/QML/MartellPyramidLevel.qml"), "com.martell1996.extras", 1, 0, "MartellPyramidLevel");
    qmlRegisterUncreatableType<SimulationMode>("com.martell1996.extras", 1, 0, "SimulationMode",
                                               "Cannot create a simulation mode object. This is an enum");
}

const IdeaAttributes Martell1996Idea::attributes()
{
    return Magic<Martell1996Attributes>::cast();
}

Martell1996Idea::Martell1996Idea() :
    m_animationSpeed(1.0),
    stopAfterCycle(false),
    m_bulkRunNum(20),
    m_mode(SimulationMode::NORMAL)
{
    m_animationTimer = new QTimer();
    m_animationTimer->setInterval(3000* m_animationSpeed);
    connect(m_animationTimer,&QTimer::timeout,this,&Martell1996Idea::nextActionFromTimer);

    m_cdfs = new MartellSimulationParamsEtc();

    for(int i = 0; i < 8; i++)
        m_organizationLevels.append(new OrganizationLevelModel(this));

    m_organizationLevels[0]->setNumberPersons(500);
    m_organizationLevels[1]->setNumberPersons(300);
    m_organizationLevels[2]->setNumberPersons(200);
    m_organizationLevels[3]->setNumberPersons(150);
    m_organizationLevels[4]->setNumberPersons(100);
    m_organizationLevels[5]->setNumberPersons(75);
    m_organizationLevels[6]->setNumberPersons(40);
    m_organizationLevels[7]->setNumberPersons(10);

    m_attritionRate << 0.15 << 0.15 << 0.15 << 0.15
                    << 0.15 << 0.15 << 0.15 << 0.15;

    for(int i = 0; i < 8; i++)
    {
        m_organizationLevels[i]->setNull();
        m_organizationLevels[i]->setCdfHolder(m_cdfs);
    }

    setPromotionCycles(0);

}

void Martell1996Idea::finishSetupQmlContext()
{

    Idea::setContextProperty("Level1", m_organizationLevels[0]);
    Idea::setContextProperty("Level2", m_organizationLevels[1]);
    Idea::setContextProperty("Level3", m_organizationLevels[2]);
    Idea::setContextProperty("Level4", m_organizationLevels[3]);
    Idea::setContextProperty("Level5", m_organizationLevels[4]);
    Idea::setContextProperty("Level6", m_organizationLevels[5]);
    Idea::setContextProperty("Level7", m_organizationLevels[6]);
    Idea::setContextProperty("Level8", m_organizationLevels[7]);
}

void Martell1996Idea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber)
{
    auto distribution = std::dynamic_pointer_cast<DistributionDataType>(dataType);
    if(plugNumber == 1)
        m_maleScore = distribution;
    else
        m_femaleScore = distribution;

    validate();
}

std::shared_ptr<DataType> Martell1996Idea::dataOut(PlugNumber)
{
    return std::dynamic_pointer_cast<DataType>(m_dataOut);
}

QJsonObject Martell1996Idea::save() const
{
    QJsonObject saveJson = Idea::save();
    QJsonArray orgLevelsArray;
    for(int i = 0; i < 8; i++)
    {
        orgLevelsArray.append(m_organizationLevels[i]->save());
    }
    saveJson["orgLvl"] = orgLevelsArray;
    saveJson["s"] = m_animationSpeed;
    saveJson["b"] = m_bulkRunNum;

    return saveJson;
}

void Martell1996Idea::load(const QJsonObject &p)
{
    QJsonValue v = p["orgLvl"];
    if(!v.isUndefined())
    {
        QJsonArray orgLevelsArray = v.toArray();
        m_attritionRate.clear();
        for(int i = 0; i < 8; i++)
        {
            m_organizationLevels[i]->load(orgLevelsArray.at(i).toObject());
            m_attritionRate.append(m_organizationLevels[i]->attritionRate());
        }
        emit attritionRateChanged(m_attritionRate);
    }

    v = p["s"];
    if(!v.isUndefined())
    {
        setAnimationSpeed(v.toDouble());
    }

    v = p["b"];
    if(!v.isUndefined())
    {
        m_bulkRunNum = v.toInt();
        emit bulkRunNumChanged(m_bulkRunNum);
    }
}

QList<QVariant> Martell1996Idea::percentWomen() const
{
    return m_percentWomen;
}

QList<QVariant> Martell1996Idea::meanScore() const
{
    return m_meanScore;
}

int Martell1996Idea::originals() const
{
    return m_originals;
}

int Martell1996Idea::promotionCycles() const
{
    return m_promotionCycles;
}

QList<double> Martell1996Idea::attritionRate() const
{
    return m_attritionRate;
}

double Martell1996Idea::animationSpeed() const
{
    return m_animationSpeed;
}

int Martell1996Idea::bulkRunNum() const
{
    return m_bulkRunNum;
}

SimulationMode::SimulationModeEnum Martell1996Idea::mode() const
{
    return m_mode;
}

void Martell1996Idea::validate()
{
    auto v_maleScore = m_maleScore.lock();
    auto v_femaleScore = m_femaleScore.lock();

    if(v_maleScore && v_femaleScore)
    {
        m_cdfs->prepareMaleCDF(v_maleScore.get()->xValues(),v_maleScore.get()->yValues());
        m_cdfs->prepareFemaleCDF(v_femaleScore.get()->xValues(), v_femaleScore.get()->yValues());



        setStatus(IdeaStatus::Working, "Ready", "This box has the data required to simulate.");
        m_dataOut = std::make_shared<Martell1996DataType>();
        emit newData(0);

        for(int i = 0; i < 8; i++)
            m_organizationLevels[i]->reset();

        calculateDescriptiveStatistics();
        updateStatistics();
    }
    else
    {
        emit hideCohensD();
        emit hideR2();

        setDefaultStatus();
    }
}

void Martell1996Idea::normalStepButtonClicked()
{
    setMode(SimulationMode::NORMAL);
    emit startSpinningGear();

    normalStep();
    updateDisplays();

    emit stopSpinningGear();
}

void Martell1996Idea::normalFinishButtonClicked()
{
    setMode(SimulationMode::NORMAL);
    emit startSpinningGear();

    while(m_originals != 0)
    {
        normalStep();
    }
    updateDisplays();

    //qDebug() << "Number Time series data points: " << m_tickerTapeData.length();
    //exportDataToCSV();

    emit stopSpinningGear();
}

void Martell1996Idea::normalResetButtonClicked()
{
    setMode(SimulationMode::NORMAL);

    reset();
}

void Martell1996Idea::normalStep()
{
    //Attrition.
    for(int i = 0; i < 8; i++)
    {
        m_organizationLevels[i]->removeAtAttritionRate(false);
    }

    //Promote from Within.
    for(int i = 7; i > 0; i--)
    {
        int emptyPos = m_organizationLevels[i]->numEmptyPositions();
        m_organizationLevels[i]->addNewlyPromotedPersons(
                    m_organizationLevels[i-1]->promoteBestNPeople(emptyPos, false), false);
    }

    //fill lowest level new hires.
    m_organizationLevels[0]->fillLevelWithNewHires(false);
    setPromotionCycles(m_promotionCycles + 1);

    updateStatistics();
}

void Martell1996Idea::reset()
{
    setPromotionCycles(0);
    for(int i = 0; i < 8; i++)
        m_organizationLevels[i]->reset();

    updateStatistics();
}

void Martell1996Idea::animationStartButtonClicked()
{
    setMode(SimulationMode::ANIMATE);
    stopAfterCycle = false;
    nextActionFromTimer();
    m_animationTimer->start();
}

void Martell1996Idea::animationStopButtonClicked()
{
    setMode(SimulationMode::ANIMATE);
    stopAfterCycle = true;
}

void Martell1996Idea::animationStepButtonClicked()
{
    setMode(SimulationMode::ANIMATE);
    nextActionFromTimer();
    m_animationTimer->start();
    stopAfterCycle = true;
}

void Martell1996Idea::animationResetButtonClicked()
{
    setMode(SimulationMode::ANIMATE);
    m_animationTimer->stop();
    nextAction = AttritionStart;
    reset();
}

void Martell1996Idea::bulkRun()
{
    startSpinningGear();

    SimulationMode::SimulationModeEnum oldMode = mode();
    setMode(SimulationMode::BULK);

    //Start gear. Clear old data.
    m_bulkRunData.clear();
    m_bulkRunData.reserve(m_bulkRunNum);

    for(int i = 0; i < m_bulkRunNum; i++)
    {
        //Reset for simulation run.
        reset();

        //Run the simulation.
        while(m_originals != 0)
            normalStep();

        updateDisplays();

        QList<double> percentWomen;
        QList<double> meanScore;
        for(int i = 0; i < 8; i++)
        {
            percentWomen.append(m_percentWomen.at(i).toDouble());
            meanScore.append(m_meanScore.at(i).toDouble());
        }
        runData data = runData(percentWomen, meanScore, m_promotionCycles);
        m_bulkRunData.append(data);
    }

    //Export data stop gear.
    m_dataOut = std::make_shared<Martell1996DataType>(m_bulkRunNum, m_bulkRunData);
    emit newData(0);

    setMode(oldMode);
    stopSpinningGear();
}

void Martell1996Idea::attritionStep()
{
    for(int i = 0; i < 8; i++)
    {
        m_organizationLevels[i]->removeAtAttritionRate(true);
    }
}

void Martell1996Idea::promoteFromWithin()
{
    for(int i = 7; i > 0; i--)
    {
        int emptyPos = m_organizationLevels[i]->numEmptyPositions();
        m_organizationLevels[i]->addNewlyPromotedPersons(
                    m_organizationLevels[i-1]->promoteBestNPeople(emptyPos, true), true);
    }
}

void Martell1996Idea::fillLowestLevelNewHires()
{
    m_organizationLevels[0]->fillLevelWithNewHires(true);
    setPromotionCycles(m_promotionCycles + 1);
    updateStatistics();
}

void Martell1996Idea::updateDisplays()
{
    for(int i = 0; i < 8; i++)
    {
        m_organizationLevels.at(i)->updateDisplay();
    }
}

void Martell1996Idea::setToAnimationMode(bool animationMode)
{
    if(animationMode)
        setMode(SimulationMode::ANIMATE);
    else
        setMode(SimulationMode::NORMAL);
}

void Martell1996Idea::nextActionFromTimer()
{
    if(nextAction == AttritionStart)
    {
        attritionStep();
        updateDisplays();
        nextAction = PromoteFromWithinStart;
    }
    else if(nextAction == PromoteFromWithinStart)
    {
        promoteFromWithin();
        nextAction = NewHiresStart;
    }
    else if(nextAction == NewHiresStart)
    {
        updateDisplays();
        fillLowestLevelNewHires();
        nextAction = Pause;
    }
    else
    {
        updateDisplays();
        nextAction = AttritionStart;
        if(stopAfterCycle)
            m_animationTimer->stop();
        if(originals() == 0)
            m_animationTimer->stop();
        emit stopSpinningGear();
    }
}

void Martell1996Idea::setPercentWomen(QList<QVariant> percentWomen)
{
    if (m_percentWomen == percentWomen)
        return;

    m_percentWomen = percentWomen;
    emit percentWomenChanged(m_percentWomen);
}

void Martell1996Idea::setMeanScore(QList<QVariant> meanScore)
{
    if (m_meanScore == meanScore)
        return;

    m_meanScore = meanScore;
    emit meanScoreChanged(m_meanScore);
}

void Martell1996Idea::setOriginals(int originals)
{
    if (m_originals == originals)
        return;

    m_originals = originals;
    emit originalsChanged(m_originals);
}

void Martell1996Idea::setPromotionCycles(int promotionCycles)
{
    if (m_promotionCycles == promotionCycles)
        return;

    m_promotionCycles = promotionCycles;
    emit promotionCyclesChanged(m_promotionCycles);
}

void Martell1996Idea::setAttritionRate(int level, double attritionRate)
{
    if(m_attritionRate[level] == attritionRate)
        return;

    m_organizationLevels[level]->setAttritionRate(attritionRate);
    m_attritionRate[level] = attritionRate;

    emit attritionRateChanged(m_attritionRate);
}

void Martell1996Idea::setAnimationSpeed(double animationSpeed)
{
    if (qFuzzyCompare(m_animationSpeed, animationSpeed))
        return;

    m_animationTimer->setInterval(3000*animationSpeed);
    m_animationSpeed = animationSpeed;
    emit animationSpeedChanged(m_animationSpeed);
}

void Martell1996Idea::setBulkRunNum(int bulkRunNum)
{
    if (m_bulkRunNum == bulkRunNum)
        return;

    m_bulkRunNum = bulkRunNum;
    emit bulkRunNumChanged(m_bulkRunNum);
}

void Martell1996Idea::setMode(SimulationMode::SimulationModeEnum mode)
{
    if (m_mode == mode)
        return;
    m_animationTimer->stop();
    nextAction = AttritionStart;

    m_mode = mode;
    emit modeChanged(m_mode);
}

void Martell1996Idea::updateStatistics()
{
    QList<QVariant> percentWomen;
    percentWomen.reserve(8);
    for(int i = 0; i < 8; i++)
    {
        percentWomen.append(m_organizationLevels[i]->percentWomen());
    }
    setPercentWomen(percentWomen);

    QList<QVariant> averageScore;
    averageScore.reserve(8);
    for(int i = 0; i < 8; i++)
    {
        averageScore.append(m_organizationLevels[i]->averageScore());
    }
    setMeanScore(averageScore);

    int totalOriginals = 0;
    for(int i = 0; i < 8; i++)
    {
        totalOriginals += m_organizationLevels[i]->originalEmployees();
    }
    setOriginals(totalOriginals);

    //m_tickerTapeData.append(MartellTimeStepData(percentWomen,averageScore,totalOriginals,m_promotionCycles));
}

void Martell1996Idea::calculateDescriptiveStatistics()
{
    double maleMean = m_maleScore.lock()->getMean();
    double femaleMean = m_femaleScore.lock()->getMean();
    double maleSD = qSqrt(m_maleScore.lock()->getVariance());
    double femaleSD = qSqrt(m_femaleScore.lock()->getVariance());

    double d = (maleMean - femaleMean) / maleSD;
    emit showCohensD(d);

    if(qFuzzyCompare(maleSD, femaleSD))
    {
        double R2 = 1.0 / (1.0 + 1.0 / (0.25*d*d));
        emit showR2(R2);
    }
    else
    {
        emit hideR2();
    }

}

//void Martell1996Idea::exportDataToCSV()
//{
//    QString path = QDir::currentPath() + QDir::separator() + "export.csv";
//    QFile file(path);
//    if(!file.open(QIODevice::ReadWrite))
//    {
//        qDebug() << "Failed to load";
//    }
//    else
//    {
//        qDebug() << "loaded";
//        for(int i = 0; i < m_tickerTapeData.length(); i++)
//        {
//            file.write(m_tickerTapeData[i].toString().toLatin1());
//            file.write("\r\n");
//        }
//    }
//}



