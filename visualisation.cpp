#include "visualisation.h"
#include <iostream>
#include <iomanip>
using namespace std;

string TrafficVisualizer::getSignalSymbol(Lane* lane, Lane* greenLane) {
    if (lane == greenLane) return "[GREEN]";
    return "[RED]  ";
}

string TrafficVisualizer::getVehicleSymbol(int count) {
    if (count == 0)  return "  ";
    if (count <= 3)  return "[C]";
    if (count <= 7)  return "[CC]";
    return "[CCC]";
}

void TrafficVisualizer::displayIntersection(
    Lane* north, Lane* south, Lane* east, Lane* west,
    Lane* currentGreen
) {
    cout << "\n" << string(65, '=') << endl;
    cout << "           TRAFFIC INTERSECTION (Top View)" << endl;
    cout << string(65, '=') << "\n" << endl;

    cout << "                       NORTH" << endl;
    cout << "                     " << getSignalSymbol(north, currentGreen) << endl;

    int northVehicles = north->getVehicleCount();
    int displayCount = (northVehicles > 5) ? 5 : northVehicles;

    for (int i = 0; i < displayCount; i++) {
        cout << "                       [C]" << endl;
    }
    if (northVehicles > 5) {
        cout << "                    (" << northVehicles - 5 << " more)" << endl;
    }
    if (north->hasEmergencyVehicle()) {
        cout << "                      [EMERGENCY]" << endl;
    }

    cout << endl;

    cout << "WEST ";
    int westVehicles = west->getVehicleCount();
    int westDisplay = (westVehicles > 3) ? 3 : westVehicles;
    for (int i = 0; i < westDisplay; i++) {
        cout << "[C] ";
    }
    if (westVehicles > 3) cout << "(" << westVehicles - 3 << "+) ";

    cout << getSignalSymbol(west, currentGreen);
    cout << " ======+====== ";
    cout << getSignalSymbol(east, currentGreen) << " ";

    int eastVehicles = east->getVehicleCount();
    int eastDisplay = (eastVehicles > 3) ? 3 : eastVehicles;
    for (int i = 0; i < eastDisplay; i++) {
        cout << "[C] ";
    }
    if (eastVehicles > 3) cout << "(" << eastVehicles - 3 << "+)";
    cout << " EAST" << endl;

    if (west->hasEmergencyVehicle()) {
        cout << "     [EMERGENCY]";
    }
    cout << endl << endl;

    int southVehicles = south->getVehicleCount();
    if (southVehicles > 5) {
        cout << "                    (" << southVehicles - 5 << " more)" << endl;
    }

    displayCount = (southVehicles > 5) ? 5 : southVehicles;
    for (int i = 0; i < displayCount; i++) {
        cout << "                       [C]" << endl;
    }
    if (south->hasEmergencyVehicle()) {
        cout << "                      [EMERGENCY]" << endl;
    }

    cout << "                     " << getSignalSymbol(south, currentGreen) << endl;
    cout << "                       SOUTH\n" << endl;

    cout << string(65, '=') << "\n" << endl;
}

void TrafficVisualizer::displayCompactStatus(
    vector<Lane*> lanes,
    Lane* currentGreen,
    int cycleNumber
) {
    cout << "\n+-------------------------------------------------------------+" << endl;
    cout << "|  CYCLE " << left << setw(4) << cycleNumber << " - LANE STATUS"
         << setw(36) << " " << "|" << endl;
    cout << "+-------------------------------------------------------------+" << endl;

    for (Lane* lane : lanes) {
        cout << "| ";

        if (lane == currentGreen) {
            cout << "[G] ";
        } else {
            cout << "[R] ";
        }

        cout << setw(8) << left << lane->getLaneName();
        cout << "| Vehicles: " << setw(3) << lane->getVehicleCount();
        cout << " | Wait: ";
        displayWaitingBar(lane->getAverageWaitingTime(), 60);
        cout << " | Pri: " << fixed << setprecision(1)
             << setw(6) << lane->calculatePriority();

        if (lane->hasEmergencyVehicle()) {
            cout << " [EMRG]";
        }
        if (lane->isBlocked()) {
            cout << " [BLKD]";
        }

        cout << " |" << endl;
    }

    cout << "+-------------------------------------------------------------+" << endl;
}

void TrafficVisualizer::displayWaitingBar(int waitTime, int maxWait) {
    int barLength = 10;
    int filled = (waitTime * barLength) / maxWait;
    if (filled > barLength) filled = barLength;

    cout << "[";
    for (int i = 0; i < filled; i++) {
        cout << "#";
    }
    for (int i = filled; i < barLength; i++) {
        cout << "-";
    }
    cout << "] " << setw(3) << waitTime << "s";
}
