#include <bits/stdc++.h>
using namespace std;
#define ll long long
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct Data
{
    map<string, string> attributes;
    string label;
};

struct Node
{
    string attribute; // splitting attribute
    map<string, Node *> children;
    string classification;
};

string priortyValue(vector<Data> parent)
{
    map<string, long long> labels;
    for (auto &data : parent)
    {
        labels[data.label]++;
    }
    string priortyLabel;
    long long maxCount = 0;

    for (auto &pair : labels)
    {
        if (pair.second > maxCount)
        {
            priortyLabel = pair.first;
            maxCount = pair.second;
        }
    }

    return priortyLabel;
}

set<string> getUniqueAttributeValues(vector<Data> trainingdata, string &attribute)
{
    set<string> uniqueVals;
    for (auto &example : trainingdata)
    {
        uniqueVals.insert(example.attributes.at(attribute));
    }

    return uniqueVals;
}
double calculateEntropy(vector<Data> dataset)
{
    map<string, long long> labels; // storing which label has how many instances
    for (auto &data : dataset)
    {
        labels[data.label]++;
    }

    double entropy = 0.0;
    double totalExamples = dataset.size();

    // Calculate entropy using the label frequencies
    for (auto &entry : labels)
    {
        double p = (double)entry.second / dataset.size();
        entropy += p * log2(1 / p);
    }
    return entropy;
}

double calculateGiniImpurity(vector<Data> &dataset)
{
    map<string, long long> labels; // Store label counts
    long long totalExamples = dataset.size();

    // occurrences of each label
    for (long long i = 0; i < totalExamples; i++)
    {
        labels[dataset[i].label]++;
    }

    double gini = 1.0;

    for (auto &entry : labels)
    {
        double proportion = (double)entry.second / totalExamples;
        gini -= proportion * proportion;
    }

    return gini;
}

double calculateGain(vector<Data> trainingdata, string &attribute, int func)
{
    double curEntropy = 0.0;
    if (func == 1)
    {
        curEntropy = calculateEntropy(trainingdata);
    }
    else
    {
        curEntropy = calculateGiniImpurity(trainingdata);
    }
    set<string> uniqueVals = getUniqueAttributeValues(trainingdata, attribute);

    double weightedEntropy = 0.0;
    for (auto &value : uniqueVals)
    {
        // Create a subset of examples that match the current value of the attribute
        vector<Data> subset;
        for (auto &example : trainingdata)
        {
            if (example.attributes.at(attribute) == value)
                subset.push_back(example);
        }
        // double subsetEntropy = calculateEntropy(subset);
        double subsetEntropy = 0.0;
        if (func == 1)
        {
            subsetEntropy = calculateEntropy(subset);
        }
        else
        {
            subsetEntropy = calculateGiniImpurity(subset);
        }

        weightedEntropy += ((double)subset.size() / trainingdata.size()) * subsetEntropy;
    }

    double infoGain = curEntropy - weightedEntropy;
    return infoGain;
}

string findBestAttribute(vector<Data> trainingdata, vector<string> curr_attributes, int func)
{
    string bestAttribute;
    double maxInfoGain = -1.0;

    // Iterate through each attribute and calculate  gain
    for (auto &attribute : curr_attributes)
    {
        double infoGain = calculateGain(trainingdata, attribute, func);
        if (infoGain > maxInfoGain)
        {
            maxInfoGain = infoGain;
            bestAttribute = attribute;
        }
    }

    return bestAttribute;
}

string findBestAttributerandom(vector<Data> trainingdata, vector<string> curr_attribute, int func)
{
    vector<pair<string, double>> gains; // Vector to store attribute and its information gain
    for (long long i = 0; i < curr_attribute.size(); i++)
    {
        double infoGain = calculateGain(trainingdata, curr_attribute[i], func);
        gains.push_back({curr_attribute[i], infoGain});
    }
    sort(gains.begin(), gains.end(), [](const pair<string, double> &a, const pair<string, double> &b)
         {
             return a.second > b.second; // Sort in descending order of info gain
         });

    int topN = min(3, (int)gains.size());
    vector<string> topAttributes;
    for (int i = 0; i < topN; i++)
    {
        topAttributes.push_back(gains[i].first);
    }

    uniform_int_distribution<int> dist(0, topN - 1);
    int randomIndex = dist(rng);
    return topAttributes[randomIndex];
}

