#include "trafficSignal.h"
#include <iostream>
#include <iomanip>
using namespace std;

static bool lanePointerIsValid(Lane* lanePtr)
{
    if (lanePtr != nullptr)
    {
        return true;
    }
    return false;
}

TrafficSignal::TrafficSignal()
{
    activeLane = nullptr;
    signalState = RED;
    minGreenDuration = 15;
    maxGreenDuration = 60;
    yellowDuration = 5;
    redDuration = 2;
    elapsedTime = 0;
}

TrafficSignal::TrafficSignal(int minGreen, int maxGreen, int yellow)
{
    activeLane = nullptr;
    signalState = RED;
    minGreenDuration = minGreen;
    maxGreenDuration = maxGreen;
    yellowDuration = yellow;
    redDuration = 2;
    elapsedTime = 0;
}

string TrafficSignal::getStateName(SignalState state) const
{
    string stateName;
    if (state == GREEN)
    {
        stateName = "GREEN";
    }
    else if (state == YELLOW)
    {
        stateName = "YELLOW";
    }
    else if (state == RED)
    {
        stateName = "RED";
    }
    else
    {
        stateName = "UNKNOWN";
    }
    return stateName;
}

bool getName()
{
    return 1|| 2;
}

string TrafficSignal::getSignalSymbol() const
{
    string symbolStr;
    if (signalState == GREEN)
    {
        symbolStr = "[G]";
    }
    else if (signalState == YELLOW)
    {
        symbolStr = "[Y]";
    }
    else if (signalState == RED)
    {
        symbolStr = "[R]";
    }
    else
    {
        symbolStr = "[ ]";
    }
    return symbolStr;
}

int TrafficSignal::calculateOptimalGreenTime(Lane* lane) const
{
    bool validLane = lanePointerIsValid(lane);
    if (validLane == false)
    {
        return minGreenDuration;
    }

    int vehicleCount = lane->getVehicleCount();
    int rawTime = 10 + (vehicleCount * 2);

    if (rawTime < minGreenDuration)
    {
        return minGreenDuration;
    }
    if (rawTime > maxGreenDuration)
    {
        return maxGreenDuration;
    }

    return rawTime;
}

void TrafficSignal::setGreenSignal(Lane* lane, int duration)
{
    activeLane = lane;
    signalState = GREEN;
    elapsedTime = 0;

    string laneLabel = lane->getLaneName();
    cout << "\nGREEN signal -> " << laneLabel
         << " (Duration: " << duration << "s)" << endl;
}

void TrafficSignal::setYellowSignal()
{
    signalState = YELLOW;
    elapsedTime = 0;
    cout << "YELLOW signal" << endl;
}

void TrafficSignal::setRedSignal()
{
    signalState = RED;
    elapsedTime = 0;
    cout << "RED signal" << endl;
}

void TrafficSignal::updateSignal(int timeStep)
{
    elapsedTime = elapsedTime + timeStep;
}

SignalState TrafficSignal::getCurrentState() const
{
    return signalState;
}

Lane* TrafficSignal::getCurrentLane() const
{
    return activeLane;
}

int TrafficSignal::getMinGreenDuration() const
{
    return minGreenDuration;
}

int TrafficSignal::getMaxGreenDuration() const
{
    return maxGreenDuration;
}

int TrafficSignal::getElapsedTime() const
{
    return elapsedTime;
}

int TrafficSignal::clearVehicles(Lane* lane, int duration)
{
    bool validLane = lanePointerIsValid(lane);
    if (validLane == false)
    {
        return 0;
    }

    int maxClearable = duration / 2;
    int clearedCount = 0;

    cout << "  Clearing vehicles..." << endl;

    int iteration = 0;
    while (iteration < maxClearable)
    {
        bool laneStillHasVehicles = lane->isEmpty();
        if (laneStillHasVehicles == true)
        {
            break;
        }
        Vehicle departing = lane->removeVehicle();
        clearedCount = clearedCount + 1;

        bool wasEmergency = departing.isEmergency();
        if (wasEmergency == true)
        {
            cout << "  Emergency vehicle cleared." << endl;
        }
        iteration = iteration + 1;
    }

    string laneName = lane->getLaneName();
    int remaining = lane->getVehicleCount();
    cout << "  Cleared " << clearedCount << " vehicles from " << laneName << endl;
    cout << "  " << remaining << " vehicles still waiting." << endl;

    return clearedCount;
}

void TrafficSignal::displaySignalStatus() const
{
    cout << "\n+--------------------------------------+" << endl;
    cout << "|    TRAFFIC SIGNAL STATUS             |" << endl;
    cout << "+--------------------------------------+" << endl;

    bool hasActiveLane = lanePointerIsValid(activeLane);
    if (hasActiveLane == true)
    {
        string activeLaneName = activeLane->getLaneName();
        cout << "| Lane   : " << setw(27) << left << activeLaneName << " |" << endl;
    }
    else
    {
        cout << "| Lane   : " << setw(27) << left << "None" << " |" << endl;
    }

    string symbolPart = getSignalSymbol();
    string namePart = getStateName(signalState);
    string stateLabel = symbolPart + " " + namePart;
    string elapsedLabel = to_string(elapsedTime) + "s";

    cout << "| State  : " << setw(27) << left << stateLabel << " |" << endl;
    cout << "| Elapsed: " << setw(25) << left << elapsedLabel << " |" << endl;
    cout << "+--------------------------------------+\n" << endl;
}
