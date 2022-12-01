#pragma once

#include <iostream>
#include <vector>
#include <climits>
#include <iomanip>
#include <stdio.h>
#include <stdbool.h>
#include <cstdint>
#include <map>
#include <cmath>
#include <algorithm>

class Search
{
private:
    std::string text;
    std::vector<int> suffArray;
public:
    Search(std::string& text, std::vector<int> suffArray): text(text), suffArray(suffArray) {}
    std::vector<int> search(std::string& pattern);
};

bool IsPrefix(std::string pattern, std::string text, int indexSuff);

std::vector<int>& countSort(std::vector<int>& arr);
std::vector<int> lcpInit(const std::string& text, const std::vector<int>& suffArray);
std::vector<int> suffix_array(std::vector<int> &s);