#include "lane.h"
#include <bits/stdc++.h>
using namespace std;

static bool laneQueueIsEmpty(const queue<Vehicle>& q)
{
    if (q.empty() == true)
    {
        return true;
    }
    return false;
}

Lane::Lane(string name, int cap)
{
    laneName = name;
    capacity = cap;
    totalVehicles = 0;
    accumulatedWaitTime = 0;
    blocked = false;
    blockagePercent = 0.0;
}

void Lane::addVehicle(const Vehicle& v)
{
    bool laneAtCapacity = isFull();
    if (laneAtCapacity == true)
    {
        cout << "Lane " << laneName << " is full. Cannot add vehicle " << v.getVehicleID() << endl;
        return;
    }
    vehicleQueue.push(v);
    totalVehicles = totalVehicles + 1;
}

Vehicle Lane::removeVehicle()
{
    bool laneNotEmpty = isEmpty();
    if (laneNotEmpty == false)
    {
        Vehicle frontVehicle = vehicleQueue.front();
        vehicleQueue.pop();
        totalVehicles = totalVehicles - 1;
        return frontVehicle;
    }
    else
    {
        throw runtime_error("Lane " + laneName + " is empty. Cannot remove vehicle.");
    }
}

int Lane::getVehicleCount() const
{
    int count = vehicleQueue.size();
    return count;
}

bool Lane::hasEmergencyVehicle() const
{
    queue<Vehicle> scanQueue = vehicleQueue;
    while (scanQueue.empty() == false)
    {
        bool frontIsEmergency = scanQueue.front().isEmergency();
        if (frontIsEmergency == true)
        {
            return true;
        }
        scanQueue.pop();
    }
    return false;
}

void Lane::updateAllWaitingTimes(int currentTime)
{
    int presentCount = getVehicleCount();
    accumulatedWaitTime = accumulatedWaitTime + presentCount;
}

int Lane::getTotalWaitingTime() const
{
    return accumulatedWaitTime;
}

int Lane::getAverageWaitingTime() const
{
    if (totalVehicles == 0)
    {
        return 0;
    }
    int avgWait = accumulatedWaitTime / totalVehicles;
    return avgWait;
}

double Lane::calculatePriority() const
{
    int vehicleCount = getVehicleCount();
    int avgWaitingTime = getAverageWaitingTime();
    int effectiveCapacity = getEffectiveCapacity();

    if (effectiveCapacity == 0)
    {
        return 0.0;
    }

    bool emergencyPresent = hasEmergencyVehicle();
    if (emergencyPresent == true)
    {
        return 999999.0;
    }

    double numerator = (double)(vehicleCount * avgWaitingTime);
    double priorityScore = numerator / effectiveCapacity;
    return priorityScore;
}

void Lane::setBlockage(double percent)
{
    if (percent < 0.0)
    {
        percent = 0.0;
    }
    if (percent > 1.0)
    {
        percent = 1.0;
    }
    blockagePercent = percent;
    if (percent > 0.0)
    {
        blocked = true;
    }
    else
    {
        blocked = false;
    }
}

bool Lane::isBlocked() const
{
    return blocked;
}

int Lane::getEffectiveCapacity() const
{
    double reducedRatio = 1.0 - blockagePercent;
    int effectiveCap = static_cast<int>(capacity * reducedRatio);
    int result = std::max(0, effectiveCap);
    return result;
}

string Lane::getLaneName() const
{
    return laneName;
}

void Lane::displayLaneStatus() const
{
    string blockedLabel;
    if (blocked == true)
    {
        blockedLabel = "Blocked";
    }
    else
    {
        blockedLabel = "Open";
    }
    int currentCount = getVehicleCount();
    int effectiveCap = getEffectiveCapacity();
    int avgWait = getAverageWaitingTime();
    cout << "Lane " << laneName << " status: " << blockedLabel
         << ", vehicles=" << currentCount
         << ", effective capacity=" << effectiveCap
         << ", avg waiting=" << avgWait << endl;
}

bool Lane::isEmpty() const
{
    bool result = vehicleQueue.empty();
    return result;
}

bool Lane::isFull() const
{
    int presentCount = getVehicleCount();
    int effectiveCap = getEffectiveCapacity();
    if (presentCount >= effectiveCap)
    {
        return true;
    }
    return false;
}
