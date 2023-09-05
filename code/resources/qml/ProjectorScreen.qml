import QtQuick 2.15
import QtQuick.Controls 2.15
import QtMultimedia 6.0
import Qt5Compat.GraphicalEffects

Item {

    function stopBackgroundVideo() {
        backgroundVideoOutput.visible = false;
        backgroundVideo.stop();
    }

    function startBackgroundVideo() {
        backgroundVideo.play();
        backgroundVideoOutput.visible = true;
    }

    function loadBackgroundImage(fullFilePath) {
        backgroundImage.source = fullFilePath;
    }

    function loadBackgroundVideo(fullFilePath) {
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
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
    }

    MediaPlayer {
        id: backgroundVideo
        objectName: "backgroundVideo"
        loops: MediaPlayer.Infinite
        videoOutput: backgroundVideoOutput
        audioOutput: AudioOutput {
            volume: 0
        }
        onErrorOccurred: {
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
        onDurationChanged: {
            console.log("DUR: " + duration);
        }
    }

    VideoOutput {
        property bool stretch: true

        objectName: "backgroundVideoOutput"
        id: backgroundVideoOutput

        anchors.fill: parent
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        fillMode: (stretch == true ? VideoOutput.PreserveAspectCrop : VideoOutput.PreserveAspectFit)
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
