#ifndef VALUEATDISTRIBUTIONPOINTIDEA_H
#define VALUEATDISTRIBUTIONPOINTIDEA_H

#include <QPointF>
#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumberDataType.h"
#include "../DataTypes/DistributionDataType.h"

#include "DistributionDisplay.h"

struct ValueAtDistributionPointAttributes : public IdeaAttributes
{
    ValueAtDistributionPointAttributes();
};

class ValueAtDistributionPointIdea : public Idea
{
    Q_OBJECT

    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(double point READ point WRITE setPoint NOTIFY pointChanged)

public:
    explicit ValueAtDistributionPointIdea();
    const IdeaAttributes attributes() override;

    void finishSetupQmlContext() override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;
    std::shared_ptr<DataType> dataOut(PlugNumber) override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    void resetDisplay() override;

    double value() const;
    double point() const;

private:
    std::weak_ptr<DistributionDataType> m_distribution;
    std::shared_ptr<NumberDataType> m_output;

    void validate();
    void compute();
    void computeContinuous();
    void computeDiscrete();

signals:
    void valueChanged(double value);
    void pointChanged(double point);

public slots:
    // xlow, xHigh, yLow, yHigh;
    void setBound(double value, int index);
    void setAutoFit(bool autoFit);

    void setValue(double value);
    void setPoint(double point);

private:
    DistributionDisplay* m_display;
    double m_value;
    double m_point;

    std::vector<double> m_xValues;
    std::vector<double> m_yValues;
    DistributionDataType::DistributionType m_distributionType;
};

#endif // VALUEATDISTRIBUTIONPOINTIDEA_H
