#include "AreaUnderDistributionIdea.h"
#include <QJsonArray>

#include "DistributionCommonFunctions.h"

AreaUnderDistributionAttributes::AreaUnderDistributionAttributes()
{
    m_listName = "Area Under Distribution";
    m_displayName = "Area Under Distribution";
    m_authors << "dBob";
    m_description = "This box calculates the area under a distribution.";
    m_iconPath = "qrc:/Icons/AreaUnderDistribution.png";

    m_dataTypesIn << Magic<DistributionDataTypeAttributes>::cast().id();

    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_minimizable = true;
    m_minimizedName = "Area";
    m_differentNameOnMinimized = true;
    m_minimizedQMLPath = ":/QML/AreaUnderDistributionMinimized.qml";
    m_embeddedQMLPath = ":/QML/AreaUnderDistributionIdea.qml";

    m_gear = true;
}

const IdeaAttributes AreaUnderDistributionIdea::attributes()
{
    return Magic<AreaUnderDistributionAttributes>::cast();
}

AreaUnderDistributionIdea::AreaUnderDistributionIdea() :
    m_lowLimit(-1.0),
    m_upLimit(1.0),
    m_lowLimitEnabled(false),
    m_upLimitEnabled(false),
    m_areaShown(0.0)
{

}

void AreaUnderDistributionIdea::finishSetupQmlContext()
{
    m_display = new DistributionDisplay(this);
    setContextProperty("distributionDisplay", m_display);
}

void AreaUnderDistributionIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto distribution = std::dynamic_pointer_cast<DistributionDataType>(dataType);
    m_distribution = distribution;

    validate();
}

QJsonObject AreaUnderDistributionIdea::save() const
{
    QJsonObject saveJson = Idea::save();

    saveJson["d"] = m_display->save();
    saveJson["l"] = m_lowLimit;
    saveJson["le"] = m_lowLimitEnabled;
    saveJson["u"] = m_upLimit;
    saveJson["ue"] = m_upLimitEnabled;

    return saveJson;
}

void AreaUnderDistributionIdea::load(const QJsonObject &obj)
{
    QJsonValue v = obj["d"];
    if(!v.isUndefined())
    {
        m_display->load(v.toObject());
    }

    v = obj["l"];
    if(!v.isUndefined())
    {
        m_lowLimit = v.toDouble();
        emit lowLimitChanged(m_lowLimit);
    }
    v = obj["le"];
    if(!v.isUndefined())
    {
        m_lowLimitEnabled = v.toBool();
        emit lowLimitEnabledChanged(m_lowLimitEnabled);
    }

    v = obj["u"];
    if(!v.isUndefined())
    {
        m_upLimit = v.toDouble();
        emit upLimitChanged(m_upLimit);
    }
    v = obj["ue"];
    if(!v.isUndefined())
    {
        m_upLimitEnabled = v.toBool();
        emit upLimitEnabledChanged(m_upLimitEnabled);
    }

    if(valid())
        compute();
}

double AreaUnderDistributionIdea::lowLimit() const
{
    return m_lowLimit;
}

double AreaUnderDistributionIdea::upLimit() const
{
    return m_upLimit;
}

bool AreaUnderDistributionIdea::lowLimitEnabled() const
{
    return m_lowLimitEnabled;
}

bool AreaUnderDistributionIdea::upLimitEnabled() const
{
    return m_upLimitEnabled;
}

double AreaUnderDistributionIdea::areaShown() const
{
    return m_areaShown;
}

void AreaUnderDistributionIdea::validate()
{
    auto v_distribution = m_distribution.lock();
    if(v_distribution)
    {
        setStatus(IdeaStatus::Working, "Plotting", "This idea box is currently plotting the input distribution.");
        m_display->setXValues(v_distribution->xValues());
        m_display->setYValues(v_distribution->yValues());
        m_display->setDistType(v_distribution->distType());

        m_xValues = v_distribution->xValues();
        m_yValues = v_distribution->yValues();
        m_distributionContinuous = (v_distribution->distType() == DistributionDataType::Continuous);

        m_display->plot();
        compute();
    }
    else
    {
        setDefaultStatus();
        m_display->clearData();
        setAreaShown(0.0);
        emit displayAreaContinuous(std::vector<double>(), std::vector<double>());
    }
}

