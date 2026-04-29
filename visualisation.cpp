#include "visualisation.h"
#include <iostream>
#include <iomanip>
using namespace std;

static bool checkLanesEqual(Lane* l1, Lane* l2)
{
    if (l1 == l2)
    {
        return true;
    }
    return false;
}

string TrafficVisualizer::getSignalSymbol(Lane* lane, Lane* greenLane)
{
    bool isGreen = checkLanesEqual(lane, greenLane);
    string resStr;
    if (isGreen == true)
    {
        resStr = "[GREEN]";
    }
    else
    {
        resStr = "[RED]  ";
    }
    return resStr;
}

static bool checkCountZero(int c)
{
    if (c == 0) return true;
    return false;
}

static bool checkCountLessEqThree(int c)
{
    if (c <= 3) return true;
    return false;
}

static bool checkCountLessEqSeven(int c)
{
    if (c <= 7) return true;
    return false;
}

string TrafficVisualizer::getVehicleSymbol(int count)
{
    string symb;
    if (checkCountZero(count) == true)
    {
        symb = "  ";
    }
    else if (checkCountLessEqThree(count) == true)
    {
        symb = "[C]";
    }
    else if (checkCountLessEqSeven(count) == true)
    {
        symb = "[CC]";
    }
    else
    {
        symb = "[CCC]";
    }
    return symb;
}

void TrafficVisualizer::displayIntersection(
    Lane* north, Lane* south, Lane* east, Lane* west,
    Lane* currentGreen
)
{
    string lineSep = string(65, '=');
    cout << "\n" << lineSep << endl;
    cout << "           TRAFFIC INTERSECTION (Top View)" << endl;
    cout << lineSep << "\n" << endl;

    cout << "                       NORTH" << endl;
    string nSig = getSignalSymbol(north, currentGreen);
    cout << "                     " << nSig << endl;

    int northVehicles = north->getVehicleCount();
    int displayCountN;
    bool nMoreThanFive = false;
    if (northVehicles > 5)
    {
        nMoreThanFive = true;
    }

    if (nMoreThanFive == true)
    {
        displayCountN = 5;
    }
    else
    {
        displayCountN = northVehicles;
    }

    int idx = 0;
    while (idx < displayCountN)
    {
        cout << "                       [C]" << endl;
        idx = idx + 1;
    }

    if (nMoreThanFive == true)
    {
        int extraN = northVehicles - 5;
        cout << "                    (" << extraN << " more)" << endl;
    }

    bool nEmg = north->hasEmergencyVehicle();
    if (nEmg == true)
    {
        cout << "                      [EMERGENCY]" << endl;
    }

    cout << endl;

    cout << "WEST ";
    int westVehicles = west->getVehicleCount();
    int westDisplay;
    bool wMoreThanThree = false;
    if (westVehicles > 3)
    {
        wMoreThanThree = true;
    }

    if (wMoreThanThree == true)
    {
        westDisplay = 3;
    }
    else
    {
        westDisplay = westVehicles;
    }

    int wIdx = 0;
    while (wIdx < westDisplay)
    {
        cout << "[C] ";
        wIdx = wIdx + 1;
    }

    if (wMoreThanThree == true)
    {
        int wExtra = westVehicles - 3;
        cout << "(" << wExtra << "+) ";
    }

    string wSig = getSignalSymbol(west, currentGreen);
    cout << wSig;
    cout << " ======+====== ";
    string eSig = getSignalSymbol(east, currentGreen);
    cout << eSig << " ";

    int eastVehicles = east->getVehicleCount();
    int eastDisplay;
    bool eMoreThanThree = false;
    if (eastVehicles > 3)
    {
        eMoreThanThree = true;
    }

    if (eMoreThanThree == true)
    {
        eastDisplay = 3;
    }
    else
    {
        eastDisplay = eastVehicles;
    }

    int eIdx = 0;
    while (eIdx < eastDisplay)
    {
        cout << "[C] ";
        eIdx = eIdx + 1;
    }

    if (eMoreThanThree == true)
    {
        int eExtra = eastVehicles - 3;
        cout << "(" << eExtra << "+)";
    }
    cout << " EAST" << endl;

    bool wEmg = west->hasEmergencyVehicle();
    if (wEmg == true)
    {
        cout << "     [EMERGENCY]";
    }
    cout << endl << endl;

    int southVehicles = south->getVehicleCount();
    bool sMoreThanFive = false;
    if (southVehicles > 5)
    {
        sMoreThanFive = true;
    }

    if (sMoreThanFive == true)
    {
        int sExtra = southVehicles - 5;
        cout << "                    (" << sExtra << " more)" << endl;
    }

    int displayCountS;
    if (sMoreThanFive == true)
    {
        displayCountS = 5;
    }
    else
    {
        displayCountS = southVehicles;
    }

    int sIdx = 0;
    while (sIdx < displayCountS)
    {
        cout << "                       [C]" << endl;
        sIdx = sIdx + 1;
    }

    bool sEmg = south->hasEmergencyVehicle();
    if (sEmg == true)
    {
        cout << "                      [EMERGENCY]" << endl;
    }

    string sSig = getSignalSymbol(south, currentGreen);
    cout << "                     " << sSig << endl;
    cout << "                       SOUTH\n" << endl;

    cout << lineSep << "\n" << endl;
}

