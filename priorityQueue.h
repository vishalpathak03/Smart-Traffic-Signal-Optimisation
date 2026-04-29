#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "lane.h"
#include <vector>
using namespace std;

struct LanePriority {
    Lane* lanePtr;
    double priorityValue;

    LanePriority(Lane* l, double p)
    {
        lanePtr = l;
        priorityValue = p;
    }
};

class PriorityQueue {
private:
    vector<LanePriority> heapStorage;

    void bubbleUp(int index);
    void bubbleDown(int index);
    int getParentIndex(int index);
    int getLeftChildIndex(int index);
    int getRightChildIndex(int index);
    void swapEntries(int i, int j);

public:
    PriorityQueue();

    void insert(Lane* lane, double priority);
    Lane* extractMax();
    Lane* peekMax() const;

    int size() const;
    bool isEmpty() const;
    void clear();
    void displayQueue() const;

    void buildFromLanes(vector<Lane*>& lanes);
};

#endif
