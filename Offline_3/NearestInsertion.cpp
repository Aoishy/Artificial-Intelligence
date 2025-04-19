#include <bits/stdc++.h>
using namespace std;
#define maximum_distance 1000000000.0


int Find_NearestCity(vector<vector<double>> DistanceMatrix, vector<bool> visited, vector<int> travel)
{
    int nearestCity = -1;
    double minDistance = maximum_distance;

    for (int cityInTravel : travel)
    {
        for (int city = 0; city < DistanceMatrix.size(); city++)
        {
            if (!visited[city] && DistanceMatrix[cityInTravel][city] < minDistance)
            {
                minDistance = DistanceMatrix[cityInTravel][city];
                nearestCity = city;
            }
        }
    }
    return nearestCity;
}




int FindBestInsertion(vector<vector<double>> distanceMatrix, vector<int> travel, int newCity)
{
    int bestPosition = -1;
    double minIncrease = maximum_distance;
    for (int i = 0; i < travel.size() - 1; i++)
    {
        double increase = distanceMatrix[travel[i]][newCity] + distanceMatrix[newCity][travel[i + 1]] - distanceMatrix[travel[i]][travel[i + 1]];
        if (increase < minIncrease)
        {
            minIncrease = increase;
            bestPosition = i + 1;
        }
    }
    return bestPosition;
}




vector<int> NearestInsertionTSP(vector<vector<double>> DistanceMatrix, int source)
{
    int traveling_cities = DistanceMatrix.size();
    vector<int> travel = {source, source};
    vector<bool> visited(traveling_cities, false);
    visited[source] = true;
    while (travel.size() < traveling_cities + 1)
    {
        int nearestCity = Find_NearestCity(DistanceMatrix, visited, travel);
        if (nearestCity == -1)
            break;
        visited[nearestCity] = true;
        int bestPosition = FindBestInsertion(DistanceMatrix, travel, nearestCity);
        travel.insert(travel.begin() + bestPosition, nearestCity);
    }
    return travel;
}