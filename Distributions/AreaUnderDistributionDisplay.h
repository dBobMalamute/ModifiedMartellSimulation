#ifndef AREAUNDERDISTRIBUTIONDISPLAY_H
#define AREAUNDERDISTRIBUTIONDISPLAY_H

#include <QObject>
#include <QJsonObject>
#include "../DataTypes/DistributionDataType.h"

class AreaUnderDistributionDisplay : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double xLow READ xLow WRITE setXLow NOTIFY xLowChanged)
    Q_PROPERTY(double xHigh READ xHigh WRITE setXHigh NOTIFY xHighChanged)
    Q_PROPERTY(double yLow READ yLow WRITE setYLow NOTIFY yLowChanged)
    Q_PROPERTY(double yHigh READ yHigh WRITE setYHigh NOTIFY yHighChanged)

    Q_PROPERTY(double areaLowLimit READ areaLowLimit WRITE setAreaLowLimit NOTIFY areaLowLimitChanged)
    Q_PROPERTY(double areaHighLimit READ areaHighLimit WRITE setAreaHighLimit NOTIFY areaHighLimitChanged)

    Q_PROPERTY(bool autoFit READ autoFit NOTIFY autoFitChanged)

public:
    explicit AreaUnderDistributionDisplay(QObject *parent = nullptr);

    bool setBound(double value, int index);
    bool setAutoFit(bool autoFit);

    double xLow() const;
    double xHigh() const;
    double yLow() const;
    double yHigh() const;
    double areaLowLimit() const;
    double areaHighLimit() const;
    bool autoFit() const;

    QJsonObject save() const;
    void load(const QJsonObject &);

    void setXValues(const std::vector<double> &xValues);
    void setYValues(const std::vector<double> &yValues);
    void setDistType(const DistributionDataType::DistributionType &distType);
    void clearData();

    void plot();

signals:
    void xLowChanged(double xLow);
    void xHighChanged(double xHigh);
    void yLowChanged(double yLow);
    void yHighChanged(double yHigh);

    void areaLowLimitChanged(double areaLowLimit);
    void areaHighLimitChanged(double areaHighLimit);

    void autoFitChanged(bool autoFit);

    void displayContinuousDistribution(std::vector<double> xValues, std::vector<double> yValues);
    void displayDiscreteDistribution(std::vector<double> xValues, std::vector<double> yValues);
    void showAreaContinuousDistribution(std::vector<double> xValues, std::vector<double> yValues);

public slots:

    void setXLow(double xLow);
    void setXHigh(double xHigh);
    void setYLow(double yLow);
    void setYHigh(double yHigh);

    void setAreaLowLimit(double areaLowLimit);
    void setAreaHighLimit(double areaHighLimit);

private:
    double m_xLow;
    double m_xHigh;
    double m_yLow;
    double m_yHigh;

    double m_areaLowLimit;
    double m_areaHighLimit;

    bool m_autoFit;

    void fit();
    void plotContinuous();
    void plotDiscrete();

    std::vector<double> m_xValues;
    std::vector<double> m_yValues;
    DistributionDataType::DistributionType m_distType;


};

#endif // AREAUNDERDISTRIBUTIONDISPLAY_H
