#include "comparison.h"
#include <iostream>
#include <iomanip>
using namespace std;

ComparisonAnalysis::ComparisonAnalysis() {}

void ComparisonAnalysis::addResult(AlgorithmResult result) {
    results.push_back(result);
}

void ComparisonAnalysis::displayComparison() const {
    cout << "\n+-----------------------------------------------------------+" << endl;
    cout << "|          ALGORITHM COMPARISON ANALYSIS                    |" << endl;
    cout << "+-----------------------------------------------------------+\n" << endl;

    displayImprovementTable();
    displayBarChart();
}

void ComparisonAnalysis::displayImprovementTable() const {
    cout << "+----------------------+------------+--------------+------------+" << endl;
    cout << "| Algorithm            | Avg Wait   | Throughput   | Efficiency |" << endl;
    cout << "|                      | (seconds)  | (veh/min)    |            |" << endl;
    cout << "+----------------------+------------+--------------+------------+" << endl;

    for (const auto& result : results) {
        cout << "| " << setw(20) << left << result.algorithmName
             << " | " << setw(10) << fixed << setprecision(1) << result.avgWaitingTime
             << " | " << setw(12) << setprecision(2) << result.throughput
             << " | ";

        if (results.size() > 1) {
            double baseWait = results[1].avgWaitingTime;
            double improvement = ((baseWait - result.avgWaitingTime) / baseWait) * 100;

            if (improvement >= 0) {
                cout << "+" << setw(7) << setprecision(1) << improvement << "% ";
            } else {
                cout << setw(8) << setprecision(1) << improvement << "% ";
            }
        } else {
            cout << "  Baseline ";
        }

        cout << "|" << endl;
    }

    cout << "+----------------------+------------+--------------+------------+\n" << endl;
}

void ComparisonAnalysis::displayBarChart() const {
    cout << "\nVisual Comparison - Average Waiting Time:\n" << endl;

    double maxWait = 0;
    for (const auto& result : results) {
        if (result.avgWaitingTime > maxWait) {
            maxWait = result.avgWaitingTime;
        }
    }

    for (const auto& result : results) {
        cout << setw(22) << left << result.algorithmName << " : ";

        int barLength = (int)((result.avgWaitingTime / maxWait) * 40);

        for (int i = 0; i < barLength; i++) {
            cout << "#";
        }

        cout << " " << fixed << setprecision(1) << result.avgWaitingTime << "s" << endl;
    }

    if (results.size() >= 2) {
        double improvement = calculateImprovement();

        cout << "\nResult: Greedy Algorithm reduces waiting time by "
             << fixed << setprecision(1) << improvement << "%" << endl;

        if (improvement >= 30) {
            cout << "  -> Excellent optimization." << endl;
        } else if (improvement >= 20) {
            cout << "  -> Good optimization." << endl;
        } else if (improvement >= 10) {
            cout << "  -> Moderate improvement." << endl;
        }
    }

    cout << endl;
}

double ComparisonAnalysis::calculateImprovement() const {
    if (results.size() < 2) return 0.0;

    double greedyWait = results[0].avgWaitingTime;
    double fixedWait = results[1].avgWaitingTime;

    return ((fixedWait - greedyWait) / fixedWait) * 100.0;
}
