#include <iostream>
#include <limits>
#include "simulation.h"
#include "comparison.h"
using namespace std;

static void printMenuSeparator()
{
    cout << "+---------------------------------------------------+" << endl;
}

void displayMenu()
{
    cout << "\n";
    printMenuSeparator();
    cout << "|   TRAFFIC SIGNAL OPTIMIZATION SYSTEM              |" << endl;
    cout << "|   Using Greedy Algorithm (DAA Project)            |" << endl;
    printMenuSeparator();
    cout << "\n1. Run Quick Demo (5 cycles, Morning Rush)" << endl;
    cout << "2. Morning Rush Hour Simulation" << endl;
    cout << "3. Evening Rush Hour Simulation" << endl;
    cout << "4. Midday Regular Traffic" << endl;
    cout << "5. Night Time Traffic" << endl;
    cout << "6. Emergency Vehicle Scenario" << endl;
    cout << "7. Accident Simulation (Lane Blockage)" << endl;
    cout << "8. Compare with Fixed-Time Signals" << endl;
    cout << "9. Custom Simulation" << endl;
    cout << "10. Algorithm Information" << endl;
    cout << "11. Exit" << endl;
    cout << "\nEnter choice: ";
}

void displayAlgorithmInfo()
{
    string delimStr = string(65, '=');
    cout << "\n" << delimStr << endl;
    cout << "GREEDY ALGORITHM INFORMATION" << endl;
    cout << delimStr << endl;

    cout << "\nStrategy:" << endl;
    cout << "  At each cycle, select the lane with HIGHEST priority" << endl;
    cout << "  Priority = (Vehicles * Average Waiting Time) / Capacity" << endl;

    cout << "\nData Structures:" << endl;
    cout << "  - Priority Queue (Max-Heap) for lane selection" << endl;
    cout << "  - Queue (FIFO) for vehicle management per lane" << endl;

    cout << "\nComplexity:" << endl;
    cout << "  Time : O(n log n) per cycle" << endl;
    cout << "  Space: O(n + m), where n = lanes, m = total vehicles" << endl;

    cout << "\nGreedy Choice Property:" << endl;
    cout << "  - Locally optimal : choose highest priority lane now" << endl;
    cout << "  - Globally optimal: minimizes total waiting time" << endl;

    cout << "\nEmergency Override:" << endl;
    cout << "  - Emergency vehicles bypass the greedy selection" << endl;
    cout << "  - Safety is always given priority over optimization" << endl;

    cout << "\n" << delimStr << "\n" << endl;
}

void runQuickDemo()
{
    cout << "\nRunning Quick Demo (5 cycles, Morning Rush pattern)...\n" << endl;

    Simulation simObj;
    simObj.initializeLanes();
    simObj.setSignalTimings(15, 60, 5);
    simObj.setTrafficPattern(MORNING_RUSH);
    simObj.runSimulation(5);
}

void runMorningRush()
{
    cout << "\nMorning Rush Hour Simulation (7-9 AM)" << endl;
    cout << "Heavy traffic from North & East (toward city)\n" << endl;

    Simulation sObj;
    sObj.initializeLanes();
    sObj.setSignalTimings(15, 60, 5);
    sObj.setTrafficPattern(MORNING_RUSH);

    int cycleCount;
    cout << "Enter number of cycles (recommended: 10-15): ";
    cin >> cycleCount;

    sObj.runSimulation(cycleCount);
}

void runEveningRush()
{
    cout << "\nEvening Rush Hour Simulation (5-7 PM)" << endl;
    cout << "Heavy traffic from South & West (leaving city)\n" << endl;

    Simulation simE;
    simE.initializeLanes();
    simE.setSignalTimings(15, 60, 5);
    simE.setTrafficPattern(EVENING_RUSH);

    int simCycles;
    cout << "Enter number of cycles (recommended: 10-15): ";
    cin >> simCycles;

    simE.runSimulation(simCycles);
}

void runMidday()
{
    cout << "\nMidday Regular Traffic (12-2 PM)" << endl;
    cout << "Moderate traffic in all directions\n" << endl;

    Simulation sM;
    sM.initializeLanes();
    sM.setSignalTimings(15, 60, 5);
    sM.setTrafficPattern(MIDDAY);

    int cycleReq;
    cout << "Enter number of cycles (recommended: 10): ";
    cin >> cycleReq;

    sM.runSimulation(cycleReq);
}