void AreaUnderDistributionIdea::compute()
{
    if(!valid())
        return;

    if(m_distributionContinuous)
        computeContinuous();
    else
        computeDiscrete();
}

void AreaUnderDistributionIdea::computeContinuous()
{
    emit startSpinningGear();

    double xLength = m_display->xHigh() - m_display->xLow();
    double yLength = m_display->yHigh() - m_display->yLow();

    std::vector<double> xPos;
    std::vector<double> yPos;
    xPos.reserve(m_xValues.size() + 3);
    yPos.reserve(m_xValues.size() + 3);

    size_t startIndex = 0;
    size_t endIndex = m_xValues.size();
    QPointF initialPoint;
    QList<QPointF> finalPoints;

    double area = 0.0;

    bool StartIsEndEdgeCase = false;

    //Handle the lower limit.
    if(m_lowLimitEnabled)
    {
        auto lower = std::lower_bound(m_xValues.begin(), m_xValues.end(), m_lowLimit);
        startIndex = std::distance(m_xValues.begin(), lower);
        if(startIndex == 0)
        {
            //Lower bound is below the distribution
            xPos.push_back(450.0 * (m_xValues[0] - m_display->xLow()) / xLength);
            yPos.push_back(301.0);
            initialPoint = QPointF(450.0 * (m_xValues[0] - m_display->xLow()) / xLength, 301.0);
        }
        else if (startIndex >= m_xValues.size())
        {
            //Lower bound is above the distribution.
            m_output = std::make_shared<NumberDataType>(0);
            setAreaShown(0.0);
            emit newData(0);
            emit displayAreaContinuous(std::vector<double>(), std::vector<double>());
            return;
        }
        else
        {
            //Add in an extrapolated point.
            double interLength = m_xValues[startIndex] - m_xValues[startIndex - 1];
            double intermediateY = m_yValues[startIndex] * (m_lowLimit - m_xValues[startIndex - 1]) / interLength;
            intermediateY += m_yValues[startIndex - 1] * (m_xValues[startIndex] - m_lowLimit) / interLength;

            double interpX = 450.0 * (m_lowLimit - m_display->xLow()) / xLength;
            double interpY = 300.0 - (300.0 * (intermediateY - m_display->yLow()) / yLength);
            xPos.push_back(interpX);
            yPos.push_back(interpY);
            initialPoint = QPoint(interpX, 300);

            area += 0.5 * (m_xValues[startIndex] - m_lowLimit) * (intermediateY + m_yValues[startIndex]);
        }
    }
    else
    {
        //Lower limit is negative infinity.
        xPos.push_back(450.0 * (m_xValues[0] - m_display->xLow()) / xLength);
        yPos.push_back(301.0);
        initialPoint = QPointF(450.0 * (m_xValues[0] - m_display->xLow()) / xLength, 301.0);
    }

    //Handle the upper limit.
    if(m_upLimitEnabled)
    {
        auto upper = std::lower_bound(m_xValues.begin(), m_xValues.end(), m_upLimit);
        endIndex = std::distance(m_xValues.begin(), upper);
        if(endIndex == 0)
        {
            // Upper bound is below distribution
            m_output = std::make_shared<NumberDataType>(0.0);
            emit newData(0);
            setAreaShown(0.0);
            emit displayAreaContinuous(std::vector<double>(), std::vector<double>());
            return;
        }
        else if(endIndex >= m_xValues.size())
        {
            finalPoints.append(QPointF(450.0 * (m_xValues[m_xValues.size() -1] - m_display->xLow()) / xLength,
                               301.0));
        }
        else
        {
            //Add in an extra interpolated point.
            double interLength = m_xValues[endIndex] - m_xValues[endIndex - 1];
            double intermediateY = m_yValues[endIndex] * (m_upLimit - m_xValues[endIndex - 1]) / interLength;
            intermediateY += m_yValues[endIndex - 1] * (m_xValues[endIndex] - m_upLimit) / interLength;

            double interpX = 450.0 * (m_upLimit - m_display->xLow()) / xLength;
            double interpY = 300.0 - (300.0 * (intermediateY - m_display->yLow()) / yLength);
            finalPoints.append(QPointF(interpX, interpY));
            finalPoints.append(QPointF(interpX, 301.0));

            if(startIndex == endIndex)
                StartIsEndEdgeCase = true;
            else
                area += 0.5 * (m_upLimit - m_xValues[endIndex-1]) * (intermediateY + m_yValues[endIndex-1]);
        }
    }
    else
    {
        finalPoints.append(QPointF(450.0 * (m_xValues[m_xValues.size() -1] - m_display->xLow()) / xLength,
                           301.0));
    }

    // Add in points between start and end index.
    for(size_t i = startIndex; i < endIndex; i++)
    {
        xPos.push_back(450.0 * (m_xValues[i] - m_display->xLow()) / xLength);
        yPos.push_back(300.0 - (300.0 * (m_yValues[i] - m_display->yLow()) / yLength));
    }

    //Add up the area;
    for(size_t i = startIndex; i < endIndex - 1; i++)
    {
        area += (m_xValues[i+1] - m_xValues[i]) * 0.5 * (m_yValues[i+1] + m_yValues[i]);
    }

    /// This is quite annoying.
    if(StartIsEndEdgeCase)
    {
        double interLength = m_xValues[endIndex] - m_xValues[endIndex - 1];
        double intermediateY1 = m_yValues[endIndex] * (m_upLimit - m_xValues[endIndex - 1]) / interLength;
        intermediateY1 += m_yValues[endIndex - 1] * (m_xValues[endIndex] - m_upLimit) / interLength;
        double intermediateY2 = m_yValues[endIndex] * (m_lowLimit - m_xValues[endIndex - 1]) / interLength;
        intermediateY2 += m_yValues[endIndex - 1] * (m_xValues[endIndex] - m_lowLimit) / interLength;

        area = 0.5 * (m_upLimit - m_lowLimit) * (intermediateY1 + intermediateY2);
    }

    for(int i = 0; i < finalPoints.size(); i++)
    {
        xPos.push_back(finalPoints.at(i).x());
        yPos.push_back(finalPoints.at(i).y());
    }

    xPos.push_back(initialPoint.x());
    yPos.push_back(initialPoint.y());

    m_output = std::make_shared<NumberDataType>(area);
    setAreaShown(area);
    emit newData(0);
    emit displayAreaContinuous(xPos, yPos);

    emit stopSpinningGear();
}

