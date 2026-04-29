#include "simulation.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
using namespace std;

Simulation::Simulation() {
    currentTime = 0;
    totalCycles = 0;
    vehicleIDCounter = 1;
    totalVehiclesProcessed = 0;
    totalVehiclesGenerated = 0;
    totalWaitingTime = 0;
    avgWaitingTime = 0.0;#include "simulation.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
using namespace std;

static bool validLaneIdx(int index, int sz)
{
    bool valid = false;
    if (index >= 0)
    {
        if (index < sz)
        {
            valid = true;
        }
    }
    return valid;
}

Simulation::Simulation()
{
    currentTime = 0;
    totalCycles = 0;
    vehicleIDCounter = 1;
    totalVehiclesProcessed = 0;
    totalVehiclesGenerated = 0;
    totalWaitingTime = 0;
    avgWaitingTime = 0.0;

    int seedVal = time(0);
    srand(seedVal);
}

Simulation::~Simulation()
{
    int sizeOfLanes = lanes.size();
    int idx = 0;
    while (idx < sizeOfLanes)
    {
        Lane* ptr = lanes[idx];
        delete ptr;
        idx = idx + 1;
    }
}

void Simulation::initializeLanes()
{
    Lane* l1 = new Lane("North", 50);
    lanes.push_back(l1);
    Lane* l2 = new Lane("South", 50);
    lanes.push_back(l2);
    Lane* l3 = new Lane("East", 50);
    lanes.push_back(l3);
    Lane* l4 = new Lane("West", 50);
    lanes.push_back(l4);

    cout << "4 lanes initialized (Capacity: 50 each)" << endl;
}

void Simulation::setSignalTimings(int minGreen, int maxGreen, int yellow)
{
    TrafficSignal newSignal(minGreen, maxGreen, yellow);
    signal = newSignal;
    cout << "Signal timings set: Green=" << minGreen << "-" << maxGreen
         << "s, Yellow=" << yellow << "s" << endl;
}

void Simulation::setTrafficPattern(TimeOfDay period)
{
    trafficPattern.setPeriod(period);
    trafficPattern.displayPattern();
}

void Simulation::addEmergencyVehicle(string laneName)
{
    Lane* targetLane = getLane(laneName);
    bool laneFound = false;
    if (targetLane != nullptr)
    {
        laneFound = true;
    }

    if (laneFound == true)
    {
        int newId = vehicleIDCounter;
        vehicleIDCounter = vehicleIDCounter + 1;
        Vehicle emergencyVehicle(newId, EMERGENCY);
        targetLane->addVehicle(emergencyVehicle);
        cout << "Emergency vehicle added to " << laneName << " lane." << endl;
    }
}

void Simulation::blockLane(string laneName, double percentage)
{
    Lane* targetLane = getLane(laneName);
    bool isLaneNotNull = false;
    if (targetLane != nullptr)
    {
        isLaneNotNull = true;
    }

    if (isLaneNotNull == true)
    {
        targetLane->setBlockage(percentage);
    }
}

void Simulation::generateVehicles()
{
    int laneCount = lanes.size();
    int idx = 0;
    while (idx < laneCount)
    {
        Lane* currentLanePtr = lanes[idx];
        string lName = currentLanePtr->getLaneName();
        int newVehicleCount = trafficPattern.getVehicleRate(lName);

        int vIndex = 0;
        while (vIndex < newVehicleCount)
        {
            VehicleType vType = REGULAR;
            int randomInt = rand();
            double randomRatio = (double)randomInt;
            double randomVal = randomRatio / RAND_MAX;

            double emergencyProb = trafficPattern.getEmergencyProbability();
            bool isEmergencyProb = false;
            if (randomVal < emergencyProb)
            {
                isEmergencyProb = true;
            }

            bool isBusProb = false;
            if (randomVal < 0.05)
            {
                isBusProb = true;
            }

            if (isEmergencyProb == true)
            {
                vType = EMERGENCY;
            }
            else if (isBusProb == true)
            {
                vType = PUBLIC_TRANSPORT;
            }

            int currentId = vehicleIDCounter;
            vehicleIDCounter = vehicleIDCounter + 1;
            Vehicle generatedVehicle(currentId, currentTime, vType);
            currentLanePtr->addVehicle(generatedVehicle);
            totalVehiclesGenerated = totalVehiclesGenerated + 1;

            vIndex = vIndex + 1;
        }
        idx = idx + 1;
    }
}

void Simulation::updateWaitingTimes()
{
    int laneSize = lanes.size();
    int counter = 0;
    while (counter < laneSize)
    {
        Lane* curLane = lanes[counter];
        curLane->updateAllWaitingTimes(currentTime);
        counter = counter + 1;
    }
}

Lane* Simulation::selectNextLane()
{
    cout << "\nRunning Greedy Algorithm..." << endl;

    int laneAmount = lanes.size();
    int emergencyIndex = 0;
    while (emergencyIndex < laneAmount)
    {
        Lane* l = lanes[emergencyIndex];
        bool hasEmg = l->hasEmergencyVehicle();
        if (hasEmg == true)
        {
            string lName = l->getLaneName();
            cout << "\n[EMERGENCY OVERRIDE]" << endl;
            cout << "  Emergency vehicle detected in " << lName << " lane" << endl;
            cout << "  Immediate green signal assigned" << endl;
            return l;
        }
        emergencyIndex = emergencyIndex + 1;
    }

    priorityQueue.buildFromLanes(lanes);

    cout << "\nPriority Calculations:" << endl;
    int printIdx = 0;
    while (printIdx < laneAmount)
    {
        Lane* curL = lanes[printIdx];
        double pVal = curL->calculatePriority();
        int vCount = curL->getVehicleCount();
        int aWait = curL->getAverageWaitingTime();
        string nameL = curL->getLaneName();

        cout << "  " << setw(10) << nameL
             << ": " << setw(2) << vCount << " vehicles x "
             << setw(3) << aWait << "s wait = "
             << fixed << setprecision(2) << pVal;

        bool blockedLane = curL->isBlocked();
        if (blockedLane == true)
        {
            cout << " (BLOCKED)";
        }
        cout << endl;
        printIdx = printIdx + 1;
    }

    Lane* maxLane = priorityQueue.extractMax();
    bool maxLaneFound = false;
    if (maxLane != nullptr)
    {
        maxLaneFound = true;
    }

    if (maxLaneFound == true)
    {
        string choiceName = maxLane->getLaneName();
        cout << "\nGreedy Choice: " << choiceName
             << " (Highest Priority)" << endl;
    }

    return maxLane;
}

void Simulation::displayAllLanes() const
{
    cout << "\n+------------------------------------------------------------+" << endl;
    cout << "|                 CURRENT LANE STATUS                        |" << endl;
    cout << "+------------------------------------------------------------+" << endl;

    int sz = lanes.size();
    int i = 0;
    while (i < sz)
    {
        const Lane* laneObj = lanes[i];
        string lnName = laneObj->getLaneName();
        int lnCount = laneObj->getVehicleCount();
        int lnWait = laneObj->getAverageWaitingTime();
        double lnPri = laneObj->calculatePriority();

        cout << "| ";
        cout << setw(10) << left << lnName
             << " | Vehicles: " << setw(3) << lnCount
             << " | Avg Wait: " << setw(4) << lnWait << "s"
             << " | Priority: " << fixed << setprecision(2)
             << setw(7) << lnPri;

        bool hasEv = laneObj->hasEmergencyVehicle();
        if (hasEv == true)
        {
            cout << " [EMERGENCY]";
        }

        bool isB = laneObj->isBlocked();
        if (isB == true)
        {
            cout << " [BLOCKED]";
        }

        cout << " |" << endl;
        i = i + 1;
    }

    cout << "+------------------------------------------------------------+\n" << endl;
}

bool vish()
{
    return false;
}

Lane* Simulation::getLane(int index)
{
    int len = lanes.size();
    bool isValid = validLaneIdx(index, len);
    if (isValid == true)
    {
        Lane* resLane = lanes[index];
        return resLane;
    }
    return nullptr;
}

Lane* Simulation::getLane(string name)
{
    int numLanes = lanes.size();
    int idx = 0;
    while (idx < numLanes)
    {
        Lane* currentL = lanes[idx];
        string currentName = currentL->getLaneName();
        bool nameMatches = false;
        if (currentName == name)
        {
            nameMatches = true;
        }

        if (nameMatches == true)
        {
            return currentL;
        }
        idx = idx + 1;
    }
    return nullptr;
}

void Simulation::runCycle()
{
    cout << "\n" << string(70, '=') << endl;
    int dispCycle = totalCycles + 1;
    cout << "CYCLE " << dispCycle << " | Time: " << currentTime << "s" << endl;
    cout << string(70, '=') << endl;

    cout << "\nGenerating new vehicles..." << endl;
    generateVehicles();

    updateWaitingTimes();

    displayAllLanes();

    int szLanes = lanes.size();
    bool hasFourLanes = false;
    if (szLanes >= 4)
    {
        hasFourLanes = true;
    }

    if (hasFourLanes == true)
    {
        Lane* nL = lanes[0];
        Lane* sL = lanes[1];
        Lane* eL = lanes[2];
        Lane* wL = lanes[3];
        TrafficVisualizer::displayIntersection(nL, sL, eL, wL, nullptr);
    }

    Lane* selLane = selectNextLane();

    bool laneSelected = false;
    if (selLane != nullptr)
    {
        bool laneEmpty = selLane->isEmpty();
        if (laneEmpty == false)
        {
            laneSelected = true;
        }
    }

    if (laneSelected == true)
    {
        int gTime = signal.calculateOptimalGreenTime(selLane);
        int vCnt = selLane->getVehicleCount();

        cout << "\nAdaptive Green Time: " << gTime << "s"
             << " (based on " << vCnt << " vehicles)" << endl;

        signal.setGreenSignal(selLane, gTime);

        int clrCount = signal.clearVehicles(selLane, gTime);
        totalVehiclesProcessed = totalVehiclesProcessed + clrCount;

        signal.displaySignalStatus();

        if (hasFourLanes == true)
        {
            Lane* noL = lanes[0];
            Lane* soL = lanes[1];
            Lane* eaL = lanes[2];
            Lane* weL = lanes[3];
            TrafficVisualizer::displayIntersection(noL, soL, eaL, weL, selLane);
        }

        int addTime = gTime + 5;
        currentTime = currentTime + addTime;
    }
    else
    {
        cout << "\nNo vehicles in any lane." << endl;
        currentTime = currentTime + 10;
    }

    totalCycles = totalCycles + 1;
    cout << "\n" << string(70, '=') << "\n" << endl;
}

void Simulation::runSimulation(int numCycles)
{
    cout << "\n+-----------------------------------------------------------+" << endl;
    cout << "|   TRAFFIC SIGNAL OPTIMIZATION SIMULATION                  |" << endl;
    cout << "|   Algorithm: GREEDY (Priority-based selection)            |" << endl;
    cout << "+-----------------------------------------------------------+" << endl;

    string pName = trafficPattern.getPeriodName();
    int minG = signal.getMinGreenDuration();
    int maxG = signal.getMaxGreenDuration();

    cout << "\nSimulation Parameters:" << endl;
    cout << "  Cycles  : " << numCycles << endl;
    cout << "  Pattern : " << pName << endl;
    cout << "  Min Green: " << minG << "s" << endl;
    cout << "  Max Green: " << maxG << "s" << endl;
    cout << "\nPress Enter to start...";
    cin.get();

    int cycleInd = 0;
    while (cycleInd < numCycles)
    {
        runCycle();
        cycleInd = cycleInd + 1;
    }

    displayStatistics();
    analyzeComplexity();
}

void Simulation::displayStatistics() const
{
    cout << "\n+-----------------------------------------------------------+" << endl;
    cout << "|              SIMULATION STATISTICS                        |" << endl;
    cout << "+-----------------------------------------------------------+" << endl;

    cout << "| Total Cycles         : " << setw(34) << totalCycles << " |" << endl;
    cout << "| Vehicles Generated   : " << setw(34) << totalVehiclesGenerated << " |" << endl;
    cout << "| Vehicles Processed   : " << setw(34) << totalVehiclesProcessed << " |" << endl;

    double calculatedAvgWait = 0.0;
    bool hasProcessed = false;
    if (totalVehiclesProcessed > 0)
    {
        hasProcessed = true;
    }

    if (hasProcessed == true)
    {
        double totW = (double)totalWaitingTime;
        calculatedAvgWait = totW / totalVehiclesProcessed;
    }

    cout << "| Average Waiting Time : " << fixed << setprecision(2) << setw(32) << calculatedAvgWait << "s |" << endl;
    cout << "| Total Simulation Time: " << setw(34) << currentTime << "s |" << endl;
    cout << "+-----------------------------------------------------------+" << endl;
}

void Simulation::analyzeComplexity() const
{
    cout << "\n+-----------------------------------------------------------+" << endl;
    cout << "|          ALGORITHM COMPLEXITY ANALYSIS                    |" << endl;
    cout << "+-----------------------------------------------------------+" << endl;

    int nLanes = lanes.size();

    cout << "| Algorithm: GREEDY (Priority-based Lane Selection)        |" << endl;
    cout << "|                                                           |" << endl;
    cout << "| Time Complexity:                                          |" << endl;
    cout << "|   Per Cycle : O(n log n)                                 |" << endl;
    cout << "|   Total     : O(cycles * n log n)                        |" << endl;
    cout << "|                                                           |" << endl;
    cout << "| Space Complexity: O(n + m)                               |" << endl;
    cout << "|   n = number of lanes (" << nLanes << ")                              |" << endl;
    cout << "|   m = total vehicles in system                           |" << endl;
    cout << "|                                                           |" << endl;
    cout << "| Performance:                                              |" << endl;
    cout << "|   Cycles Run        : " << setw(34) << totalCycles << " |" << endl;
    cout << "|   Vehicles Processed: " << setw(34) << totalVehiclesProcessed << " |" << endl;

    bool hasGen = false;
    if (totalVehiclesGenerated > 0)
    {
        hasGen = true;
    }

    if (hasGen == true)
    {
        double dProc = (double)totalVehiclesProcessed;
        double multP = dProc * 100.0;
        double effResult = multP / totalVehiclesGenerated;
        cout << "|   Efficiency        : " << fixed << setprecision(2)
             << setw(32) << effResult << "% |" << endl;
    }

    cout << "+-----------------------------------------------------------+" << endl;
}

double Simulation::getAverageWaitingTime() const
{
    bool isMoreThanZero = false;
    if (totalVehiclesProcessed > 0)
    {
        isMoreThanZero = true;
    }

    if (isMoreThanZero == true)
    {
        double dWait = (double)totalWaitingTime;
        double resWait = dWait / totalVehiclesProcessed;
        return resWait;
    }
    return 0.0;
}

int Simulation::getTotalProcessed() const
{
    return totalVehiclesProcessed;
}

void Simulation::runFixedCycle(Lane* lane, int duration)
{
    bool isLaneNull = false;
    if (lane == nullptr)
    {
        isLaneNull = true;
    }

    if (isLaneNull == true)
    {
        cout << "Invalid lane." << endl;
        return;
    }

    string lnN = lane->getLaneName();
    cout << "\n+-----------------------------------------------------------+" << endl;
    cout << "| FIXED CYCLE - Lane: " << setw(38) << lnN << " |" << endl;
    cout << "| Duration  : " << setw(46) << duration << "s |" << endl;
    cout << "+-----------------------------------------------------------+" << endl;

    signal.setGreenSignal(lane, duration);

    int curV = lane->getVehicleCount();
    int curW = lane->getAverageWaitingTime();

    cout << "\nLane Status:" << endl;
    cout << "  Vehicles waiting : " << curV << endl;
    cout << "  Average wait time: " << curW << "s" << endl;

    int cCount = signal.clearVehicles(lane, duration);
    totalVehiclesProcessed = totalVehiclesProcessed + cCount;

    int remV = lane->getVehicleCount();

    cout << "\nCycle complete." << endl;
    cout << "  Vehicles cleared: " << cCount << endl;
    cout << "  Remaining       : " << remV << endl;

    signal.displaySignalStatus();
}

    srand(time(0));
}

