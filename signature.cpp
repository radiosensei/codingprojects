#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <fstream>

using namespace std;

const int SIZE = 1000;



void writeBinary(string filename, int* values, int length)
{
    ofstream out_file;
    out_file.open(filename, ios::binary);
    out_file.write(reinterpret_cast<char*>(&length), sizeof(int));
    for (int i = 0;i < length;i++)
    {
        out_file.write(reinterpret_cast<char*>(&values[i]), sizeof(int));
    }
}

void createBinaryFile(string filename)
{
    int* nums = new int[SIZE];
    for (int i = 0;i < SIZE;i++)
    {
        nums[i] = rand() % 1000;
    }
    writeBinary(filename, nums, SIZE);
}


class Analyzer {
protected:
    int* values;
    int size;
    int* cloneValues(int* vals, int s);

public:
    Analyzer(int* values, int size)
    {
        this->size = size;
        this->values = cloneValues(values, size);
        
    }
    virtual ~Analyzer() { delete[]values; }

    virtual string analyze() = 0;
};
int* Analyzer::cloneValues(int* vals, int s)
{
    int* copy = new int[s];
    for (int i = 0;i < s;i++)
    {
        copy[i] = vals[i];
        
    }
     return copy;
    
}

// ------------------------ Selection Sort ------------------------

void selection_sort(int* arr, int size) {
    for (int i = 0; i < size - 1; ++i) {
        int min = i;
        for (int j = i + 1; j < size; ++j) {
            if (arr[j] < arr[min])
                min = j;
        }
        swap(arr[i], arr[min]);
    }
}
//-----------------------Binary Reader-------------------------


class BinaryReader
{
private:
    int* values;
    int size;
    
public:
    BinaryReader(string filename)
    {
        readValues(filename);
    }
    ~BinaryReader()
    {
        delete[] values;
    }
    int* getValues() const
    {
        return values;
    }
    int GetSize() const
    {
        return size;
    }
    void readValues(string filename)
    {
        ifstream in_file;

        in_file.open(filename, ios::binary);

        in_file.read(reinterpret_cast<char*>(&size), sizeof(int));

        values = new int[size];

        for (int i = 0;i < size;i++)
        {
            in_file.read(reinterpret_cast<char*>(&values[i]), sizeof(int));
        }
    }

 };

// ------------------------ Binary Search ------------------------

int binary_search_recursive(int* arr, int target, int left, int right) 
{
    if (left > right)
        return -1;

    int mid = left + (right - left) / 2;
    if (arr[mid] == target)
        return mid;
    else if (arr[mid] > target)
        return binary_search_recursive(arr, target, left, mid - 1);
    else
        return binary_search_recursive(arr, target, mid + 1, right);
}

int binary_search(int* arr, int target, int size)
{
    return binary_search_recursive(arr, target, 0, size - 1);
}

// ------------------------ SearchAnalyzer ------------------------

class SearchAnalyzer : public Analyzer
{
public:
    SearchAnalyzer(int* values, int size) : Analyzer(values, size) {
        selection_sort(this->values, this->size);
    }

    string analyze() override 
    {
        int count = 0;

        for (int i = 0; i < 100; i++)
        {
            int randVal = rand() % 1000;

            if (binary_search(values, randVal, size) != -1) {
                count++;
            }
        }

        return "SearchAnalyzer: " + to_string(count) + " values found.";
    }
};

//-----------------------Duplicate Analyzer-------------------------
class DuplicatesAnalyzer :
    public Analyzer
{
public:
    DuplicatesAnalyzer(int* vals, int size)
        :Analyzer(vals, size) {
    }

    string analyze() override
    {
        int* counts = new int[size];
        for (int i = 0;i < size;i++)

            counts[i] = 0;


        for (int j = 0;j < size;j++)

            counts[values[j]] += 1;
        int dup = 0;
        for (int k = 0;k < size;k++)

            if (counts[k] > 1)
                dup += 1;


        string res = "There are " + std::to_string(dup) + " in the random intergers. \n";

        delete[] counts;
        return res;
    }

};


//------------------------Missing Analyzer---------------------------
class missingAnalyzer:public Analyzer
{
public:
    missingAnalyzer(int* vals, int s)
        :Analyzer(vals, s) {
    }

    
    string analyze() override
    {
        int* counts = new int[1000];
        for (int i = 0; i < 1000;i++)
            counts[i] = 0;
        for (int i = 0; i < 1000; i++)
            counts[values[i]] += 1;
        int missings = 0;
        for (int i = 0; i < 1000;i++)
                if (counts[i] == 0)
                missings++;

        string res = "There are " + to_string(missings) + " values that do not appear in the array. \n";
        return res;
    }

};

// ------------------------ StatisticsAnalyzer ------------------------

class StatisticsAnalyzer : public Analyzer 
{
public:
    StatisticsAnalyzer(int* vals, int size) : Analyzer(vals, size) {}

    string analyze() override
    {
        selection_sort(values, size);

        int min = values[0];
        int max = values[size - 1];

        double median = 0.0;
        if (size % 2 == 0)
            median = (values[size / 2 - 1] + values[size / 2]) / 2.0;
        else
            median = values[size / 2];

        unordered_map<int, int> freqMap;
        int mode = values[0], maxFreq = 0;

        for (int i = 0; i < size; ++i) 
        {
            freqMap[values[i]]++;

            if (freqMap[values[i]] > maxFreq) 
            {
                maxFreq = freqMap[values[i]];
                mode = values[i];
            }
        }

        string result = "StatisticsAnalyzer:\n";
        result += "  Min: " + std::to_string(min) + "\n";
        result += "  Max: " + std::to_string(max) + "\n";
        result += "  Median: " + std::to_string(median) + "\n";
        result += "  Mode: " + std::to_string(mode) + "\n";
        return result;
    }
};

// ------------------------ Main for Testing ------------------------

int main()
{
    srand(time(0));
    string filename = "binary.dat";
    createBinaryFile(filename);
    BinaryReader br(filename);
    vector<Analyzer*> ans;
    ans.push_back(new DuplicatesAnalyzer(br.getValues(), br.GetSize()));
    ans.push_back(new missingAnalyzer(br.getValues(), br.GetSize()));
    ans.push_back(new StatisticsAnalyzer(br.getValues(), br.GetSize()));

    for (auto an : ans)
    {
        cout << an->analyze();
        delete an;
    }

    return 0;
}
