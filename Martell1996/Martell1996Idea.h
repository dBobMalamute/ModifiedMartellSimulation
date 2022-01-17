#ifndef MARTELL1996IDEA_H
#define MARTELL1996IDEA_H

#include <QPointF>
#include <QTimer>
#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/DistributionDataType.h"
#include "Martell1996DataType.h"
#include "MartellTimeStepData.h"

#include "Person.h"
#include "OrganizationLevelModel.h"
#include "MartellSimulationParamsEtc.h"
#include "SimulationMode.h"

struct Martell1996Attributes : public IdeaAttributes
{
    Martell1996Attributes();
};

class Martell1996Idea : public Idea
{
    Q_OBJECT

    Q_PROPERTY(QList<QVariant> percentWomen READ percentWomen WRITE setPercentWomen NOTIFY percentWomenChanged)
    Q_PROPERTY(QList<QVariant> meanScore READ meanScore WRITE setMeanScore NOTIFY meanScoreChanged)

    Q_PROPERTY(int originals READ originals WRITE setOriginals NOTIFY originalsChanged)
    Q_PROPERTY(int promotionCycles READ promotionCycles WRITE setPromotionCycles NOTIFY promotionCyclesChanged)
    Q_PROPERTY(QList<double> attritionRate READ attritionRate NOTIFY attritionRateChanged)

    Q_PROPERTY(SimulationMode::SimulationModeEnum mode READ mode WRITE setMode NOTIFY modeChanged)

    Q_PROPERTY(double animationSpeed READ animationSpeed WRITE setAnimationSpeed NOTIFY animationSpeedChanged)
    Q_PROPERTY(int bulkRunNum READ bulkRunNum WRITE setBulkRunNum NOTIFY bulkRunNumChanged)

    enum NextAction
    {
        AttritionStart,
        PromoteFromWithinStart,
        NewHiresStart,
        Pause
    };

public:
    explicit Martell1996Idea();
    const IdeaAttributes attributes() override;

    void finishSetupQmlContext() override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    QList<QVariant> percentWomen() const;
    QList<QVariant> meanScore() const;

    int originals() const;  
    int promotionCycles() const;

    QList<double> attritionRate() const;

    double animationSpeed() const;

    int bulkRunNum() const;

    SimulationMode::SimulationModeEnum mode() const;

private:
    std::weak_ptr<DistributionDataType> m_maleScore;
    std::weak_ptr<DistributionDataType> m_femaleScore;

    std::shared_ptr<Martell1996DataType> m_dataOut;

    void validate();

signals:
    void percentWomenChanged(QList<QVariant> percentWomen);
    void meanScoreChanged(QList<QVariant> meanScore);

    void originalsChanged(int originals);
    void promotionCyclesChanged(int promotionCycles);
    void attritionRateChanged(QList<double> attritionRate);

    void modeChanged(SimulationMode::SimulationModeEnum mode);

    void animationSpeedChanged(double animationSpeed);
    void bulkRunNumChanged(int bulkRunNum);

    void showCohensD(double d);
    void showR2(double R2);
    void hideCohensD();
    void hideR2();


public slots:
    void normalStepButtonClicked();
    void normalFinishButtonClicked();
    void normalResetButtonClicked();

    void animationStartButtonClicked();
    void animationStopButtonClicked();
    void animationStepButtonClicked();
    void animationResetButtonClicked();

    void bulkRun();

    void attritionStep();
    void promoteFromWithin();
    void fillLowestLevelNewHires();
    void updateDisplays();

    //void bulkRunButtonClicked();
    void setToAnimationMode(bool animationMode);

    void nextActionFromTimer();

    void setPercentWomen(QList<QVariant> percentWomen);
    void setMeanScore(QList<QVariant> meanScore);   
    void setOriginals(int originals);   
    void setPromotionCycles(int promotionCycles);
    void setAttritionRate(int level, double attritionRate);

    void setMode(SimulationMode::SimulationModeEnum mode);

    void setAnimationSpeed(double animationSpeed);
    void setBulkRunNum(int bulkRunNum);


private:
    QList<OrganizationLevelModel*> m_organizationLevels;

    void normalStep();
    void reset();
    void updateStatistics();
    void calculateDescriptiveStatistics();

    MartellSimulationParamsEtc* m_cdfs;

    QList<QVariant> m_percentWomen;
    QList<QVariant> m_meanScore;

    int m_originals;
    int m_promotionCycles;

    //QList<MartellTimeStepData> m_tickerTapeData;
    //void exportDataToCSV();

    QList<double> m_attritionRate;

    double m_animationSpeed;

    QTimer* m_animationTimer;
    NextAction nextAction = AttritionStart;
    bool stopAfterCycle;

    int m_bulkRunNum;
    QList<runData> m_bulkRunData;
    SimulationMode::SimulationModeEnum m_mode;
};

#endif // MARTELL1996IDEA_H