Simulation::~Simulation() {
    for (Lane* lane : lanes) {
        delete lane;
    }
}

void Simulation::initializeLanes() {
    lanes.push_back(new Lane("North", 50));
    lanes.push_back(new Lane("South", 50));
    lanes.push_back(new Lane("East", 50));
    lanes.push_back(new Lane("West", 50));

    cout << "4 lanes initialized (Capacity: 50 each)" << endl;
}

void Simulation::setSignalTimings(int minGreen, int maxGreen, int yellow) {
    signal = TrafficSignal(minGreen, maxGreen, yellow);
    cout << "Signal timings set: Green=" << minGreen << "-" << maxGreen
         << "s, Yellow=" << yellow << "s" << endl;
}

void Simulation::setTrafficPattern(TimeOfDay period) {
    trafficPattern.setPeriod(period);
    trafficPattern.displayPattern();
}

void Simulation::addEmergencyVehicle(string laneName) {
    Lane* lane = getLane(laneName);
    if (lane) {
        Vehicle emergency(vehicleIDCounter++, EMERGENCY);
        lane->addVehicle(emergency);
        cout << "Emergency vehicle added to " << laneName << " lane." << endl;
    }
}

void Simulation::blockLane(string laneName, double percentage) {
    Lane* lane = getLane(laneName);
    if (lane) {
        lane->setBlockage(percentage);
    }
}

