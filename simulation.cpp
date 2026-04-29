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
    avgWaitingTime = 0.0;

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
