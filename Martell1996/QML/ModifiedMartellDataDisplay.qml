import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Rectangle
{
    width: 370
    height: 355
    color: "#cccccc"

    Rectangle
    {
        width: parent.width
        height: 3
        color: "black"
    }

    Row
    {
        x: 5
        y: 3
        height: 34
        Text
        {
            text: "Level"
            horizontalAlignment: Text.AlignHCenter
            anchors.verticalCenter: parent.verticalCenter
            width: 70
        }
        Text
        {
            text: "Incumbent's\nMean Score"
            horizontalAlignment: Text.AlignHCenter
            anchors.verticalCenter: parent.verticalCenter
            width: 100
        }
        Text
        {
            text: "Number\nof Positions"
            horizontalAlignment: Text.AlignHCenter
            anchors.verticalCenter: parent.verticalCenter
            width: 100
        }
        Text
        {
            text: "Percentage\nof Women";
            horizontalAlignment: Text.AlignHCenter
            anchors.verticalCenter: parent.verticalCenter
            width: 100
        }
    }

    Rectangle
    {
        y: 37
        width: parent.width
        height: 3
        color: "black"
    }

    ColumnLayout
    {
        y: 40
        width: 370
        height: 275
        ListView
        {
            id: levelListView;
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: dataModel
            spacing: 5

            ScrollBar.vertical: ScrollBar
            {
                width: 8
                anchors.right: levelListView.left
                policy: ScrollBar.AlwaysOn
            }

            delegate: Rectangle
            {
                width: 370
                height: 30
                color: "#ececec"
                Row
                {
                    anchors.fill: parent
                    Text
                    {
                        text: model.level
                        horizontalAlignment: Text.AlignHCenter
                        anchors.verticalCenter: parent.verticalCenter
                        width: 70
                    }
                    Text
                    {
                        text: model.meanScore.toFixed(2)
                        horizontalAlignment: Text.AlignHCenter
                        anchors.verticalCenter: parent.verticalCenter
                        width: 100
                    }
                    Text
                    {
                        text: model.positions
                        horizontalAlignment: Text.AlignHCenter
                        anchors.verticalCenter: parent.verticalCenter
                        width: 100
                    }
                    Text
                    {
                        text: model.percentWomen.toFixed(0)
                        horizontalAlignment: Text.AlignHCenter
                        anchors.verticalCenter: parent.verticalCenter
                        width: 100
                    }
                }
            }
        }
    }

    Rectangle
    {
        y: 315
        width: parent.width
        color: "black"
        height: 10
    }

    Rectangle
    {
        y: 325
        height: 30
        color: "#ececec"
        width: parent.width
        Text
        {
            id: promotionCyclesText
            anchors.centerIn: parent;
            visible: idea.valid;
            text: "Average Promotion Cycles: " + 40
        }
        Connections
        {
            target: idea
            function onDisplayAveragePromotionCycles(cycles)
            {
                promotionCyclesText.text = "Average Promotion Cycles: " + cycles.toFixed(2);
            }
        }
    }

}