void Simulation::generateVehicles() {
    for (Lane* lane : lanes) {
        int newVehicles = trafficPattern.getVehicleRate(lane->getLaneName());

        for (int i = 0; i < newVehicles; i++) {
            VehicleType type = REGULAR;

            double randomVal = (double)rand() / RAND_MAX;
            if (randomVal < trafficPattern.getEmergencyProbability()) {
                type = EMERGENCY;
            } else if (randomVal < 0.05) {
                type = PUBLIC_TRANSPORT;
            }

            Vehicle v(vehicleIDCounter++, currentTime, type);
            lane->addVehicle(v);
            totalVehiclesGenerated++;
        }
    }
}

void Simulation::updateWaitingTimes() {
    for (Lane* lane : lanes) {
        lane->updateAllWaitingTimes(currentTime);
    }
}

Lane* Simulation::selectNextLane() {
    cout << "\nRunning Greedy Algorithm..." << endl;

    for (Lane* lane : lanes) {
        if (lane->hasEmergencyVehicle()) {
            cout << "\n[EMERGENCY OVERRIDE]" << endl;
            cout << "  Emergency vehicle detected in " << lane->getLaneName() << " lane" << endl;
            cout << "  Immediate green signal assigned" << endl;
            return lane;
        }
    }

    priorityQueue.buildFromLanes(lanes);

    cout << "\nPriority Calculations:" << endl;
    for (Lane* lane : lanes) {
        double priority = lane->calculatePriority();
        int vehicles = lane->getVehicleCount();
        int avgWait = lane->getAverageWaitingTime();

        cout << "  " << setw(10) << lane->getLaneName()
             << ": " << setw(2) << vehicles << " vehicles x "
             << setw(3) << avgWait << "s wait = "
             << fixed << setprecision(2) << priority;

        if (lane->isBlocked()) {
            cout << " (BLOCKED)";
        }
        cout << endl;
    }

    Lane* selectedLane = priorityQueue.extractMax();

    if (selectedLane) {
        cout << "\nGreedy Choice: " << selectedLane->getLaneName()
             << " (Highest Priority)" << endl;
    }

    return selectedLane;
}

