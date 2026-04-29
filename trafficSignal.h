#ifndef TRAFFIC_SIGNAL_H
#define TRAFFIC_SIGNAL_H

#include "lane.h"
#include <string>
using namespace std;

enum SignalState {
    RED,
    YELLOW,
    GREEN
};

class TrafficSignal {
private:
    Lane* currentLane;
    SignalState currentState;
    int minGreenDuration;
    int maxGreenDuration;
    int yellowDuration;
    int redDuration;
    int elapsedTime;

    string getStateName(SignalState state) const;

public:
    TrafficSignal();
    TrafficSignal(int minGreen, int maxGreen, int yellow);

    void setGreenSignal(Lane* lane, int duration);
    void setYellowSignal();
    void setRedSignal();
    void updateSignal(int timeStep);

    int calculateOptimalGreenTime(Lane* lane) const;

    SignalState getCurrentState() const;
    Lane* getCurrentLane() const;
    int getMinGreenDuration() const;
    int getMaxGreenDuration() const;
    int getElapsedTime() const;

    int clearVehicles(Lane* lane, int duration);

    void displaySignalStatus() const;
    string getSignalSymbol() const;
};

#endif
