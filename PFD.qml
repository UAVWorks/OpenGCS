import QtQuick 1.1
import "./components"

Rectangle {
    // Property Defintions
    property real roll: 0
    property real pitch: 0
    property real heading: 0

    property real alt: 0
    property real airspeed: 0
    property real groundspeed: 0

    property bool enableBackgroundVideo: false

    // Dial with a slider to adjust it
    RollPitchIndicator {
        id: rollPitchIndicator
        anchors.centerIn: parent
        rollAngle: parent.roll
        pitchAngle: parent.pitch
        enableBackgroundVideo: parent.enableBackgroundVideo
    }

    PitchIndicator {
        id: pitchIndicator
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        opacity: 0.6

        pitchAngle: parent.pitch
        rollAngle: parent.roll
    }

    AltitudeIndicator {
        id: altIndicator
        anchors.right: parent.right
        alt: parent.alt
    }

    SpeedIndicator {
        id: speedIndicator
        anchors.left: parent.left
        airspeed: parent.airspeed
        groundspeed: parent.groundspeed
    }

    CompassIndicator {
        id: compassIndicator
        anchors.horizontalCenter: parent.horizontalCenter
        heading: parent.heading
    }
}
