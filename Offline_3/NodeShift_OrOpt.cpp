#include <bits/stdc++.h>
using namespace std;


double calculateTourCost(vector<int> travel, vector<vector<double>> distanceMatrix)
{
    double totalCost = 0.0;
    for (int i = 0; i < travel.size() - 1; i++)
    {
        totalCost += distanceMatrix[travel[i]][travel[i + 1]];
    }
    totalCost += distanceMatrix[travel[travel.size() - 1]][travel[0]];
    return totalCost;
}

vector<int> NodeShift(vector<int> travel, vector<vector<double>> distanceMatrix)
{
    vector<int> bestpath = travel;
    double bestcost = calculateTourCost(travel, distanceMatrix);

    bool imprv = true;
    int n = travel.size();
    while (imprv)
    {
        imprv = false;
        for (int i = 0; i < n ; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if(i==j)
                {
                    continue;
                }
                vector<int>newpath=bestpath;
                int node=newpath[i];
                newpath.erase(newpath.begin()+i);
                newpath.insert(newpath.begin()+j,node);
                double newcost=calculateTourCost(newpath,distanceMatrix);
                if(newcost<bestcost)
                { 
                    bestpath=newpath;
                    bestcost=newcost;
                    imprv=true;

                }
            }
        }
    }
    return bestpath;
}

vector<int> orOptHeuristic(vector<int> travel,vector<vector<double>>distanceMatrix) {
    int n = travel.size();
    bool imprv = true;

    while (imprv) {

        imprv = false;

        double bestCost = calculateTourCost(travel, distanceMatrix);

        for (int segmentLength = 1; segmentLength <= 3; ++segmentLength) {
            for (int i = 0; i < n; ++i) {
        
                int endSegment = i + segmentLength;
                if (endSegment > n) break;

                vector<int> segment(travel.begin() + i, travel.begin() + endSegment);
                vector<int> newpath = travel;
                newpath.erase(newpath.begin() + i, newpath.begin() + endSegment);

            
                for (int j = 0; j <= newpath.size(); ++j) {

                    vector<int> modifiedTour = newpath;
                    modifiedTour.insert(modifiedTour.begin() + j, segment.begin(), segment.end());

                    double newCost = calculateTourCost(modifiedTour, distanceMatrix);

                    if (newCost < bestCost) {
                        travel = modifiedTour;
                        bestCost = newCost;
                        imprv = true;
                    }
                }
            }
        }
    }
    return travel;
}