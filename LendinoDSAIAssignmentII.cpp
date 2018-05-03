// THIS IS THE PROVIDED CODE FOR PROGRAM #2, DSA 1, SPRING 2018

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

// A simple class; each object holds four public fields
class Data {
public:
    unsigned int val1;
    unsigned int val2;
    char val3;
    string val4;
};

// Load the data from a specified input file
void loadDataList(list<Data *> &l, const string &filename) {

ifstream input(filename.c_str());
if (!input) {
cerr << "Error: could not open " << filename << endl;
exit(1);
}

// The first line indicates the size
string line;
getline(input, line);
stringstream ss(line);
int size;
ss >> size;

// Load the data
for (int i = 0; i < size; i++) {
getline(input, line);
stringstream ss2(line);
Data *pData = new Data();
ss2 >> pData->val1 >> pData->val2 >> pData->val3 >> pData->val4;
l.push_back(pData);
}

input.close();
}

// Output the data to a specified output file
void writeDataList(const list<Data *> &l, const string &filename) {

    ofstream output(filename.c_str());
    if (!output) {
        cerr << "Error: could not open " << filename << endl;
        exit(1);
    }

    // Write the size first
    int size = l.size();
    output << size << endl;

    // Write the data
    for (list<Data *>::const_iterator ipD = l.begin(); ipD != l.end(); ipD++) {
        output << (*ipD)->val1 << " "
               << (*ipD)->val2 << " "
               << (*ipD)->val3 << " "
               << (*ipD)->val4 << endl;
    }

    output.close();
}

// Sort the data according to a specified field
// (Implementation of this function will be later in this file)
void sortDataList(list<Data *> &, int field);

// The main function calls routines to get the data, sort the data,
// and output the data. The sort is timed according to CPU time.
int main() {
    string filename;
    cout << "Enter name of input file: ";
    cin >> filename;
    list<Data *> theList;
    loadDataList(theList, filename);

    cout << "Data loaded.  Executing sort..." << endl;

    int field = 0;
    cout << "Enter an integer from 1 - 4, representing field to sort: ";
    try {
        cin >> field;
        if (field < 1 || field > 4) {
            cerr << "Error: invalid field" << endl;
            exit(1);
        }
    }
    catch (...) {
        cerr << "Error: invalid field" << endl;
        exit(1);
    }

    clock_t t1 = clock();
    sortDataList(theList, field);
    clock_t t2 = clock();
    double timeDiff = ((double) (t2 - t1)) / CLOCKS_PER_SEC;

    cout << "Sort finished. CPU time was " << timeDiff << " seconds." << endl;

    cout << "Enter name of output file: ";
    cin >> filename;
    writeDataList(theList, filename);

    return 0;
}


// -------------------------------------------------
// YOU MAY NOT CHANGE OR ADD ANY CODE ABOVE HERE !!!
// -------------------------------------------------

// You may add global variables, functions, and/or
// class defintions here if you wish.
/*
struct sortByv2 {
    bool operator()(const Data* A, const Data* B){return A->val2 < B->val2;}
};
*/

struct sortByv4 {
    bool operator()(const Data* A, const Data* B){return A->val4 < B->val4;}
};

void insertionSort(Data **arr, int size){
    int i =1;
    while(i < size){
        auto x = arr[i];
        auto j = i-1;
        while(j >= 0 && arr[j]->val1 > x->val1){
            arr[j+1] = arr[j];
            j = j-1;
        }
        arr[j+1] = x;
        i = i+1;
    }
}

Data** countSort(Data **arr, int size){
    int *count = new int[128];
    for(int i = 0; i < 128; i++){
        count[i] = 0;
    }
    for(int i = 0; i < size; i++){
        count[arr[i]->val3]++;
    }
    //calculate the starting index for each key
    int total = 0;
    for(int i = 0; i <128; i++){
        int oldCount = count[i];
        count[i] = total;
        total += oldCount;
    }
    Data **output = new Data*[size];
    //copy to output array, preserving order of inputs with equal keys
    for(int i = 0; i < size; i++){
        output[count[arr[i]->val3]] = arr[i];
        count[arr[i]->val3] += 1;
    }
    return output;
}

unsigned long getMaximum(Data **arr, int size){
    unsigned long max = arr[0]->val2;
    for(int i =1; i < size; i++){
        if(arr[i]->val2 > max){
            max = arr[i]->val2;
        }
    }
    return max;
}

Data** count2(Data **arr, int size, unsigned long exp, int BASE){
    int *count = new int[BASE];
    Data **output = new Data*[size];
    for(int i = 0; i < BASE; i++){
        count[i] = 0;
    }

    for(int i = 0; i < size; i++){
        count[ (arr[i]->val2/exp)%BASE  ]++;
    }

    for(int i = 1; i < BASE; i++){
        count[i] += count[i-1];
    }

    for(int i = size - 1; i >= 0; i--){
        output[ count[ (arr[i]->val2/exp)%BASE ] - 1] = arr[i];
        count[ (arr[i]->val2/exp)%BASE ] --;
    }
    for(int i = 0; i < size; i++){
        arr[i] = output[i];
    }
}

Data** radix(Data **arr, int size){
    int BASE = 500000;
    unsigned long m = getMaximum(arr, size);
    for(unsigned long exp = 1; m/exp > 0; exp *=BASE){
       count2(arr, size, exp, BASE);
    }
}

void sortDataList(std::list<Data *> &l, int field) {
// Fill in this function
    Data **arr = new Data*[l.size()];
    copy(l.begin(),l.end(),arr);

    if (field == 1) {
        insertionSort(arr, l.size());
        auto it = l.begin();
        int i = 0;
        while(it != l.end()){
            *it = arr[i++];
            it++;
        }
    }

    else if (field == 2) {
        radix(arr, l.size());
        auto it = l.begin();
        int i = 0;
        while(it != l.end()){
            *it = arr[i++];
            it++;
        }

    }

    else if (field == 3) {
        arr = countSort(arr, l.size());
        auto it = l.begin();
        int i = 0;
        while(it != l.end()){
            *it = arr[i++];
            it++;
        }
    }

    else if (field == 4) {
        l.sort(sortByv4());
    }
    else {
        return;
    }
}

