//
//  main.cpp
//  sort
//
//  Created by Xiaoyu Wu on 4/7/19.
//  Copyright © 2019 Xiaoyu Wu. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <vector>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <algorithm>

using namespace std;

struct sortInfo{
    float time;
    int comparisons = 0;
    int movements = 0;
    string sortedFile;
    string algorithm;
    string type;
    int number;
};

vector<string> generateFile();
sortInfo applySort(string unsortedFile, string outputFile);
sortInfo selectionSort(string unsortedFile, string outputFile);
sortInfo insertionSort(string unsortedFile, string outputFile);
sortInfo bubbleSort(string unsortedFile, string outputFile);
sortInfo mergeSort(string unsortedFile, string outputFile);
sortInfo quickSort(string unsortedFile, string outputFile);
void mergeSortFun(vector<int>& mergeList, int left, int right, sortInfo& info);
void merge(vector<int>& mergeList, int left, int middle, int right, sortInfo& info);
void quickSortFun(vector<int>& quickList, int low, int high, sortInfo& info);
int partition(vector<int>& quickList, int low, int high, sortInfo& info);


const int NUMBERFILE = 15;
const string PWD = "/Users/xiaoyuwu/csce221/lab4-sorting/";
//const string PWD = "";
int main() {
    string unsortedFile;
    string outputFile;
    vector<string> files;
    files = generateFile();
    vector<sortInfo> information;
    
    for(int i = 0; i < files.size(); ++i)
    {
        unsortedFile = files.at(i);
        outputFile = unsortedFile.substr(0, unsortedFile.length()-4) + "-output.txt";
        information.push_back(applySort(unsortedFile, outputFile));
    }
    ofstream onFS;
    onFS.open(PWD + "result.txt");
    onFS << "Algorithm," << "Type," << "Number," << "Comparison," << "Movement," << "Time" << endl;
    for(int i = 0; i < information.size(); ++i)
    {
        onFS << information.at(i).algorithm << "," << information.at(i).type << "," << information.at(i).number << "," << information.at(i).comparisons << "," << information.at(i).movements << "," << information.at(i).time << endl;
    }
    onFS.close();
    cout << "Done" << endl;
    return 0;
}

vector<string> generateFile()
{
    srand(1);
    ofstream onFS;
    string outputFile;
    int totalNum;
    vector<string> files;
    vector<string> sortName {"merge"};
 //   vector<string> sortName {"selection", "insertion", "bubble", "merge", "quick"};
    vector<int> numList = {};
    for(int i = 0; i < sortName.size(); ++i)
    {
        for(int j = 1; j <= NUMBERFILE; ++j)
        {
            outputFile = PWD + sortName.at(i) + "-random-" + to_string(j) + ".txt";
            onFS.open(outputFile);
            totalNum = 10000 * j;
            onFS << 4 << endl;
            //onFS << i+1 << endl;
            onFS << totalNum << endl;
            for(int k = 0; k < totalNum; ++k)
            {
                rand();
                int randomNum = rand();
                onFS << randomNum << endl;
                numList.push_back(randomNum);
            }
            onFS.close();
            files.push_back(outputFile);
            outputFile = PWD + sortName.at(i) + "-ordered-" + to_string(j) + ".txt";
            onFS.open(outputFile);
            onFS << 4 << endl;
            //onFS << i+1 << endl;
            onFS << totalNum << endl;
            sort(numList.begin(), numList.end());
            for(int k = 0; k < numList.size(); ++k)
            {
                onFS << numList.at(k) << endl;
            }
            onFS.close();
            files.push_back(outputFile);
            
            outputFile = PWD + sortName.at(i) + "-reverse-" + to_string(j) + ".txt";
            onFS.open(outputFile);
            onFS << 4 << endl;
            //onFS << i+1 << endl;
            onFS << totalNum << endl;
            reverse(numList.begin(), numList.end());
            for(int k = 0; k < numList.size(); ++k)
            {
                onFS << numList.at(k) << endl;
            }
            numList.erase(numList.begin(), numList.end());
            onFS.close();
            files.push_back(outputFile);
        }
       
    }
    return files;
}