void AreaUnderDistributionIdea::computeDiscrete()
{
    emit startSpinningGear();

    double xLength = m_display->xHigh() - m_display->xLow();
    double yLength = m_display->yHigh() - m_display->yLow();

    std::vector<double> xPos;
    std::vector<double> yPos;
    xPos.reserve(2*m_xValues.size() + 3);
    yPos.reserve(2*m_xValues.size() + 3);

    size_t startIndex = 0;
    size_t endIndex = m_xValues.size();
    QPointF initialPoint;
    QPointF finalPoint;

    double area = 0.0;

    //Handle the lower limit.
    if(m_lowLimitEnabled)
    {
        auto lower = std::lower_bound(m_xValues.begin(), m_xValues.end(), m_lowLimit);
        startIndex = std::distance(m_xValues.begin(), lower);
        if(startIndex == 0)
        {
            //Lower bound is below the distribution
            xPos.push_back(450.0 * (m_xValues[0] - m_display->xLow()) / xLength);
            yPos.push_back(301.0);
            initialPoint = QPointF(450.0 * (m_xValues[0] - m_display->xLow()) / xLength, 301.0);
        }
        else if (startIndex >= m_xValues.size())
        {
            //Lower bound is above the distribution.
            m_output = std::make_shared<NumberDataType>(0);
            setAreaShown(0.0);
            emit newData(0);
            emit displayAreaContinuous(std::vector<double>(), std::vector<double>());
            return;
        }
        else
        {
            double xLoc = 450.0 * (m_xValues[startIndex] - m_display->xLow()) / xLength;
            xPos.push_back(xLoc);
            yPos.push_back(301);
            initialPoint = QPoint(xLoc, 300);
        }
    }
    else
    {
        //Lower limit is negative infinity.
        xPos.push_back(450.0 * (m_xValues[0] - m_display->xLow()) / xLength);
        yPos.push_back(301.0);
        initialPoint = QPointF(450.0 * (m_xValues[0] - m_display->xLow()) / xLength, 301.0);
    }

    //Handle the upper limit.
    if(m_upLimitEnabled)
    {
        auto upper = std::lower_bound(m_xValues.begin(), m_xValues.end(), m_upLimit);
        endIndex = std::distance(m_xValues.begin(), upper);
        if(endIndex == 0)
        {
            // Upper bound is below distribution
            m_output = std::make_shared<NumberDataType>(0.0);
            emit newData(0);
            setAreaShown(0.0);
            emit displayAreaContinuous(std::vector<double>(), std::vector<double>());
            return;
        }
        else if(endIndex >= m_xValues.size())
        {
            finalPoint = QPointF(450.0 * (m_xValues[m_xValues.size() - 1] + 1 - m_display->xLow()) / xLength, 301.0);
        }
        else
        {
            qDebug() << "m_xValues[endIndex]: " << m_xValues[endIndex];

            double xLoc = 450.0 * (m_xValues[endIndex] - m_display->xLow()) / xLength;
            finalPoint = QPointF(xLoc, 300);

            //            area += 0.5 * (m_upLimit - m_xValues[endIndex-1]) * (intermediateY + m_yValues[endIndex-1]);
        }
    }
    else
    {
        //Upper limit is positive infinity.
        finalPoint = QPointF(450.0 * (m_xValues[m_xValues.size() - 1] + 1 - m_display->xLow()) / xLength, 301.0);
    }

    // Add in points between start and end index.
    for(size_t i = startIndex; i < endIndex; i++)
    {
        xPos.push_back(450.0 * (m_xValues[i] - m_display->xLow()) / xLength);
        xPos.push_back(450.0 * (m_xValues[i] + 1 - m_display->xLow()) / xLength);
        yPos.push_back(300.0 - (300.0 * (m_yValues[i] - m_display->yLow()) / yLength));
        yPos.push_back(300.0 - (300.0 * (m_yValues[i] - m_display->yLow()) / yLength));
    }

    //Add up the area;
    for(size_t i = startIndex; i < endIndex; i++)
    {
        area += m_yValues[i];
    }

    xPos.push_back(finalPoint.x());
    yPos.push_back(finalPoint.y());


    xPos.push_back(initialPoint.x());
    yPos.push_back(initialPoint.y());

    m_output = std::make_shared<NumberDataType>(area);
    setAreaShown(area);
    emit newData(0);
    emit displayAreaContinuous(xPos, yPos);

    emit stopSpinningGear();
}

