#include "ValueAtDistributionPointIdea.h"
#include <QJsonArray>

#include "DistributionCommonFunctions.h"

ValueAtDistributionPointAttributes::ValueAtDistributionPointAttributes()
{
    m_listName = "Distribution Value";
    m_displayName = "Distribution Value";
    m_authors << "dBob";
    m_description = "This box displays a distribution.";
    m_iconPath = "qrc:/Icons/ValueAtDistributionPoint.png";

    m_dataTypesIn << Magic<DistributionDataTypeAttributes>::cast().id();
    m_dataTypesOut << Magic<NumberDataTypeAttributes>::cast().id();

    m_minimizable = true;
    m_minimizedName = "Value";
    m_differentNameOnMinimized = true;
    m_minimizedQMLPath = ":/QML/ValueAtDistributionPointMinimized.qml";
    m_embeddedQMLPath = ":/QML/ValueAtDistributionPointIdea.qml";

    m_gear = true;
}

const IdeaAttributes ValueAtDistributionPointIdea::attributes()
{
    return Magic<ValueAtDistributionPointAttributes>::cast();
}

ValueAtDistributionPointIdea::ValueAtDistributionPointIdea() :
    m_value(0.0),
    m_point(0.0)

{
}

void ValueAtDistributionPointIdea::finishSetupQmlContext()
{
    m_display = new DistributionDisplay(this);
    setContextProperty("distributionDisplay", m_display);
}

void ValueAtDistributionPointIdea::onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber)
{
    auto distribution = std::dynamic_pointer_cast<DistributionDataType>(dataType);
    m_distribution = distribution;

    validate();
}

std::shared_ptr<DataType> ValueAtDistributionPointIdea::dataOut(PlugNumber)
{
    return std::static_pointer_cast<DataType>(m_output);
}

QJsonObject ValueAtDistributionPointIdea::save() const
{
    QJsonObject saveJson = Idea::save();

    saveJson["d"] = m_display->save();
    saveJson["p"] = m_point;

    return saveJson;
}

void ValueAtDistributionPointIdea::load(const QJsonObject &obj)
{
    QJsonValue v = obj["d"];
    if(!v.isUndefined())
    {
        m_display->load(v.toObject());
    }

    v = obj["p"];
    if(!v.isUndefined())
    {
        m_point = v.toDouble();
        emit pointChanged(m_point);
    }

    if(valid())
        compute();
}

double ValueAtDistributionPointIdea::value() const
{
    return m_value;
}

double ValueAtDistributionPointIdea::point() const
{
    return m_point;
}

void ValueAtDistributionPointIdea::validate()
{
    auto v_distribution = m_distribution.lock();
    if(v_distribution)
    {
        setStatus(IdeaStatus::Working, "Plotting", "This idea box is currently plotting the input distribution.");
        m_display->setXValues(v_distribution->xValues());
        m_display->setYValues(v_distribution->yValues());
        m_display->setDistType(v_distribution->distType());
        m_display->plot();

        m_xValues = v_distribution->xValues();
        m_yValues = v_distribution->yValues();
        m_distributionType = v_distribution->distType();

        compute();
    }
    else
    {
        setDefaultStatus();
        m_display->clearData();
    }
}

void ValueAtDistributionPointIdea::compute()
{
    if(m_distributionType == DistributionDataType::Continuous)
        computeContinuous();
    else
        computeDiscrete();
}

void ValueAtDistributionPointIdea::computeContinuous()
{
    emit startSpinningGear();

    auto lower = std::lower_bound(m_xValues.begin(), m_xValues.end(), m_point);
    size_t index = std::distance(m_xValues.begin(), lower);

    double value;
    if(index == m_xValues.size())
    {
        // point is above distribution.
        value = 0;
    }
    else if (index == 0)
    {
        // point is below distribution.
        value = 0;
    }
    else
    {
        double width = m_xValues[index] - m_xValues[index - 1];
        value = m_yValues[index] * (m_point - m_xValues[index-1]) / width;
        value += m_yValues[index-1] * (m_xValues[index] - m_point) / width;
    }

    setValue(value);

    m_output = std::make_shared<NumberDataType>(value);
    emit newData(0);

    emit stopSpinningGear();
}

void ValueAtDistributionPointIdea::computeDiscrete()
{
    emit startSpinningGear();

    auto lower = std::lower_bound(m_xValues.begin(), m_xValues.end(), m_point);
    size_t index = std::distance(m_xValues.begin(), lower);

    double value;
    if(index == m_xValues.size())
    {
        // point is above distribution.
        value = 0;
    }
    else if (index == 0)
    {
        // point is below distribution.
        value = 0;
    }
    else
    {
        value = m_yValues[index];
        m_point = m_xValues[index];
        emit pointChanged(m_point);
    }

    setValue(value);

    m_output = std::make_shared<NumberDataType>(value);
    emit newData(0);

    emit stopSpinningGear();
}

void ValueAtDistributionPointIdea::setBound(double value, int index)
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

void ValueAtDistributionPointIdea::setAutoFit(bool autoFit)
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

void ValueAtDistributionPointIdea::setValue(double value)
{
    if (qFuzzyCompare(m_value, value))
        return;

    m_value = value;
    emit valueChanged(m_value);
}

void ValueAtDistributionPointIdea::setPoint(double point)
{
    if (qFuzzyCompare(m_point, point))
        return;

    QJsonObject oldParams;
    oldParams["p"] = m_point;

    m_point = point;
    emit pointChanged(m_point);

    QJsonObject newParams;
    newParams["p"] = m_point;
    emit reportParamsChanged(oldParams, newParams);

    if(valid())
        compute();
}


void ValueAtDistributionPointIdea::resetDisplay()
{
    if(valid())
        m_display->plot();
}
