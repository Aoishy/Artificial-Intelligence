#include <bits/stdc++.h>
#include "NearestInsertion.cpp"
#include "RandomInsertion.cpp"
#include "CheapestInsertion.cpp"
#include "TwoOpt.cpp"
#include "NodeShift_OrOpt.cpp"


using namespace std;

#define maximum_distance 1000000000.0

int source;
struct City
{
    int index;
    double x, y;
};
vector<City> cities;

double Eucidian_dis(City city1, City city2)
{
    double dis = sqrt(pow(city1.x - city2.x, 2) + pow(city1.y - city2.y, 2));
    return dis;
}


int main()
{
    string filename = "kroA100.tsp";
    ifstream file(filename);
    string line;
    bool inNodeSection = false;
    while (getline(file, line))
    {
        if (line == "NODE_COORD_SECTION")
        {
            inNodeSection = true;
            continue;
        }
        if (line == "EOF")
            break;
        if (inNodeSection)
        {
            stringstream inputcity(line);
            City city;
            inputcity >> city.index >> city.x >> city.y;
            cities.push_back(city);
           
        }
    }
    int distancematrix_size = cities.size();
    vector<vector<double>> DistanceMatrix(distancematrix_size, vector<double>(distancematrix_size, 0));
    for (int i = 0; i < distancematrix_size; i++)
    {
        for (int j = 0; j < distancematrix_size; j++)
        {
            if (i != j)
            {
                DistanceMatrix[i][j] = DistanceMatrix[j][i] = Eucidian_dis(cities[i], cities[j]);
            }
        }
    }
    cout << "Enter Your heuristic Choice:" << endl;
    cout << "1. Nearest_Insertion Tour:" << endl;
    cout << "2. Random Insertion Tour:" << endl;
    cout << "3. Cheapest Insertion Tour " << endl;
    cout << "4. 2opt Insertion Tour " << endl;
    cout << "5. Node Shift Tour " << endl;
    cout << "6. orOpt inserion Tour " << endl; 
    int n;
    cin >> n;
    vector<int> travel(cities.size());
    if (n == 1)
    {
        source = cities[0].index - 1;
        travel = NearestInsertionTSP(DistanceMatrix, source);
        cout << "Nearest Insertion Tour:\n ";
    }
    else if (n == 2)
    {
        travel = RandomInsertionTSP(DistanceMatrix);
        cout << "Random Insertion Tour:\n ";
    }
    else if (n == 3)
    {
        travel = cheapestInsertionTSP(DistanceMatrix);
        cout << "Cheapest Insertion Tour:\n ";
    }
    else if (n == 4)
    {
        iota(travel.begin(), travel.end(), 0);
        travel = twoOptAlgorithm(travel, DistanceMatrix);
        travel.push_back(travel[0]);
        cout << "2 opt Insertion Tour:\n ";
    }
    else if (n == 5)
    {
        for (int i = 0; i < cities.size(); ++i) {
        travel[i] =i;
        }

        travel = NodeShift(travel, DistanceMatrix);
        travel.push_back(travel[0]);
        cout << "Node Shift Insertion Tour:\n ";
    }
     else if (n == 6)
    {
       iota(travel.begin(), travel.end(), 0);
       travel = orOptHeuristic(travel, DistanceMatrix);

        travel.push_back(travel[0]);
        cout << "Oropt Insertion Tour:\n ";
    }

    double totalCost = calculateTourCost(travel, DistanceMatrix);
    cout << "Total tour cost:  " << totalCost << endl;

    return 0;
}