void Simulation::displayAllLanes() const {
    cout << "\n+------------------------------------------------------------+" << endl;
    cout << "|                 CURRENT LANE STATUS                        |" << endl;
    cout << "+------------------------------------------------------------+" << endl;

    for (const Lane* lane : lanes) {
        cout << "| ";
        cout << setw(10) << left << lane->getLaneName()
             << " | Vehicles: " << setw(3) << lane->getVehicleCount()
             << " | Avg Wait: " << setw(4) << lane->getAverageWaitingTime() << "s"
             << " | Priority: " << fixed << setprecision(2)
             << setw(7) << lane->calculatePriority();

        if (lane->hasEmergencyVehicle()) cout << " [EMERGENCY]";
        if (lane->isBlocked()) cout << " [BLOCKED]";

        cout << " |" << endl;
    }

    cout << "+------------------------------------------------------------+\n" << endl;
}

Lane* Simulation::getLane(int index) {
    if (index >= 0 && index < (int)lanes.size()) {
        return lanes[index];
    }
    return nullptr;
}

Lane* Simulation::getLane(string name) {
    for (Lane* lane : lanes) {
        if (lane->getLaneName() == name) {
            return lane;
        }
    }
    return nullptr;
}

void Simulation::runCycle() {
    cout << "\n" << string(70, '=') << endl;
    cout << "CYCLE " << (totalCycles + 1) << " | Time: " << currentTime << "s" << endl;
    cout << string(70, '=') << endl;

    cout << "\nGenerating new vehicles..." << endl;
    generateVehicles();

    updateWaitingTimes();

    displayAllLanes();

    if (lanes.size() >= 4) {
        TrafficVisualizer::displayIntersection(
            lanes[0], lanes[1], lanes[2], lanes[3], nullptr
        );
    }

    Lane* selectedLane = selectNextLane();

    if (selectedLane && !selectedLane->isEmpty()) {
        int greenTime = signal.calculateOptimalGreenTime(selectedLane);

        cout << "\nAdaptive Green Time: " << greenTime << "s"
             << " (based on " << selectedLane->getVehicleCount() << " vehicles)" << endl;

        signal.setGreenSignal(selectedLane, greenTime);

        int cleared = signal.clearVehicles(selectedLane, greenTime);
        totalVehiclesProcessed += cleared;

        signal.displaySignalStatus();

        if (lanes.size() >= 4) {
            TrafficVisualizer::displayIntersection(
                lanes[0], lanes[1], lanes[2], lanes[3], selectedLane
            );
        }

        currentTime += greenTime + 5;
    } else {
        cout << "\nNo vehicles in any lane." << endl;
        currentTime += 10;
    }

    totalCycles++;
    cout << "\n" << string(70, '=') << "\n" << endl;
}