sortInfo applySort(string unsortedFile, string outputFile)
{
    sortInfo infos;
    ifstream inFS;
    string fileName = unsortedFile;
    inFS.open(fileName.c_str());
    string content;
    inFS >> content;
    if(content == "1")
    {
        infos = selectionSort(fileName, outputFile);
    }
    else if (content == "2")
    {
        infos = insertionSort(fileName, outputFile);
    }
    else if(content == "3")
    {
        infos = bubbleSort(fileName, outputFile);
    }
    else if(content == "4")
    {
        infos = mergeSort(fileName, outputFile);
    }
    else if(content == "5")
    {
        infos = quickSort(fileName, outputFile);
    }
    else
    {
        cout << "wrong file." << endl;
    }
    return infos;
}

void readData(string unsortedFile, sortInfo& info, vector<int>& dataList)
{
    string algorithmName;
    ifstream inFS;
    inFS.open(unsortedFile.c_str());
    inFS >> algorithmName >> info.number;
    dataList.resize(info.number);
    if(algorithmName == "1")
    {
        info.algorithm = "selection";
    }
    else if(algorithmName == "2")
    {
        info.algorithm = "insertion";
    }
    else if(algorithmName == "3")
    {
        info.algorithm = "bubble";
    }
    else if(algorithmName == "4")
    {
        info.algorithm = "merge";
    }
    else
    {
        info.algorithm = "quick";
    }

    if(unsortedFile.find("random") != string::npos)
    {
        info.type = "random order";
    }
    else if(unsortedFile.find("ordered") != string::npos)
    {
        info.type = "sorted order";
    }
    else if(unsortedFile.find("reverse") != string::npos)
    {
        info.type = "reverse order";
    }
    else
    {
        info.type = "wrong";
    }
    int i = 0;
    while(!inFS.eof())
    {
        string content = "";
        inFS >> content;
        
        if(content != "")
        {
            dataList.at(i) = stoi(content);
        }
        i++;
    }
}
sortInfo selectionSort(string unsortedFile, string outputFile)
{
    sortInfo info;
    vector<int> selectionList;
    readData(unsortedFile, info, selectionList);
    time_t tstart = clock();
    for(int i = 0; i < selectionList.size()-1; ++i)
    {
        int minIndex = i;
        for(int j = i+1; j < selectionList.size(); ++j)
        {
            info.comparisons += 1;
            if(selectionList.at(minIndex) > selectionList.at(j))
            {
                minIndex = j;
            }
        }
        if(minIndex != i)
        {
            swap(selectionList.at(i), selectionList.at(minIndex));
            info.movements += 1;
        }
    }
    float timel = (float)(clock() - tstart)/CLOCKS_PER_SEC;
    info.time = timel;
    ofstream onFS;
    string sortedFileName = outputFile;
    onFS.open(sortedFileName);
    for(int i = 0; i < selectionList.size(); ++i)
    {
        onFS << selectionList.at(i) << endl;
    }
    onFS.close();
    info.sortedFile = sortedFileName;
    return info;
}

sortInfo insertionSort(string unsortedFile, string outputFile)
{
    sortInfo info;
    vector<int> insertionList;
    readData(unsortedFile, info, insertionList);
    time_t tstart = clock();
    for(int i = 1; i < insertionList.size(); ++i)
    {
        int key = insertionList.at(i);
        int j = i - 1;
        while(j >= 0 && insertionList.at(j) > key)
        {
            insertionList.at(j+1) = insertionList.at(j);
            j = j - 1;
            info.comparisons += 1;
            info.movements += 1;
        }
        insertionList.at(j+1) = key;
    }
    float timel = (float)(clock() - tstart)/CLOCKS_PER_SEC;
    info.time = timel;
    ofstream onFS;
    string sortedFileName = outputFile;
    onFS.open(sortedFileName);
    for(int i = 0; i < insertionList.size(); ++i)
    {
        onFS << insertionList.at(i) << endl;
    }
    onFS.close();
    info.sortedFile = sortedFileName;
    return info;
}

