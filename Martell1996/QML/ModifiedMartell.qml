import QtQuick 2.12
import QtQuick.Shapes 1.12
import QtQuick.Controls 2.12
import com.martell1996.extras 1.0

Rectangle
{
    width: 734
    height: 460
    color: "black"

    Text
    {
        y: 105
        x: 5
        id: cohensDText
        font.pointSize: 10
        color: "lightGray"
    }

    Connections
    {
        target: idea

        function onSetupPieChart(numSections, sweeps, colors)
        {
            //This is a weird way of handling this. Look for a better way.
            section1.fillColor = "transparent";
            section2.fillColor = "transparent";
            section3.fillColor = "transparent";
            section4.fillColor = "transparent";
            section5.fillColor = "transparent";
            section6.fillColor = "transparent";

            if(numSections >= 1)
            {
                section1.fillColor = colors[0];
                section1.sweepAngle = sweeps[0];
            }
            if(numSections >= 2)
            {
                section2.fillColor = colors[1];
                section2.startAngle = sweeps[0];
                section2.sweepAngle = sweeps[1] - sweeps[0];
            }
            if(numSections >= 3)
            {
                section3.fillColor = colors[2];
                section3.startAngle = sweeps[1];
                section3.sweepAngle = sweeps[2] - sweeps[1];
            }
            if(numSections >= 4)
            {
                section4.fillColor = colors[3];
                section4.startAngle = sweeps[2];
                section4.sweepAngle = sweeps[3] - sweeps[2];
            }
            if(numSections >= 5)
            {
                section5.fillColor = colors[4];
                section5.startAngle = sweeps[3];
                section5.sweepAngle = sweeps[4] - sweeps[3];
            }
            if(numSections >= 6)
            {
                section6.fillColor = colors[5];
                section6.startAngle = sweeps[4];
                section6.sweepAngle = sweeps[5] - sweeps[4];
            }
        }

        function onSetupNameRectangles(numRects, names, colors)
        {
            let i; let comp; let obj;
            for(i = 0; i < nameRectangleArea.children.length; i++)
                nameRectangleArea.children[i].destroy();

            for(i = 0; i < numRects; i++)
            {
                comp = Qt.createComponent("ModifiedMartellPieChartKey.qml")
                obj = comp.createObject(nameRectangleArea, {
                                            groupColor: colors[i],
                                            groupName: names[i],
                                            y: 2 + 18*i});
            }
        }

        function onShowCohensD(d)
        {
            cohensDText.text = "Cohens D: " + d.toPrecision(4)
        }
        function onHideCohensD()
        {
            cohensDText.text = "";
        }
    }

    Shape
    {
        id: pieChartShape
        width: 400
        height: 400
        x: -148
        y: -148
        scale: 0.25
        layer.enabled: true
        layer.samples: 12

        ModifiedMartellPieChartPath
        {
            id: section1
            fillColor: "transparent";
            sweepAngle: 120.0;
        }
        ModifiedMartellPieChartPath
        {
            id: section2
            fillColor: "transparent";
            startAngle: 120.0;
            sweepAngle: 120.0;
        }
        ModifiedMartellPieChartPath
        {
            id: section3
            fillColor: "transparent";
            startAngle: 120.0;
            sweepAngle: 120.0;
        }
        ModifiedMartellPieChartPath
        {
            id: section4
            fillColor: "transparent";
            startAngle: 120.0;
            sweepAngle: 120.0;
        }
        ModifiedMartellPieChartPath
        {
            id: section5
            fillColor: "transparent";
            startAngle: 120.0;
            sweepAngle: 120.0;
        }
        ModifiedMartellPieChartPath
        {
            id: section6
            fillColor: "transparent";
            startAngle: 120.0;
            sweepAngle: 120.0;
        }
    }

    Item
    {
        id: nameRectangleArea
        x: 105
        y: 0
        width: 100
        height: 100
    }

    //    Column
    //    {
    //        width: 540
    //        height: 360
    //        x: 10
    //        y: 40
    //        spacing: 2

    //        Repeater
    //        {
    //            id: levelsRepeater
    //            anchors.fill: parent
    //            model: idea.numLevels
    //            delegate: Rectangle
    //            {
    //                id: rectDelegate
    //                anchors.horizontalCenter: parent.horizontalCenter
    //                width: 180 + 360.0 * index / idea.numLevels
    //                height: (360 / idea.numLevels) - 2
    //                color: "#3e3124"
    //            }
    //        }
    //    }

    //This is weird, but I don't know a better way.
    ModifiedMartellPyramidLevel{level: 0; model: Level0}
    ModifiedMartellPyramidLevel{level: 1; model: Level1}
    ModifiedMartellPyramidLevel{level: 2; model: Level2}
    ModifiedMartellPyramidLevel{level: 3; model: Level3}
    ModifiedMartellPyramidLevel{level: 4; model: Level4}
    ModifiedMartellPyramidLevel{level: 5; model: Level5}
    ModifiedMartellPyramidLevel{level: 6; model: Level6}
    ModifiedMartellPyramidLevel{level: 7; model: Level7}
    ModifiedMartellPyramidLevel{level: 8; model: Level8}
    ModifiedMartellPyramidLevel{level: 9; model: Level9}
    ModifiedMartellPyramidLevel{level: 10; model: Level10}
    ModifiedMartellPyramidLevel{level: 11; model: Level11}
    ModifiedMartellPyramidLevel{level: 12; model: Level12}
    ModifiedMartellPyramidLevel{level: 13; model: Level13}
    ModifiedMartellPyramidLevel{level: 14; model: Level14}
    ModifiedMartellPyramidLevel{level: 15; model: Level15}
    ModifiedMartellPyramidLevel{level: 16; model: Level16}
    ModifiedMartellPyramidLevel{level: 17; model: Level17}
    ModifiedMartellPyramidLevel{level: 18; model: Level18}
    ModifiedMartellPyramidLevel{level: 19; model: Level19}
    ModifiedMartellPyramidLevel{level: 20; model: Level20}
    ModifiedMartellPyramidLevel{level: 21; model: Level21}
    ModifiedMartellPyramidLevel{level: 22; model: Level22}
    ModifiedMartellPyramidLevel{level: 23; model: Level23}
    ModifiedMartellPyramidLevel{level: 24; model: Level24}
    ModifiedMartellPyramidLevel{level: 25; model: Level25}
    ModifiedMartellPyramidLevel{level: 26; model: Level26}
    ModifiedMartellPyramidLevel{level: 27; model: Level27}
    ModifiedMartellPyramidLevel{level: 28; model: Level28}
    ModifiedMartellPyramidLevel{level: 29; model: Level29}

    Rectangle
    {
        radius: 4
        color: "darkGray"
        y: 6
        width: 80
        height: 30
        x: 560
        Text
        {
            anchors.centerIn: parent
            text: "Score"
            font.bold: true
            font.pointSize: 12
            color: "black"
        }
    }

    Column
    {
        width: 80
        height: 360
        x: 560
        y: 40
        spacing: 2
        Repeater
        {
            id: avgScoreRepeater
            anchors.fill: parent
            model: 0
            delegate: Rectangle
            {
                width: 80
                height: (360 / idea.numLevels) - 2
                color: "lightgray"
                Text
                {
                    anchors.centerIn: parent
                    text: idea.meanScore[index].toPrecision(4);
                    font.pointSize: Math.min(10, parent.height - 2)
                    color: "black"
                }
            }
        }
        Connections
        {
            target: idea
            function onMeanScoreChanged(meanScore)
            {
                avgScoreRepeater.model = idea.numLevels;
            }
        }
    }

    Rectangle
    {
        radius: 4
        color: "darkGray"
        y: 6
        width: 80
        height: 30
        x: 650
        Text
        {
            anchors.centerIn: parent
            text: "%Women"
            font.bold: true
            font.pointSize: 12
            color: "black"
        }
    }

    Column
    {
        width: 80
        height: 360
        x: 650
        y: 40
        spacing: 2
        Repeater
        {
            id: percentWomenRepeater
            anchors.fill: parent
            model: 0
            delegate: Rectangle
            {
                width: 80
                height: (360 / idea.numLevels) - 2
                color: "lightgray"
                Text
                {
                    anchors.centerIn: parent
                    text: idea.percentWomen[index].toPrecision(4);
                    font.pointSize: Math.min(10, parent.height - 2)
                    color: "black"
                }
            }

        }
        Connections
        {
            target: idea
            function onPercentWomenChanged(percentWomen)
            {
                percentWomenRepeater.model = idea.numLevels;
            }
        }
    }

    Rectangle
    {
        id: singleRunControls
        anchors.bottom: parent.bottom
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
                width: 160
                color: "lightgray"
                Text
                {
                    visible: idea.valid;
                    anchors.centerIn: parent
                    text: "Originals: " + idea.originals;
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