void AreaUnderDistributionIdea::setBound(double value, int index)
{
    QJsonObject oldParams;;
    oldParams["d"] = m_display->save();

    bool ok = m_display->setBound(value, index);

    if(!ok)
        return;

    QJsonObject newParams;
    newParams["d"] = m_display->save();
    reportParamsChanged(oldParams, newParams);
}

void AreaUnderDistributionIdea::setAutoFit(bool autoFit)
{
    QJsonObject oldParams;;
    oldParams["d"] = m_display->save();

    bool ok = m_display->setAutoFit(autoFit);

    if(!ok)
        return;

    QJsonObject newParams;
    newParams["d"] = m_display->save();
    reportParamsChanged(oldParams, newParams);
}

void AreaUnderDistributionIdea::setLowLimit(double lowLimit)
{
    if (qFuzzyCompare(m_lowLimit, lowLimit))
        return;

    if(lowLimit > m_upLimit)
    {
        if(m_upLimitEnabled)
            return;
        else
        {
            m_upLimit = m_lowLimit + 1;
            emit upLimitChanged(m_upLimit);
        }
    }
    QJsonObject oldParams;
    oldParams["l"] = m_lowLimit;

    m_lowLimit = lowLimit;
    emit lowLimitChanged(m_lowLimit);

    QJsonObject newParams;
    newParams["l"] = m_lowLimit;

    if(!m_lowLimitEnabled)
    {
        oldParams["le"] = false;
        newParams["le"] = true;
        m_lowLimitEnabled = true;
        emit lowLimitEnabledChanged(m_lowLimitEnabled);
    }
    emit reportParamsChanged(oldParams, newParams);

    if(valid())
        compute();
}

