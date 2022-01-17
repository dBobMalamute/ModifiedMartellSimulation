#ifndef AREAUNDERDISTRIBUTIONIDEA_H
#define AREAUNDERDISTRIBUTIONIDEA_H

#include <QPointF>
#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"
#include "../DataTypes/DistributionDataType.h"

#include "DistributionDisplay.h"

struct AreaUnderDistributionAttributes : public IdeaAttributes
{
    AreaUnderDistributionAttributes();
};

class AreaUnderDistributionIdea : public Idea
{
    Q_OBJECT

    Q_PROPERTY(double lowLimit READ lowLimit WRITE setLowLimit NOTIFY lowLimitChanged)
    Q_PROPERTY(double upLimit READ upLimit WRITE setUpLimit NOTIFY upLimitChanged)
    Q_PROPERTY(bool lowLimitEnabled READ lowLimitEnabled WRITE setLowLimitEnabled NOTIFY lowLimitEnabledChanged)
    Q_PROPERTY(bool upLimitEnabled READ upLimitEnabled WRITE setUpLimitEnabled NOTIFY upLimitEnabledChanged)

    Q_PROPERTY(double areaShown READ areaShown WRITE setAreaShown NOTIFY areaShownChanged)

public:
    explicit AreaUnderDistributionIdea();
    const IdeaAttributes attributes() override;

    void finishSetupQmlContext() override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber plugNumber) override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    void resetDisplay() override;

    double lowLimit() const;
    double upLimit() const;
    bool lowLimitEnabled() const;
    bool upLimitEnabled() const;    
    double areaShown() const;

private:
    std::weak_ptr<DistributionDataType> m_distribution;
    std::shared_ptr<NumberDataType> m_output;

    void validate();
    void compute();
    void computeContinuous();
    void computeDiscrete();

signals:
    void lowLimitChanged(double lowLimit);
    void upLimitChanged(double upLimit);
    void lowLimitEnabledChanged(bool lowLimitEnabled);
    void upLimitEnabledChanged(bool upLimitEnabled);
    void areaShownChanged(double areaShown);

    void displayAreaContinuous(std::vector<double> xValues, std::vector<double> yValues);

public slots:
    // xlow, xHigh, yLow, yHigh;
    void setBound(double value, int index);
    void setAutoFit(bool autoFit);

    void setLowLimit(double lowLimit);
    void setUpLimit(double upLimit);
    void setLowLimitEnabled(bool lowLimitEnabled);
    void setUpLimitEnabled(bool upLimitEnabled);
    void setAreaShown(double areaShown);

private:
    DistributionDisplay* m_display;

    double m_lowLimit;
    double m_upLimit;
    bool m_lowLimitEnabled;
    bool m_upLimitEnabled;
    double m_areaShown;

    std::vector<double> m_xValues;
    std::vector<double> m_yValues;
    bool m_distributionContinuous;
};

#endif // AREAUNDERDISTRIBUTIONIDEA_H
