import QtQuick 2.12
import QtQuick.Controls 2.12

import com.martell1996.extras 1.0

Rectangle
{
    id: root
    width: 1030
    height: 440
    color: "#0f0f0f"

    Connections
    {
        target: idea
        function onHideR2()
        {
            r2Text.text = "";
        }
        function onHideCohensD()
        {
            cohensDText.text = "";
        }
        function onShowCohensD(d)
        {
            cohensDText.text = "d: " + d.toPrecision(4);
        }
        function onShowR2(r2)
        {
            r2Text.text = "R2: " + r2.toPrecision(4);
        }
    }

    Image
    {
        id: leaveCompanyRectangle;
        width: 100
        height: 100
        source: "qrc:/Images/GreenerPastures.png"
        x: 690
        y: 75
        z: 2
    }

    Image
    {
        id: enterCompanyRectangle
        width: 100
        height: 100
        source: "qrc:/Images/NewHires.png";
        x: 20
        y: 75
        z: 2
    }

    Rectangle
    {
        y: 24
        width: 960
        height: 350
        color: "black"

        Text
        {
            id: cohensDText
            color: "lightgray"
            x: 2
            font.pointSize: 10
        }
        Text
        {
            y: 14
            color: "lightgray"

            x: 2
            id: r2Text;
            font.pointSize: 10
        }

        MartellPyramidLevel
        {
            x: 308             // @disable-check M16
            width: 200;     // @disable-check M16
            gridY: 10    // @disable-check M16
            gridX: 1     // @disable-check M16
            gridHeight: 20 // @disable-check M16
            gridWidth: 200 // @disable-check M16
            cellSize: 20     // @disable-check M16
            model: Level8;     // @disable-check M16
            level: 8 // @disable-check M16
        }

        MartellPyramidLevel
        {
            x: 264             // @disable-check M16
            y: 44             // @disable-check M16
            width: 288;     // @disable-check M16
            gridY: 8    // @disable-check M16
            gridX: 4     // @disable-check M16
            gridHeight: 28 // @disable-check M16
            gridWidth: 288 // @disable-check M16
            cellSize: 14     // @disable-check M16
            model: Level7;     // @disable-check M16
            level: 7 // @disable-check M16
        }

        MartellPyramidLevel
        {
            x: 220             // @disable-check M16
            y: 88             // @disable-check M16
            width: 376;     // @disable-check M16
            gridY: 1    // @disable-check M16
            gridX: 25     // @disable-check M16
            gridHeight: 40 // @disable-check M16
            gridWidth: 330 // @disable-check M16
            cellSize: 13     // @disable-check M16
            model: Level6;     // @disable-check M16
            level: 6 // @disable-check M16
        }

        MartellPyramidLevel
        {
            x: 176             // @disable-check M16
            y: 132             // @disable-check M16
            width: 464;     // @disable-check M16
            gridY: 1    // @disable-check M16
            gridX: 11     // @disable-check M16
            gridHeight: 40 // @disable-check M16
            gridWidth: 442 // @disable-check M16
            cellSize: 13     // @disable-check M16
            model: Level5;     // @disable-check M16
            level: 5 // @disable-check M16
        }

        MartellPyramidLevel
        {
            x: 132             // @disable-check M16
            y: 176             // @disable-check M16
            width: 552;     // @disable-check M16
            gridY: 4    // @disable-check M16
            gridX: 1     // @disable-check M16
            gridHeight: 40 // @disable-check M16
            gridWidth: 550 // @disable-check M16
            cellSize: 11     // @disable-check M16
            model: Level4;     // @disable-check M16
            level: 4 // @disable-check M16
        }

        MartellPyramidLevel
        {
            x: 88             // @disable-check M16
            y: 220             // @disable-check M16
            width: 640;     // @disable-check M16
            gridY: 1    // @disable-check M16
            gridX: 21     // @disable-check M16
            gridHeight: 40 // @disable-check M16
            gridWidth: 600 // @disable-check M16
            cellSize: 10     // @disable-check M16
            model: Level3;     // @disable-check M16
            level: 3 // @disable-check M16
        }

        MartellPyramidLevel
        {
            x: 44             // @disable-check M16
            y: 264             // @disable-check M16
            width: 728;     // @disable-check M16
            gridY: 5    // @disable-check M16
            gridX: 13     // @disable-check M16
            gridHeight: 40 // @disable-check M16
            gridWidth: 704 // @disable-check M16
            cellSize: 8     // @disable-check M16
            model: Level2;     // @disable-check M16
            level: 2 // @disable-check M16
        }

        MartellPyramidLevel
        {
            x: 0             // @disable-check M16
            y: 308             // @disable-check M16
            width: 816;     // @disable-check M16
            gridY: 1    // @disable-check M16
            gridX: 9     // @disable-check M16
            gridHeight: 40 // @disable-check M16
            gridWidth: 800 // @disable-check M16
            cellSize: 8     // @disable-check M16
            model: Level1;     // @disable-check M16
            level: 1 // @disable-check M16
        }
    }

    Column
    {
        id: percentWomenColumn
        anchors.right: parent.right
        spacing: 4
        Rectangle
        {
            color: "#ececec"
            height: 20
            width: 70
            Text
            {
                anchors.centerIn: parent
                text: qsTr("%Women")
            }
        }
        Repeater
        {
            model: 8
            delegate: Rectangle
            {
                width: 70
                height: 40
                color: "#ececec"
                Text
                {
                    anchors.centerIn: parent
                    text: idea.percentWomen[7-model.index].toFixed(1);
                }
            }

        }
    }
    Column
    {
        id: meanScoreColumn
        anchors.right: percentWomenColumn.left
        anchors.rightMargin: 4
        spacing: 4
        Rectangle
        {
            color: "#ececec"
            height: 20
            width: 50
            Text
            {
                anchors.centerIn: parent
                text: qsTr("Score")
            }
        }
        Repeater
        {
            model: 8
            delegate: Rectangle
            {
                width: 50
                height: 40
                color: "#ececec"
                Text
                {
                    anchors.centerIn: parent
                    text: idea.meanScore[7-model.index].toFixed(2);
                }
            }

        }
    }

    Column
    {
        id: attritionRate
        anchors.right: meanScoreColumn.left
        anchors.rightMargin: 4
        spacing: 4
        Rectangle
        {
            color: "#ececec"
            height: 20
            width: 76
            Text
            {
                anchors.centerIn: parent
                text: qsTr("%Attrition")
            }
        }
        Repeater
        {
            model: 8
            delegate: Rectangle
            {
                width: 76
                height: 40
                color: "#ececec"
                Text
                {
                    anchors.centerIn: parent
                    text: (100.0*idea.attritionRate[7-model.index]).toFixed(0);
                }
            }

        }
    }

    Rectangle
    {
        id: singleRunControls
        y: 390
        color: "grey"
        height: 45
        width: parent.width
        Row
        {
            anchors.fill: parent
            spacing: 10
            anchors.leftMargin: 10

            Button
            {
                anchors.verticalCenter: parent.verticalCenter
                text: "Step";
                onClicked:
                {
                    idea.normalStepButtonClicked();
                }
            }

            Button
            {
                anchors.verticalCenter: parent.verticalCenter
                text: "Full Replacement";
                onClicked:
                {
                    idea.normalFinishButtonClicked();
                }
            }

            Button
            {
                anchors.verticalCenter: parent.verticalCenter
                text: "reset";
                onClicked:
                {
                    idea.normalResetButtonClicked();
                }
            }
            Rectangle
            {
                anchors.verticalCenter: parent.verticalCenter;
                height: 35
                width: 240
                color: "lightgray"
                Text
                {
                    visible: idea.valid;
                    anchors.centerIn: parent
                    text: "Original Employees: " + idea.originals;
                }
            }
            Rectangle
            {
                anchors.verticalCenter: parent.verticalCenter;
                height: 35
                width: 200;
                color: "lightgray"
                Text
                {
                    visible: idea.valid;
                    anchors.centerIn: parent
                    text: "Promotion Cycles: " + idea.promotionCycles;
                }
            }
        }
    }


}





