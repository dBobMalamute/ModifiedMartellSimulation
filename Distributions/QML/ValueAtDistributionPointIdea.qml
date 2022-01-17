import QtQuick 2.12
import QtQuick.Controls 2.12

import com.distributions.dists 1.0

Item
{
    width: 530
    height: 380

    DistributionDisplay
    {

    }

    Rectangle
    {
        id: dot
        width: 10
        height: 10
        radius: 10
        color: "red"
        opacity: 0.8
        y: 305 - (300 * (idea.value - distributionDisplay.yLow) /
                  (distributionDisplay.yHigh - distributionDisplay.yLow))
        x: 50 + 450 * (idea.point - distributionDisplay.xLow) /
           (distributionDisplay.xHigh - distributionDisplay.xLow);
    }
    Rectangle
    {
        id: bar;
        x: dot.x + 4
        y: dot.y + 5
        width: 2
        height: 305-dot.y
        color: "red"
        opacity: 0.8
    }

    Rectangle
    {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 140
        height: 40
        radius: 8
        color: "#342b3b"

        Image
        {
            anchors.top: parent.top
            anchors.left: parent.left
            source: "qrc:/Images/ValueAtDistributionPoint.png"
        }

        Rectangle
        {
            anchors.top: parent.top
            anchors.right: parent.right
            color: "Black"
            width: 70
            height: 40
            TextInput
            {
                anchors.centerIn: parent
                text: idea.point;
                color: "#f33c3c";
                font.pointSize: 12
                selectByMouse: true
                validator: DoubleValidator{}
                onEditingFinished:
                {
                    idea.setPoint(text);
                }
            }
        }
    }

    Rectangle
    {
        height: 40
        width: 140
        x: 170
        anchors.bottom: parent.bottom
        color: "#5a0000"
        border.width: 3
        radius: 6
        border.color: "black"

        Text
        {
            anchors.centerIn: parent
            font.pointSize: 12
            color: "#cccccc"

            text: "Value: " + idea.value.toPrecision(4);
        }
    }
}