void AreaUnderDistributionIdea::setUpLimit(double upLimit)
{
    if (qFuzzyCompare(m_upLimit, upLimit))
        return;

    if(upLimit < m_lowLimit)
    {
        if(m_lowLimitEnabled)
            return;
        else
        {
            m_lowLimit = m_upLimit - 1;
            emit lowLimitChanged(m_lowLimit);
        }
    }
    QJsonObject oldParams;
    oldParams["u"] = m_upLimit;

    m_upLimit = upLimit;
    emit upLimitChanged(m_upLimit);

    QJsonObject newParams;
    newParams["u"] = m_upLimit;

    if(!m_upLimitEnabled)
    {
        oldParams["ue"] = false;
        newParams["ue"] = true;
        m_upLimitEnabled = true;
        emit upLimitEnabledChanged(m_upLimitEnabled);
    }

    emit reportParamsChanged(oldParams, newParams);

    if(valid())
        compute();
}

void AreaUnderDistributionIdea::setLowLimitEnabled(bool lowLimitEnabled)
{
    if (m_lowLimitEnabled == lowLimitEnabled)
        return;

    QJsonObject oldParams;
    oldParams["le"] = m_lowLimitEnabled;

    m_lowLimitEnabled = lowLimitEnabled;
    emit lowLimitEnabledChanged(m_lowLimitEnabled);

    QJsonObject newParams;
    newParams["le"] = m_lowLimitEnabled;

    emit reportParamsChanged(oldParams, newParams);

    if(valid())
        compute();
}

void AreaUnderDistributionIdea::setUpLimitEnabled(bool upLimitEnabled)
{
    if (m_upLimitEnabled == upLimitEnabled)
        return;

    QJsonObject oldParams;
    oldParams["ue"] = m_upLimitEnabled;

    m_upLimitEnabled = upLimitEnabled;
    emit upLimitEnabledChanged(m_upLimitEnabled);

    QJsonObject newParams;
    newParams["ue"] = m_upLimitEnabled;

    emit reportParamsChanged(oldParams, newParams);

    if(valid())
        compute();
}

void AreaUnderDistributionIdea::setAreaShown(double areaShown)
{
    if (qFuzzyCompare(m_areaShown, areaShown))
        return;

    m_areaShown = areaShown;
    emit areaShownChanged(m_areaShown);
}


std::shared_ptr<DataType> AreaUnderDistributionIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_output);
}


void AreaUnderDistributionIdea::resetDisplay()
{
    if(valid())
    {
        m_display->plot();
        compute();
    }
}
