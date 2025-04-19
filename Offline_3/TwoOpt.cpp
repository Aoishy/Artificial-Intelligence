#include <bits/stdc++.h>
using namespace std;



void twoOptSwap(vector<int> &travel, int i, int j)
{
    
    while (i < j)
    {
        swap(travel[i], travel[j]);
        i++;
        j--;
    }

}
vector<int> twoOptAlgorithm(vector<int> travel, vector<vector<double>> distanceMatrix)
{
    int n = travel.size();
    bool improvement = true;
    while (improvement)
    {
        improvement = false;

        for (int i = 1; i < n - 2; i++)
        {
            for (int j = i + 1; j < n - 1; j++)
            {
               
                double oldCost = distanceMatrix[travel[i - 1]][travel[i]] + distanceMatrix[travel[j]][travel[j + 1]];

                double newCost = distanceMatrix[travel[i - 1]][travel[j]] + distanceMatrix[travel[i]][travel[j + 1]];
                if (newCost < oldCost)
                {
                    twoOptSwap(travel, i, j);
                    improvement = true;
                }
            }
        }
    }
    return travel;
}

