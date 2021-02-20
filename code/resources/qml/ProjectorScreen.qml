import QtQuick 2.2
import QtMultimedia 5.2
import QtGraphicalEffects 1.0

Item {

    function stopBackgroundVideo()
    {
        backgroundVideoOutput.visible = false;
        backgroundVideo.stop();
    }

    function startBackgroundVideo()
    {
        backgroundVideo.play();
        backgroundVideoOutput.visible = true;
    }

    function loadBackgroundImage(fullFilePath)
    {
        backgroundImage.source = fullFilePath;
    }

    function loadBackgroundVideo(fullFilePath)
    {
        backgroundVideo.source = fullFilePath;
    }

    Rectangle {
        id: backgroundColor
        objectName: "backgroundColor"
        anchors.fill: parent
        color: "#000"
    }

    Image {
        id: backgroundImage
        objectName: "backgroundImage"
        source: ""
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.fill: parent;
    }

    MediaPlayer {
        id: backgroundVideo
        objectName: "backgroundVideo"
        autoPlay: false
        autoLoad: false
        loops: Animation.Infinite
        volume: 0
        onError: {
            console.log("Player Error");
            stopBackgroundVideo();
        }
        onPlaybackStateChanged: {
            if (playbackState == MediaPlayer.PlayingState) {
                console.log("Player Started");
            } else if (playbackState == MediaPlayer.PausedState) {
                console.log("Player Paused");
            } else if (playbackState == MediaPlayer.StoppedState) {
                console.log("Player Stopped");
            }
        }
        onStatusChanged: {
            console.log("ST: " + status);
        }
        onDurationChanged: {
            console.log("DUR: " + duration);
        }
    }

    VideoOutput {
        property bool stretch: true

        objectName: "backgroundVideoOutput"
        id: backgroundVideoOutput
        source: backgroundVideo

        anchors.fill: parent
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.centerIn: parent

        fillMode: (stretch == true ? VideoOutput.Stretch : VideoOutput.PreserveAspectFit)
    }

    Text {
        property bool autoFit: true

        id: renderText
        color: "#FFF"
        objectName: "renderText"
        text: qsTr("")
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.fill: parent
        anchors.centerIn: parent
        visible: false
        fontSizeMode: (autoFit == true ? Text.Fit : Text.Normal)
    }

    Glow {
        objectName: "renderTextGlow"
        radius: 3
        samples: 30
        color: "#000"
        spread: 1.0
        source: renderText
        anchors.fill: renderText
    }

}