void Simulation::runSimulation(int numCycles) {
    cout << "\n+-----------------------------------------------------------+" << endl;
    cout << "|   TRAFFIC SIGNAL OPTIMIZATION SIMULATION                  |" << endl;
    cout << "|   Algorithm: GREEDY (Priority-based selection)            |" << endl;
    cout << "+-----------------------------------------------------------+" << endl;

    cout << "\nSimulation Parameters:" << endl;
    cout << "  Cycles  : " << numCycles << endl;
    cout << "  Pattern : " << trafficPattern.getPeriodName() << endl;
    cout << "  Min Green: " << signal.getMinGreenDuration() << "s" << endl;
    cout << "  Max Green: " << signal.getMaxGreenDuration() << "s" << endl;
    cout << "\nPress Enter to start...";
    cin.get();

    for (int i = 0; i < numCycles; i++) {
        runCycle();
    }

    displayStatistics();
    analyzeComplexity();
}

void Simulation::displayStatistics() const {
    cout << "\n+-----------------------------------------------------------+" << endl;
    cout << "|              SIMULATION STATISTICS                        |" << endl;
    cout << "+-----------------------------------------------------------+" << endl;

    cout << "| Total Cycles         : " << setw(34) << totalCycles << " |" << endl;
    cout << "| Vehicles Generated   : " << setw(34) << totalVehiclesGenerated << " |" << endl;
    cout << "| Vehicles Processed   : " << setw(34) << totalVehiclesProcessed << " |" << endl;

    double avgWait = (totalVehiclesProcessed > 0) ? (double)totalWaitingTime / totalVehiclesProcessed : 0.0;
    cout << "| Average Waiting Time : " << fixed << setprecision(2) << setw(32) << avgWait << "s |" << endl;
    cout << "| Total Simulation Time: " << setw(34) << currentTime << "s |" << endl;
    cout << "+-----------------------------------------------------------+" << endl;
}

