#include "SuffArray.h"

int main(int argc, char const *argv[])
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    
    std::string text;
    std::cin >> text;
    
    std::vector<int> enterings;
    std::vector<int> v;
    
    for (auto ch : text)
        v.push_back(ch - 'a' + 1);
    std::vector<int> suffArray = suffix_array(v);

    int i = 0;
    for (int i = 0; i < suffArray.size(); ++i) {
        suffArray[i]++;
    }
    std::string pattern;
    Search ss(text, suffArray);
    while (std::cin >> pattern) {
        if (pattern.size() <= text.size()) {
            enterings = ss.search(pattern);
            if (enterings.size() != 0) {
                enterings = countSort(enterings);
                std::cout << i + 1 << ": ";
                for (int j = 0; j < enterings.size() - 1; ++j)
                    std::cout << enterings[j] << ", ";
                std::cout << enterings[enterings.size() - 1] << std::endl;
            }
        }
        i++;
    }
    return 0;
}