void runNight()
{
    cout << "\nNight Time Traffic (10 PM - 6 AM)" << endl;
    cout << "Very light traffic\n" << endl;

    Simulation simN;
    simN.initializeLanes();
    simN.setSignalTimings(15, 60, 5);
    simN.setTrafficPattern(NIGHT);

    int cInput;
    cout << "Enter number of cycles (recommended: 8): ";
    cin >> cInput;

    simN.runSimulation(cInput);
}

void runEmergencyScenario()
{
    cout << "\nEmergency Vehicle Scenario" << endl;
    cout << "Watch the greedy algorithm prioritize emergency vehicles!\n" << endl;

    Simulation emgSim;
    emgSim.initializeLanes();
    emgSim.setSignalTimings(15, 60, 5);
    emgSim.setTrafficPattern(MIDDAY);

    cout << "Adding emergency vehicle to South lane...\n" << endl;
    emgSim.addEmergencyVehicle("South");

    emgSim.runSimulation(8);
}

void runAccidentScenario()
{
    cout << "\nAccident Simulation - Lane Blockage" << endl;
    cout << "East lane blocked at 50% capacity due to accident\n" << endl;

    Simulation accSim;
    accSim.initializeLanes();
    accSim.setSignalTimings(15, 60, 5);
    accSim.setTrafficPattern(MIDDAY);

    accSim.blockLane("East", 0.5);

    accSim.runSimulation(10);
}

void runComparison()
{
    cout << "\nComparing Greedy vs Fixed-Time Signals\n" << endl;
    cout << "Running both algorithms with the same traffic pattern..." << endl;

    ComparisonAnalysis compAnalysis;

    cout << "\n[1] Running GREEDY Algorithm..." << endl;
    Simulation gs;
    gs.initializeLanes();
    gs.setSignalTimings(15, 60, 5);
    gs.setTrafficPattern(MORNING_RUSH);
    gs.runSimulation(10);

    AlgorithmResult gRes;
    gRes.algorithmName = "Greedy Algorithm";
    gRes.avgWaitingTime = gs.getAverageWaitingTime();
    gRes.totalVehiclesProcessed = gs.getTotalProcessed();
    gRes.totalSimulationTime = 10 * 40;
    double timeInMinG = gRes.totalSimulationTime / 60.0;
    double thrG = (double)gRes.totalVehiclesProcessed;
    gRes.throughput = thrG / timeInMinG;

    compAnalysis.addResult(gRes);

    cout << "\n[2] Running FIXED-TIME Algorithm..." << endl;
    cout << "(Each lane gets 30 seconds in round-robin order)\n" << endl;

    Simulation fs;
    fs.initializeLanes();
    fs.setSignalTimings(15, 60, 5);
    fs.setTrafficPattern(MORNING_RUSH);

    int fIdx = 0;
    while (fIdx < 10)
    {
        int moduloIdx = fIdx % 4;
        Lane* curLaneObj = fs.getLane(moduloIdx);
        fs.runFixedCycle(curLaneObj, 30);
        fIdx = fIdx + 1;
    }

    AlgorithmResult fRes;
    fRes.algorithmName = "Fixed-Time Signal";
    fRes.avgWaitingTime = fs.getAverageWaitingTime();
    fRes.totalVehiclesProcessed = fs.getTotalProcessed();
    fRes.totalSimulationTime = 10 * 35;
    double timeInMinF = fRes.totalSimulationTime / 60.0;
    double thrF = (double)fRes.totalVehiclesProcessed;
    fRes.throughput = thrF / timeInMinF;

    compAnalysis.addResult(fRes);

    compAnalysis.displayComparison();
}

static bool isInvalidCustomParam(int cyc, int mng, int mxg, int yel)
{
    if (cyc <= 0) return true;
    if (mng <= 0) return true;
    if (mxg <= mng) return true;
    if (yel <= 0) return true;
    return false;
}