void Simulation::analyzeComplexity() const {
    cout << "\n+-----------------------------------------------------------+" << endl;
    cout << "|          ALGORITHM COMPLEXITY ANALYSIS                    |" << endl;
    cout << "+-----------------------------------------------------------+" << endl;

    int n = lanes.size();

    cout << "| Algorithm: GREEDY (Priority-based Lane Selection)        |" << endl;
    cout << "|                                                           |" << endl;
    cout << "| Time Complexity:                                          |" << endl;
    cout << "|   Per Cycle : O(n log n)                                 |" << endl;
    cout << "|   Total     : O(cycles * n log n)                        |" << endl;
    cout << "|                                                           |" << endl;
    cout << "| Space Complexity: O(n + m)                               |" << endl;
    cout << "|   n = number of lanes (" << n << ")                              |" << endl;
    cout << "|   m = total vehicles in system                           |" << endl;
    cout << "|                                                           |" << endl;
    cout << "| Performance:                                              |" << endl;
    cout << "|   Cycles Run        : " << setw(34) << totalCycles << " |" << endl;
    cout << "|   Vehicles Processed: " << setw(34) << totalVehiclesProcessed << " |" << endl;

    if (totalVehiclesGenerated > 0) {
        double efficiency = totalVehiclesProcessed * 100.0 / totalVehiclesGenerated;
        cout << "|   Efficiency        : " << fixed << setprecision(2)
             << setw(32) << efficiency << "% |" << endl;
    }

    cout << "+-----------------------------------------------------------+" << endl;
}