sortInfo bubbleSort(string unsortedFile, string outputFile)
{
    sortInfo info;
    vector<int> bubbleList;
    readData(unsortedFile, info, bubbleList);
    time_t tstart = clock();
    for(int i = 0; i < bubbleList.size(); ++i)
    {
        for(int j = 0; j < bubbleList.size()-i-1; ++j)
        {
            info.comparisons += 1;
            if(bubbleList.at(j) > bubbleList.at(j+1))
            {
                int temp = bubbleList.at(j);
                bubbleList.at(j) = bubbleList.at(j+1);
                bubbleList.at(j+1) = temp;
                info.movements += 1;
            }
        }
    }
    float timel = (float)(clock() - tstart)/CLOCKS_PER_SEC;
    info.time = timel;
    ofstream onFS;
    string sortedFileName = outputFile;
    onFS.open(sortedFileName);
    for(int i = 0; i < bubbleList.size(); ++i)
    {
        onFS << bubbleList.at(i) << endl;
    }
    onFS.close();
    info.sortedFile = sortedFileName;
    return info;
}

sortInfo mergeSort(string unsortedFile, string outputFile)
{
    sortInfo info;
    vector<int> mergeList;
    readData(unsortedFile, info, mergeList);
    time_t tstart = clock();
    mergeSortFun(mergeList, 0, mergeList.size()-1, info);
    float timel = (float)(clock() - tstart)/CLOCKS_PER_SEC;
    info.time = timel;
    ofstream onFS;
    string sortedFileName = outputFile;
    onFS.open(sortedFileName);
    for(int i = 0; i < mergeList.size(); ++i)
    {
        onFS << mergeList.at(i) << endl;
    }
    onFS.close();
    info.sortedFile = sortedFileName;
    return info;
}

void mergeSortFun(vector<int>& mergeList, int left, int right, sortInfo& info)
{
    if(left < right)
    {
        int middle = (left+right)/2;
        mergeSortFun(mergeList, left, middle, info);
        mergeSortFun(mergeList, middle+1, right, info);
        merge(mergeList, left, middle, right, info);
    }
}
void merge(vector<int>& mergeList, int left, int middle, int right, sortInfo& info)
{
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;
    vector<int> L(n1);
    vector<int> R(n2);
    for(i = 0; i < n1; ++i)
    {
        L.at(i) = mergeList.at(left+i);
        ++info.movements;
    }
    for(j = 0; j < n2; ++j)
    {
        R.at(j) = mergeList.at(middle + 1 + j);
        ++info.movements;
    }
    i = 0;
    j = 0;
    k = left;
    while(i < n1 && j < n2)
    {
        ++info.comparisons;
        if(L.at(i) <= R.at(j))
        {
            mergeList.at(k) = L.at(i);
            ++info.movements;
            i++;
        }
        else
        {
            mergeList.at(k) = R.at(j);
            ++info.movements;
            j++;
        }
        k++;
    }
    while(i < n1)
    {
        mergeList.at(k) = L.at(i);
        ++info.movements;
        i++;
        k++;
    }
    while(j < n2)
    {
        mergeList.at(k) = R.at(j);
        ++info.movements;
        j++;
        k++;
    }

}

sortInfo quickSort(string unsortedFile, string outputFile)
{
    sortInfo info;
    vector<int> quickList;
    readData(unsortedFile, info, quickList);
    time_t tstart = clock();
    quickSortFun(quickList, 0, quickList.size()-1, info);
    float timel = (float)(clock() - tstart)/CLOCKS_PER_SEC;
    info.time = timel;
    ofstream onFS;
    string sortedFileName = outputFile;
    onFS.open(sortedFileName);
    for(int i = 0; i < quickList.size(); ++i)
    {
        onFS << quickList.at(i) << endl;
    }
    onFS.close();
    info.sortedFile = sortedFileName;
    return info;
}

void quickSortFun(vector<int>& quickList, int low, int high, sortInfo& info)
{
    if (low < high)
    {
        int s = partition(quickList, low, high, info);
        quickSortFun(quickList, low, s-1, info);
        quickSortFun(quickList, s+1, high, info);
    }
}

int partition(vector<int>& quickList, int low, int high, sortInfo& info)
{
    int p = quickList.at(high);
    int i = low - 1;
    for(int j = low; j <= high - 1; j++)
    {
        ++info.comparisons;
        if(quickList.at(j) <= p)
        {
            i++;
            int temp1 = quickList.at(j);
            quickList.at(j) = quickList.at(i);
            quickList.at(i) = temp1;
            ++info.movements;
        }
    }
    int temp2 = quickList.at(high);
    quickList.at(high) = quickList.at(i+1);
    quickList.at(i+1) = temp2;
    ++info.movements;
    return i+1;
}
