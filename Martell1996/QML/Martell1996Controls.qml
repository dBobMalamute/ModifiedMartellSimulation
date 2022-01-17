import QtQuick 2.12
import QtQuick.Controls 2.12
import com.malamute.controlsStyles 1.0

import com.martell1996.extras 1.0

Rectangle
{
    width: 230
    height: 190

    radius: ControlsStyle.radius()
    border.color: ControlsStyle.borderColor()
    border.width: ControlsStyle.borderWidth()

    gradient: Gradient
    {
        GradientStop{ position: 0.05; color: ControlsStyle.gradientTop()}
        GradientStop{position: 0.5; color: ControlsStyle.gradientMid()}
        GradientStop{ position: 0.95; color: ControlsStyle.gradientBottom()}
    }


    Column
    {
        anchors.fill: parent
        anchors.margins: 10
        Text
        {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Animation Options"
            color: ControlsStyle.textColor();
            font: ControlsStyle.textFont();
        }
        Row
        {
            id: speedRow
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10
            Button
            {
                text: "Start";
                width: 80
                height: 40
                onClicked:
                {
                    idea.animationStartButtonClicked();
                }
            }
            Button
            {
                text: "Stop";
                width: 80
                height: 40
                onClicked:
                {
                    idea.animationStopButtonClicked();
                }
            }
        }
        Row
        {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10
            Button
            {
                text: "Step";
                width: 80
                height: 40
                onClicked:
                {
                    idea.animationStepButtonClicked();
                }
            }
            Button
            {
                text: "Reset";
                width: 80
                height: 40
                onClicked:
                {
                    idea.animationResetButtonClicked();
                }
            }
        }

        Row
        {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10
            Text
            {
                anchors.verticalCenter: parent.verticalCenter
                text: "Speed"
                color: "#FF1744"
                font.pointSize: 12
            }
            Slider
            {
                anchors.verticalCenter: parent.verticalCenter
                from: -1
                to: 1
                value: -1.0 * Math.log(idea.animationSpeed)
                width: 100
                height: 30

                onMoved:
                {
                    idea.setAnimationSpeed(Math.exp(-1.0*value));
                }
            }
        }

        CheckBox
        {
            id: checkBox
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Animate"
            height: 36
            checked: idea.mode === SimulationMode.ANIMATE ? true : false;

            onClicked:
            {
                idea.setToAnimationMode(checked);
            }
            Rectangle
            {
                radius: 2
                anchors.fill: parent
                color: "lightgray"
            }

        }
    }
}