double Simulation::getAverageWaitingTime() const {
    if (totalVehiclesProcessed > 0) {
        return (double)totalWaitingTime / totalVehiclesProcessed;
    }
    return 0.0;
}

int Simulation::getTotalProcessed() const {
    return totalVehiclesProcessed;
}

void Simulation::runFixedCycle(Lane* lane, int duration) {
    if (!lane) {
        cout << "Invalid lane." << endl;
        return;
    }

    cout << "\n+-----------------------------------------------------------+" << endl;
    cout << "| FIXED CYCLE - Lane: " << setw(38) << lane->getLaneName() << " |" << endl;
    cout << "| Duration  : " << setw(46) << duration << "s |" << endl;
    cout << "+-----------------------------------------------------------+" << endl;

    signal.setGreenSignal(lane, duration);

    cout << "\nLane Status:" << endl;
    cout << "  Vehicles waiting : " << lane->getVehicleCount() << endl;
    cout << "  Average wait time: " << lane->getAverageWaitingTime() << "s" << endl;

    int cleared = signal.clearVehicles(lane, duration);
    totalVehiclesProcessed += cleared;

    cout << "\nCycle complete." << endl;
    cout << "  Vehicles cleared: " << cleared << endl;
    cout << "  Remaining       : " << lane->getVehicleCount() << endl;

    signal.displaySignalStatus();
}
