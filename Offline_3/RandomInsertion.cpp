#include <bits/stdc++.h>
using namespace std;
#define maximum_distance 1000000000.0


int FindBestInsertion_random(int city, vector<int> travel, vector<vector<double>> distanceMatrix)
{   
     int bestPosition = -1;
    double minIncrease = maximum_distance;

    for (int i = 0; i < travel.size() - 1; i++)
    {
        int cityA = travel[i];
        int cityB = travel[i + 1];
        double increase = distanceMatrix[cityA][city] + distanceMatrix[city][cityB] - distanceMatrix[cityA][cityB];
        if (increase < minIncrease)
        {
            minIncrease = increase;
            bestPosition = i + 1;
        }
    }
    return bestPosition;
}
vector<int> RandomInsertionTSP(vector<vector<double>> distanceMatrix)
{
    int num_of_Cities = distanceMatrix.size();

    vector<int> travel;

    vector<bool> visited(num_of_Cities, false);

    srand(time(0));
    int city1 = rand() % num_of_Cities;
    int city2;
    do
    {
        city2 = rand() % num_of_Cities;
    } while (city2 == city1);

    travel.push_back(city1);
    travel.push_back(city2);
    travel.push_back(city1);
    visited[city1] = visited[city2] = true;

    for (int i = 0; i < num_of_Cities - 2; i++)
    {
        int newCity;
        do
        {
            newCity = rand() % num_of_Cities;
        } while (visited[newCity]);

        int bestPosition = FindBestInsertion_random(newCity, travel, distanceMatrix);
        travel.insert(travel.begin() + bestPosition, newCity);
        visited[newCity] = true;
    }

    return travel;
}