#include "trafficSignal.h"
#include <iostream>
#include <iomanip>
using namespace std;

TrafficSignal::TrafficSignal() {
    currentLane = nullptr;
    currentState = RED;
    minGreenDuration = 15;
    maxGreenDuration = 60;
    yellowDuration = 5;
    redDuration = 2;
    elapsedTime = 0;
}

TrafficSignal::TrafficSignal(int minGreen, int maxGreen, int yellow) {
    currentLane = nullptr;
    currentState = RED;
    minGreenDuration = minGreen;
    maxGreenDuration = maxGreen;
    yellowDuration = yellow;
    redDuration = 2;
    elapsedTime = 0;
}

string TrafficSignal::getStateName(SignalState state) const {
    switch (state) {
        case GREEN:  return "GREEN";
        case YELLOW: return "YELLOW";
        case RED:    return "RED";
        default:     return "UNKNOWN";
    }
}

string TrafficSignal::getSignalSymbol() const {
    switch (currentState) {
        case GREEN:  return "[G]";
        case YELLOW: return "[Y]";
        case RED:    return "[R]";
        default:     return "[ ]";
    }
}

int TrafficSignal::calculateOptimalGreenTime(Lane* lane) const {
    if (!lane) return minGreenDuration;

    int vehicleCount = lane->getVehicleCount();
    int optimalTime = 10 + (vehicleCount * 2);

    if (optimalTime < minGreenDuration) return minGreenDuration;
    if (optimalTime > maxGreenDuration) return maxGreenDuration;

    return optimalTime;
}

void TrafficSignal::setGreenSignal(Lane* lane, int duration) {
    currentLane = lane;
    currentState = GREEN;
    elapsedTime = 0;

    cout << "\nGREEN signal -> " << lane->getLaneName()
         << " (Duration: " << duration << "s)" << endl;
}

void TrafficSignal::setYellowSignal() {
    currentState = YELLOW;
    elapsedTime = 0;
    cout << "YELLOW signal" << endl;
}

void TrafficSignal::setRedSignal() {
    currentState = RED;
    elapsedTime = 0;
    cout << "RED signal" << endl;
}

void TrafficSignal::updateSignal(int timeStep) {
    elapsedTime += timeStep;
}

SignalState TrafficSignal::getCurrentState() const {
    return currentState;
}

Lane* TrafficSignal::getCurrentLane() const {
    return currentLane;
}

int TrafficSignal::getMinGreenDuration() const {
    return minGreenDuration;
}

int TrafficSignal::getMaxGreenDuration() const {
    return maxGreenDuration;
}

int TrafficSignal::getElapsedTime() const {
    return elapsedTime;
}

int TrafficSignal::clearVehicles(Lane* lane, int duration) {
    if (!lane) return 0;

    int maxVehicles = duration / 2;
    int vehiclesCleared = 0;

    cout << "  Clearing vehicles..." << endl;

    for (int i = 0; i < maxVehicles && !lane->isEmpty(); i++) {
        Vehicle v = lane->removeVehicle();
        vehiclesCleared++;

        if (v.isEmergency()) {
            cout << "  Emergency vehicle cleared." << endl;
        }
    }

    cout << "  Cleared " << vehiclesCleared << " vehicles from "
         << lane->getLaneName() << endl;
    cout << "  " << lane->getVehicleCount() << " vehicles still waiting." << endl;

    return vehiclesCleared;
}

void TrafficSignal::displaySignalStatus() const {
    cout << "\n+--------------------------------------+" << endl;
    cout << "|    TRAFFIC SIGNAL STATUS             |" << endl;
    cout << "+--------------------------------------+" << endl;

    if (currentLane) {
        cout << "| Lane   : " << setw(27) << left << currentLane->getLaneName() << " |" << endl;
    } else {
        cout << "| Lane   : " << setw(27) << left << "None" << " |" << endl;
    }

    string stateStr = getSignalSymbol() + " " + getStateName(currentState);
    cout << "| State  : " << setw(27) << left << stateStr << " |" << endl;
    cout << "| Elapsed: " << setw(25) << left << (to_string(elapsedTime) + "s") << " |" << endl;
    cout << "+--------------------------------------+\n" << endl;
}