Node *BuildDecisionTree(vector<string> dataset_attribute, vector<Data> trainingdata, vector<Data> parent, int choice, int func)
{
    Node *node = new Node;
    if (trainingdata.empty())
    {
        node->classification = priortyValue(parent);
    }
    else
    {

        bool allSameLabel = true;
        string firstLabel = trainingdata[0].label;
        for (auto &data : trainingdata)
        {
            if (data.label != firstLabel)
            {
                allSameLabel = false;
                break;
            }
        }

        if (allSameLabel)
        {
            node->classification = firstLabel;
        }
        else if (dataset_attribute.empty())
        {
            node->classification = priortyValue(trainingdata);
        }
        else
        {
            string bestAttribute;
            if (choice == 1)
            {
                bestAttribute = findBestAttribute(trainingdata, dataset_attribute, func);
            }
            else if (choice == 2)
            {
                bestAttribute = findBestAttributerandom(trainingdata, dataset_attribute, func);
            }
            node->attribute = bestAttribute;
            set<string> uniqueAttributeVals = getUniqueAttributeValues(trainingdata, bestAttribute);
            for (auto &value : uniqueAttributeVals)
            {
                // Filter examples based on the current attribute value
                vector<Data> filteredDatset;
                for (auto &example : trainingdata)
                {
                    if (example.attributes[bestAttribute] == value)
                    {
                        filteredDatset.push_back(example);
                    }
                }

                vector<string> remainingAttributes = dataset_attribute;
                remainingAttributes.erase(remove(remainingAttributes.begin(), remainingAttributes.end(), bestAttribute), remainingAttributes.end());

                Node *subtree = BuildDecisionTree(remainingAttributes, filteredDatset, trainingdata, choice, func);
                node->children[value] = subtree;
            }
        }
    }
    return node;
}

double calculateAccuracy(Node *root, vector<Data> testingdata)
{
    long long correctPredictions = 0;

    for (long long i = 0; i < testingdata.size(); i++)
    {
        Node *current = root;

        // Traverse the tree iteratively
        while (!current->children.empty())
        {
            string attributeValue = testingdata[i].attributes[current->attribute];

            // Check if the attribute value has a corresponding child node
            if (current->children.find(attributeValue) != current->children.end())
            {
                current = current->children[attributeValue]; // Move to the next node
            }
            else
            {
                break; // No matching child node
            }
        }

        string prediction = current->classification;
        if (prediction == testingdata[i].label)
        {
            correctPredictions++;
        }
    }
    return (double)correctPredictions / testingdata.size() * 100;
}

void deleteTree(Node *node)
{
    if (node == nullptr)
        return;
    for (auto pair : node->children)
    {
        deleteTree(pair.second);
    }

    delete (node);
}

vector<Data> loadData(string filename, vector<string> &dataset_attributes)
{
    vector<Data> dataset;
    ifstream infile(filename);
    string s;
    while (getline(infile, s))
    {
        vector<string> values;
        string temp;
        for (char c : s)
        {
            if (c == ',')
            {
                values.push_back(temp);
                temp.clear();
            }
            else
            {
                temp += c;
            }
        }
        values.push_back(temp);

        Data data;
        for (size_t i = 0; i < dataset_attributes.size(); i++)
        {
            data.attributes[dataset_attributes[i]] = values[i];
        }
        data.label = values.back();
        dataset.push_back(data);
    }
    infile.close();
    return dataset;
}

void PrintResult(double res1, double res2, double res3, double res4)
{
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "                                           Average accuracy over 20 runs       " << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << " Attribute selection strategy       |   Information Gain  |   Gini Impurity     " << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << " Always select the best Attribute   |   " << res1 << "           |   " << res3 << "       " << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << " Randomly from Top Three Attributes |   " << res2 << "           |   " << res4 << "       " << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
}

int main()
{
    vector<string> dataset_attributes = {"buying", "maint", "doors", "persons", "lug_boot", "safety"};
    vector<Data> dataset = loadData("car.data", dataset_attributes);
    double accuracySum1 = 0.0;
    double accuracySum2 = 0.0;
    double accuracySum3 = 0.0;
    double accuracysum4 = 0.0;

    for (int i = 0; i < 20; i++)
    {
        shuffle(dataset.begin(), dataset.end(), rng);
        vector<Data> testingdata;
        vector<Data> trainingdata;
        ll datasize = dataset.size();
        ll testsize = (8 * datasize) / 10;
        for (ll i = 0; i < datasize; i++)
        {
            if (i < testsize)
            {
                trainingdata.push_back(dataset[i]);
            }
            else
            {
                testingdata.push_back(dataset[i]);
            }
        }

        vector<Data> parent;
        Node *root1 = BuildDecisionTree(dataset_attributes, trainingdata, parent, 1, 1);
        Node *root2 = BuildDecisionTree(dataset_attributes, trainingdata, parent, 2, 1);
        Node *root3 = BuildDecisionTree(dataset_attributes, trainingdata, parent, 1, 2);
        Node *root4 = BuildDecisionTree(dataset_attributes, trainingdata, parent, 2, 2);
        double sum = calculateAccuracy(root1, testingdata);
        double sum2 = calculateAccuracy(root2, testingdata);
        double sum3 = calculateAccuracy(root3, testingdata);
        double sum4 = calculateAccuracy(root4, testingdata);

        deleteTree(root1);
        deleteTree(root2);
        deleteTree(root3);
        deleteTree(root4);
        accuracySum1 += sum;
        accuracySum2 += sum2;
        accuracySum3 += sum3;
        accuracysum4 += +sum4;
    }
    PrintResult(accuracySum1 / 20, accuracySum2 / 20, accuracySum3 / 20, accuracysum4 / 20);
    return 0;
}