#include <iostream>
#include <vector>
#include <climits>
#include <iomanip>
#include <stdio.h>
#include <stdbool.h>
#include <cstdint>

const int LENGTH_OF_POST_INDEX = 6;
bool isEnter = false;

typedef struct TDataItem_ {
    int index;
    uint_fast64_t value;
} TDataItem;

class TData {
public:
    TData(); //
    TData(const TData& other); //
    // these methods have access to private methods from TData class
    friend std::istream& operator>>(std::istream& is, TData& obj); //
    friend std::ostream& operator<<(std::ostream& os, const TData& obj);
    void PrintVector();
    ~TData(); //
    int MaxElement(); //
    TData CountingSort(); //
public:
    std::vector<TDataItem> vector;
};

TData::TData()
{}

TData::TData(const TData& other)
{
    this->vector = other.vector;
}

int TData::MaxElement()
{
    int max = INT_MIN;
    for (int i = 0; i < this->vector.size(); ++i) {
        if (this->vector[i].index > max) {
            max = this->vector[i].index;
        }
    }
    return max;
}

std::istream& operator>>(std::istream& is, TData& inputVector)
{
    TDataItem newObj;
    while (is >> newObj.index >> newObj.value) {
        isEnter = true;
        inputVector.vector.push_back(newObj);
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const TData& obj)
{
    for (auto a : obj.vector) {
        os << std::setw(LENGTH_OF_POST_INDEX) << std::setfill('0')
           << a.index << '\t' << a.value << '\n';
    }
    return os;
}

TData TData::CountingSort()
{
    // declarations
    int maxElement = this->MaxElement();
    std::vector<int> prefixVector(maxElement + 1, 0);
    TData sortedVector(*this);

    // fill in prefixVector with amount of any number in this
    for (int i = 0; i < this->vector.size(); ++i) {
        prefixVector[this->vector[i].index]++;
    }
    
    // create real prefixVector
    for (int i = 1; i < maxElement + 1; ++i) {
        prefixVector[i] += prefixVector[i - 1];
    }

    // fill in the sortedVector
    for (int i = this->vector.size() - 1; i >= 0; --i) {
        sortedVector.vector[prefixVector[this->vector[i].index] - 1].index = this->vector[i].index;
        sortedVector.vector[prefixVector[this->vector[i].index] - 1].value = this->vector[i].value;
        prefixVector[this->vector[i].index]--;
    }
    return sortedVector;
}

TData::~TData()
{}

int main(int argc, char const *argv[])
{
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    TData inputVector;
    std::cin >> inputVector;
    if (!isEnter) {
        return 0;
    }
    std::cout << inputVector.CountingSort();
    return 0;
}
