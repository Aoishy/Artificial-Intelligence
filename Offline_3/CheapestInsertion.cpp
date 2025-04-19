#include <bits/stdc++.h>
using namespace std;
#define maximum_distance 1000000000.0


vector<int> cheapestInsertionTSP(vector<vector<double>> distanceMatrix)
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
        int bestCity = -1;
        int bestPosition = -1;
        double minIncrease = maximum_distance;

        for (int city = 0; city < num_of_Cities; city++)
        {
            if (visited[city])
                continue;
         
            for (int pos = 0; pos < travel.size() - 1; pos++)
            {
                int cityA = travel[pos];
                int cityB = travel[pos + 1];

                double increase = distanceMatrix[cityA][city] + distanceMatrix[city][cityB] - distanceMatrix[cityA][cityB];
       
                if (increase < minIncrease)
                {
                    minIncrease = increase;
                    bestCity = city;
                    bestPosition = pos + 1;
                }
            }
        }
  
        travel.insert(travel.begin() + bestPosition, bestCity);
        visited[bestCity] = true;
    }

    return travel;
}