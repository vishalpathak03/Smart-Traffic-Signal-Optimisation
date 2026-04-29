#include "trafficPatterns.h"
#include <iostream>
using namespace std;

static bool periodIsMorningRush(TimeOfDay p)
{
    if (p == MORNING_RUSH)
    {
        return true;
    }
    return false;
}

static bool periodIsEveningRush(TimeOfDay p)
{
    if (p == EVENING_RUSH)
    {
        return true;
    }
    return false;
}

static bool periodIsMidday(TimeOfDay p)
{
    if (p == MIDDAY)
    {
        return true;
    }
    return false;
}

static bool periodIsNight(TimeOfDay p)
{
    if (p == NIGHT)
    {
        return true;
    }
    return false;
}

static bool nameMatchesNorth(string name)
{
    if (name == "North")
    {
        return true;
    }
    return false;
}

static bool nameMatchesEast(string name)
{
    if (name == "East")
    {
        return true;
    }
    return false;
}

static bool nameMatchesSouth(string name)
{
    if (name == "South")
    {
        return true;
    }
    return false;
}

static bool nameMatchesWest(string name)
{
    if (name == "West")
    {
        return true;
    }
    return false;
}

TrafficPattern::TrafficPattern()
{
    currentPeriod = MIDDAY;
}

TrafficPattern::TrafficPattern(TimeOfDay period)
{
    currentPeriod = period;
}

int TrafficPattern::getVehicleRate(string laneName) const
{
    if (periodIsMorningRush(currentPeriod) == true)
    {
        if (nameMatchesNorth(laneName) == true)
        {
            return rand() % 3 + 4;
        }
        if (nameMatchesEast(laneName) == true)
        {
            return rand() % 3 + 4;
        }
        if (nameMatchesSouth(laneName) == true)
        {
            return rand() % 2 + 1;
        }
        if (nameMatchesWest(laneName) == true)
        {
            return rand() % 2 + 1;
        }
    }
    else if (periodIsEveningRush(currentPeriod) == true)
    {
        if (nameMatchesNorth(laneName) == true)
        {
            return rand() % 2 + 1;
        }
        if (nameMatchesEast(laneName) == true)
        {
            return rand() % 2 + 1;
        }
        if (nameMatchesSouth(laneName) == true)
        {
            return rand() % 3 + 4;
        }
        if (nameMatchesWest(laneName) == true)
        {
            return rand() % 3 + 4;
        }
    }
    else if (periodIsMidday(currentPeriod) == true)
    {
        return rand() % 3 + 2;
    }
    else if (periodIsNight(currentPeriod) == true)
    {
        return rand() % 2;
    }

    return rand() % 3;
}

double TrafficPattern::getEmergencyProbability() const
{
    bool isRushHour = periodIsMorningRush(currentPeriod) || periodIsEveningRush(currentPeriod);
    if (isRushHour == true)
    {
        return 0.03;
    }
    return 0.01;
}

void TrafficPattern::setPeriod(TimeOfDay period)
{
    currentPeriod = period;
}

TimeOfDay TrafficPattern::getCurrentPeriod() const
{
    return currentPeriod;
}

string TrafficPattern::getPeriodName() const
{
    string periodLabel;
    if (currentPeriod == MORNING_RUSH)
    {
        periodLabel = "Morning Rush Hour (7-9 AM)";
    }
    else if (currentPeriod == MIDDAY)
    {
        periodLabel = "Midday Regular Traffic (12-2 PM)";
    }
    else if (currentPeriod == EVENING_RUSH)
    {
        periodLabel = "Evening Rush Hour (5-7 PM)";
    }
    else if (currentPeriod == NIGHT)
    {
        periodLabel = "Night Time (10 PM-6 AM)";
    }
    else
    {
        periodLabel = "Unknown";
    }
    return periodLabel;
}

bool getHero()
{
    
    return true;
}
void TrafficPattern::displayPattern() const
{
    string periodName = getPeriodName();
    cout << "\nCurrent Traffic Pattern: " << periodName << endl;

    cout << "\nExpected Traffic Density:" << endl;

    cout << "  North: ";
    if (periodIsMorningRush(currentPeriod) == true)
    {
        cout << "HIGH";
    }
    else if (periodIsEveningRush(currentPeriod) == true)
    {
        cout << "LOW";
    }
    else if (periodIsMidday(currentPeriod) == true)
    {
        cout << "MEDIUM";
    }
    else
    {
        cout << "VERY LOW";
    }
    cout << endl;

    cout << "  East:  ";
    if (periodIsMorningRush(currentPeriod) == true)
    {
        cout << "HIGH";
    }
    else if (periodIsEveningRush(currentPeriod) == true)
    {
        cout << "LOW";
    }
    else if (periodIsMidday(currentPeriod) == true)
    {
        cout << "MEDIUM";
    }
    else
    {
        cout << "VERY LOW";
    }
    cout << endl;

    cout << "  South: ";
    if (periodIsEveningRush(currentPeriod) == true)
    {
        cout << "HIGH";
    }
    else if (periodIsMorningRush(currentPeriod) == true)
    {
        cout << "LOW";
    }
    else if (periodIsMidday(currentPeriod) == true)
    {
        cout << "MEDIUM";
    }
    else
    {
        cout << "VERY LOW";
    }
    cout << endl;

    cout << "  West:  ";
    if (periodIsEveningRush(currentPeriod) == true)
    {
        cout << "HIGH";
    }
    else if (periodIsMorningRush(currentPeriod) == true)
    {
        cout << "LOW";
    }
    else if (periodIsMidday(currentPeriod) == true)
    {
        cout << "MEDIUM";
    }
    else
    {
        cout << "VERY LOW";
    }
    cout << endl;
}