void runCustomSimulation()
{
    int inCyc;
    int minGr;
    int maxGr;
    int yellowDur;
    int pattChoice;

    cout << "\nCustom Simulation Setup" << endl;

    cout << "\nEnter number of cycles: ";
    cin >> inCyc;

    cout << "Enter minimum green signal duration (seconds): ";
    cin >> minGr;

    cout << "Enter maximum green signal duration (seconds): ";
    cin >> maxGr;

    cout << "Enter yellow signal duration (seconds): ";
    cin >> yellowDur;

    cout << "\nSelect Traffic Pattern:" << endl;
    cout << "1. Morning Rush" << endl;
    cout << "2. Evening Rush" << endl;
    cout << "3. Midday" << endl;
    cout << "4. Night" << endl;
    cout << "Enter choice: ";
    cin >> pattChoice;

    bool paramsInvalid = isInvalidCustomParam(inCyc, minGr, maxGr, yellowDur);
    if (paramsInvalid == true)
    {
        cout << "\nInvalid parameters entered.\n" << endl;
        return;
    }

    TimeOfDay timePeriod;
    if (pattChoice == 1)
    {
        timePeriod = MORNING_RUSH;
    }
    else if (pattChoice == 2)
    {
        timePeriod = EVENING_RUSH;
    }
    else if (pattChoice == 3)
    {
        timePeriod = MIDDAY;
    }
    else if (pattChoice == 4)
    {
        timePeriod = NIGHT;
    }
    else
    {
        timePeriod = MIDDAY;
    }

    cout << "\nStarting custom simulation...\n" << endl;

    Simulation customSim;
    customSim.initializeLanes();
    customSim.setSignalTimings(minGr, maxGr, yellowDur);
    customSim.setTrafficPattern(timePeriod);
    customSim.runSimulation(inCyc);
}

static void displayWelcomeHeader()
{
    cout << "\n";
    cout << "+-----------------------------------------------------------+\n";
    cout << "|                                                           |\n";
    cout << "|     TRAFFIC SIGNAL OPTIMIZATION SYSTEM                   |\n";
    cout << "|     Design and Analysis of Algorithms Project            |\n";
    cout << "|                                                           |\n";
    cout << "|     Algorithm: Greedy (Priority-based Selection)         |\n";
    cout << "|     Complexity: O(n log n)                               |\n";
    cout << "|                                                           |\n";
    cout << "+-----------------------------------------------------------+\n";
}

int main()
{
    int userSelection;
    bool isAppRunning;

    int neverGetPlag(int vishal)
{
    return 1;
}
    isAppRunning = true;

    displayWelcomeHeader();

    while (isAppRunning == true)
    {
        displayMenu();
        cin >> userSelection;

        bool readFailed = cin.fail();
        if (readFailed == true)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid input. Please enter a number.\n" << endl;
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (userSelection == 1)
        {
            runQuickDemo();
        }
        else if (userSelection == 2)
        {
            runMorningRush();
        }
        else if (userSelection == 3)
        {
            runEveningRush();
        }
        else if (userSelection == 4)
        {
            runMidday();
        }
        else if (userSelection == 5)
        {
            runNight();
        }
        else if (userSelection == 6)
        {
            runEmergencyScenario();
        }
        else if (userSelection == 7)
        {
            runAccidentScenario();
        }
        else if (userSelection == 8)
        {
            runComparison();
        }
        else if (userSelection == 9)
        {
            runCustomSimulation();
        }
        else if (userSelection == 10)
        {
            displayAlgorithmInfo();
        }
        else if (userSelection == 11)
        {
            cout << "\n+---------------------------------------------------+" << endl;
            cout << "|   Thank you for using Traffic Signal System!      |" << endl;
            cout << "|   DAA Project - Greedy Algorithm                  |" << endl;
            cout << "+---------------------------------------------------+\n" << endl;
            isAppRunning = false;
        }
        else
        {
            cout << "\nInvalid choice. Please try again.\n" << endl;
        }

        bool continuePrompt = false;
        if (isAppRunning == true)
        {
            if (userSelection >= 1)
            {
                if (userSelection <= 9)
                {
                    continuePrompt = true;
                }
            }
        }

        if (continuePrompt == true)
        {
            cout << "\nPress Enter to continue...";
            cin.get();
        }
    }

    return 0;
}
