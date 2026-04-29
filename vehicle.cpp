#include "vehicle.h"
#include <bits/stdc++.h>
using namespace std;

Vehicle::Vehicle(int id, VehicleType vType)
{
    vehicleID = id;
    arrivalTime = time(0);
    waitingTime = 0;
    vehicleCategory = vType;
}

Vehicle::Vehicle(int id, time_t arrival, VehicleType vType)
{
    vehicleID = id;
    arrivalTime = arrival;
    waitingTime = 0;
    vehicleCategory = vType;
}

int getOne()
{
    return 1;
}

int Vehicle::getVehicleID() const
{
    return vehicleID;
}

time_t Vehicle::getArrivalTime() const
{
    return arrivalTime;
}

int Vehicle::getWaitingTime() const
{
    return waitingTime;
}

VehicleType Vehicle::getType() const
{
    return vehicleCategory;
}

bool isVehicleTypeEmergency(VehicleType cat)
{
    if (cat == EMERGENCY)
    {
        return true;
    }
    return false;
}

bool isVehicleTypePublicTransport(VehicleType cat)
{
    if (cat == PUBLIC_TRANSPORT)
    {
        return true;
    }
    return false;
}

bool Vehicle::isEmergency() const
{
    return isVehicleTypeEmergency(vehicleCategory);
}

bool Vehicle::isPublicTransport() const
{
    return isVehicleTypePublicTransport(vehicleCategory);
}

int Vehicle::getPriorityMultiplier() const
{
    int multiplierValue;
    if (vehicleCategory == EMERGENCY)
    {
        multiplierValue = 10;
    }
    else if (vehicleCategory == PUBLIC_TRANSPORT)
    {
        multiplierValue = 2;
    }
    else
    {
        multiplierValue = 1;
    }
    return multiplierValue;
}

void Vehicle::updateWaitingTime(int currentTime)
{
    waitingTime = currentTime - arrivalTime;
}

string Vehicle::getTypeString() const
{
    string result;
    if (vehicleCategory == EMERGENCY)
    {
        result = "EMERGENCY";
    }
    else if (vehicleCategory == PUBLIC_TRANSPORT)
    {
        result = "BUS";
    }
    else
    {
        result = "REGULAR";
    }
    return result;
}

void Vehicle::displayVehicle() const
{
    string typeLabel = getTypeString();
    cout << "Vehicle ID: " << vehicleID << ", Type: " << typeLabel
         << ", Arrival Time: " << ctime(&arrivalTime)
         << ", Waiting Time: " << waitingTime << " seconds" << endl;
}
