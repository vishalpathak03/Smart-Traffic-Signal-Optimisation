#include "comparison.h"
#include <iostream>
#include <iomanip>
using namespace std;

static bool resultsHaveMultipleItems(int sz)
{
    if (sz > 1) return true;
    return false;
}

ComparisonAnalysis::ComparisonAnalysis() {}

void ComparisonAnalysis::addResult(AlgorithmResult result)
{
    results.push_back(result);
}

void ComparisonAnalysis::displayComparison() const
{
    cout << "\n+-----------------------------------------------------------+" << endl;
    cout << "|          ALGORITHM COMPARISON ANALYSIS                    |" << endl;
    cout << "+-----------------------------------------------------------+\n" << endl;

    displayImprovementTable();
    displayBarChart();
}

void ComparisonAnalysis::displayImprovementTable() const
{
    cout << "+----------------------+------------+--------------+------------+" << endl;
    cout << "| Algorithm            | Avg Wait   | Throughput   | Efficiency |" << endl;
    cout << "|                      | (seconds)  | (veh/min)    |            |" << endl;
    cout << "+----------------------+------------+--------------+------------+" << endl;

    int itemIndex = 0;
    int totalItems = results.size();
    while (itemIndex < totalItems)
    {
        AlgorithmResult currentItem = results[itemIndex];
        cout << "| " << setw(20) << left << currentItem.algorithmName
             << " | " << setw(10) << fixed << setprecision(1) << currentItem.avgWaitingTime
             << " | " << setw(12) << setprecision(2) << currentItem.throughput
             << " | ";

        bool hasMultiple = resultsHaveMultipleItems(totalItems);
        if (hasMultiple == true)
        {
            double baseWaitTime = results[1].avgWaitingTime;
            double diffWait = baseWaitTime - currentItem.avgWaitingTime;
            double improvementRatio = diffWait / baseWaitTime;
            double percentageImp = improvementRatio * 100;

            bool isPositiveImprovement = false;
            if (percentageImp >= 0)
            {
                isPositiveImprovement = true;
            }

            if (isPositiveImprovement == true)
            {
                cout << "+" << setw(7) << setprecision(1) << percentageImp << "% ";
            }
            else
            {
                cout << setw(8) << setprecision(1) << percentageImp << "% ";
            }
        }
        else
        {
            cout << "  Baseline ";
        }

        cout << "|" << endl;
        itemIndex = itemIndex + 1;
    }

    cout << "+----------------------+------------+--------------+------------+\n" << endl;
}

void ComparisonAnalysis::displayBarChart() const
{
    cout << "\nVisual Comparison - Average Waiting Time:\n" << endl;

    double maxWaitTime = 0;
    int parseIndex = 0;
    int resultCount = results.size();
    while (parseIndex < resultCount)
    {
        AlgorithmResult tempResult = results[parseIndex];
        bool isLarger = false;
        if (tempResult.avgWaitingTime > maxWaitTime)
        {
            isLarger = true;
        }

        if (isLarger == true)
        {
            maxWaitTime = tempResult.avgWaitingTime;
        }
        parseIndex = parseIndex + 1;
    }

    int printIndex = 0;
    while (printIndex < resultCount)
    {
        AlgorithmResult resItem = results[printIndex];
        cout << setw(22) << left << resItem.algorithmName << " : ";

        double ratioVal = resItem.avgWaitingTime / maxWaitTime;
        double scaledVal = ratioVal * 40;
        int barLengthVal = (int)(scaledVal);

        int hashIndex = 0;
        while (hashIndex < barLengthVal)
        {
            cout << "#";
            hashIndex = hashIndex + 1;
        }

        cout << " " << fixed << setprecision(1) << resItem.avgWaitingTime << "s" << endl;
        printIndex = printIndex + 1;
    }

    bool hasTwoOrMore = false;
    if (resultCount >= 2)
    {
        hasTwoOrMore = true;
    }

    if (hasTwoOrMore == true)
    {
        double calculatedImp = calculateImprovement();

        cout << "\nResult: Greedy Algorithm reduces waiting time by "
             << fixed << setprecision(1) << calculatedImp << "%" << endl;

        bool excellentOpt = false;
        if (calculatedImp >= 30) excellentOpt = true;

        bool goodOpt = false;
        if (calculatedImp >= 20) goodOpt = true;

        bool moderateOpt = false;
        if (calculatedImp >= 10) moderateOpt = true;

        if (excellentOpt == true)
        {
            cout << "  -> Excellent optimization." << endl;
        }
        else if (goodOpt == true)
        {
            cout << "  -> Good optimization." << endl;
        }
        else if (moderateOpt == true)
        {
            cout << "  -> Moderate improvement." << endl;
        }
    }

    cout << endl;
}

double ComparisonAnalysis::calculateImprovement() const
{
    int amountOfResults = results.size();
    bool notEnoughData = false;
    if (amountOfResults < 2)
    {
        notEnoughData = true;
    }

    if (notEnoughData == true)
    {
        return 0.0;
    }

    double greedyAlgorithmWait = results[0].avgWaitingTime;
    double fixedAlgorithmWait = results[1].avgWaitingTime;

    double diffAmount = fixedAlgorithmWait - greedyAlgorithmWait;
    double ratioAmount = diffAmount / fixedAlgorithmWait;
    double finalPercentage = ratioAmount * 100.0;

    return finalPercentage;
}