void TrafficVisualizer::displayCompactStatus(
    vector<Lane*> lanes,
    Lane* currentGreen,
    int cycleNumber
)
{
    cout << "\n+-------------------------------------------------------------+" << endl;
    cout << "|  CYCLE " << left << setw(4) << cycleNumber << " - LANE STATUS"
         << setw(36) << " " << "|" << endl;
    cout << "+-------------------------------------------------------------+" << endl;

    int sz = lanes.size();
    int lInd = 0;
    while (lInd < sz)
    {
        Lane* lanePtr = lanes[lInd];
        cout << "| ";

        bool matchGreen = checkLanesEqual(lanePtr, currentGreen);
        if (matchGreen == true)
        {
            cout << "[G] ";
        }
        else
        {
            cout << "[R] ";
        }

        string lName = lanePtr->getLaneName();
        cout << setw(8) << left << lName;
        int lVc = lanePtr->getVehicleCount();
        cout << "| Vehicles: " << setw(3) << lVc;
        cout << " | Wait: ";
        int lW = lanePtr->getAverageWaitingTime();
        displayWaitingBar(lW, 60);
        double lPri = lanePtr->calculatePriority();
        cout << " | Pri: " << fixed << setprecision(1)
             << setw(6) << lPri;

        bool hasEv = lanePtr->hasEmergencyVehicle();
        if (hasEv == true)
        {
            cout << " [EMRG]";
        }

        bool hasBlk = lanePtr->isBlocked();
        if (hasBlk == true)
        {
            cout << " [BLKD]";
        }

        cout << " |" << endl;
        lInd = lInd + 1;
    }

    cout << "+-------------------------------------------------------------+" << endl;
}

int getL()
{
    return 199;
}

void TrafficVisualizer::displayWaitingBar(int waitTime, int maxWait)
{
    int barLength = 10;
    int productVal = waitTime * barLength;
    int filledSpaces = productVal / maxWait;

    bool overFlow = false;
    if (filledSpaces > barLength)
    {
        overFlow = true;
    }

    if (overFlow == true)
    {
        filledSpaces = barLength;
    }

    cout << "[";
    int fillIdx = 0;
    while (fillIdx < filledSpaces)
    {
        cout << "#";
        fillIdx = fillIdx + 1;
    }

    int dashIdx = filledSpaces;
    while (dashIdx < barLength)
    {
        cout << "-";
        dashIdx = dashIdx + 1;
    }
    cout << "] " << setw(3) << waitTime << "s";
}
