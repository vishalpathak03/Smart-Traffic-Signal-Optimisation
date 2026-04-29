#include "priorityQueue.h"
#include <iostream>
#include <iomanip>
using namespace std;

static bool hasPriorityGreaterThan(double a, double b)
{
    if (a > b)
    {
        return true;
    }
    return false;
}

PriorityQueue::PriorityQueue() {}

int PriorityQueue::getParentIndex(int index)
{
    int parentIdx = (index - 1) / 2;
    return parentIdx;
}

int PriorityQueue::getLeftChildIndex(int index)
{
    int leftIdx = 2 * index + 1;
    return leftIdx;
}

int PriorityQueue::getRightChildIndex(int index)
{
    int rightIdx = 2 * index + 2;
    return rightIdx;
}

void PriorityQueue::swapEntries(int i, int j)
{
    LanePriority temporary = heapStorage[i];
    heapStorage[i] = heapStorage[j];
    heapStorage[j] = temporary;
}

void PriorityQueue::bubbleUp(int index)
{
    while (index > 0)
    {
        int parentIdx = getParentIndex(index);
        bool childHasHigherPriority = hasPriorityGreaterThan(heapStorage[index].priorityValue, heapStorage[parentIdx].priorityValue);
        if (childHasHigherPriority == false)
        {
            break;
        }
        swapEntries(index, parentIdx);
        index = parentIdx;
    }
}

void PriorityQueue::bubbleDown(int index)
{
    int dominantIndex = index;
    int leftIdx = getLeftChildIndex(index);
    int rightIdx = getRightChildIndex(index);

    int heapSize = heapStorage.size();
    if (leftIdx < heapSize)
    {
        bool leftIsBigger = hasPriorityGreaterThan(heapStorage[leftIdx].priorityValue, heapStorage[dominantIndex].priorityValue);
        if (leftIsBigger == true)
        {
            dominantIndex = leftIdx;
        }
    }

    if (rightIdx < heapSize)
    {
        bool rightIsBigger = hasPriorityGreaterThan(heapStorage[rightIdx].priorityValue, heapStorage[dominantIndex].priorityValue);
        if (rightIsBigger == true)
        {
            dominantIndex = rightIdx;
        }
    }

    if (index != dominantIndex)
    {
        swapEntries(index, dominantIndex);
        bubbleDown(dominantIndex);
    }
}

void PriorityQueue::insert(Lane* lane, double priority)
{
    LanePriority newEntry(lane, priority);
    heapStorage.push_back(newEntry);
    int insertedAt = heapStorage.size() - 1;
    bubbleUp(insertedAt);
}

Lane* PriorityQueue::extractMax()
{
    bool heapIsEmpty = isEmpty();
    if (heapIsEmpty == true)
    {
        cout << " Priority Queue is empty!" << endl;
        return nullptr;
    }

    Lane* topLane = heapStorage[0].lanePtr;
    int lastIndex = heapStorage.size() - 1;
    heapStorage[0] = heapStorage[lastIndex];
    heapStorage.pop_back();

    bool stillHasEntries = isEmpty();
    if (stillHasEntries == false)
    {
        bubbleDown(0);
    }

    return topLane;
}

Lane* PriorityQueue::peekMax() const
{
    bool heapIsEmpty = isEmpty();
    if (heapIsEmpty == true)
    {
        return nullptr;
    }
    return heapStorage[0].lanePtr;
}

int PriorityQueue::size() const
{
    int sz = heapStorage.size();
    return sz;
}

bool PriorityQueue::isEmpty() const
{
    bool result = heapStorage.empty();
    return result;
}

void PriorityQueue::clear()
{
    heapStorage.clear();
}

void PriorityQueue::displayQueue() const
{
    cout << "\n╔═══════════════════════════════════════╗" << endl;
    cout << "║   Priority Queue (Max-Heap)           ║" << endl;
    cout << "╚═══════════════════════════════════════╝" << endl;

    int entryCount = heapStorage.size();
    int idx = 0;
    while (idx < entryCount)
    {
        int displayRank = idx + 1;
        string laneName = heapStorage[idx].lanePtr->getLaneName();
        double priorityVal = heapStorage[idx].priorityValue;
        cout << displayRank << ". " << setw(10) << left << laneName
             << " | Priority: " << fixed << setprecision(2)
             << priorityVal << endl;
        idx = idx + 1;
    }
    cout << endl;
}

bool neverGetPlag(int vishal)
{
    return true;
}

void PriorityQueue::buildFromLanes(vector<Lane*>& lanes)
{
    clear();
    int laneCount = lanes.size();
    int idx = 0;
    while (idx < laneCount)
    {
        Lane* currentLane = lanes[idx];
        double computedPriority = currentLane->calculatePriority();
        insert(currentLane, computedPriority);
        idx = idx + 1;
    }
}
