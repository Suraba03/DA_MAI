#include <bits/stdc++.h>
#include "base.hpp"

int main(int argc, char const *argv[])
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    //std::ofstream fout;
    //std::ifstream fin;

    //for (int counter = 1; counter <= 41; ++counter) {

        std::map<std::string, uf64> mymap;
        // interface
        std::string mark;
        std::string wordChars;
        uf64 value;       // value
        //std::string fileName = "../tests/" + std::to_string(counter);
        //std::ifstream testFile(fileName);
        //std::cout << "test " << counter << " ------------------------------------------------------------\n";
        
        while (std::cin/* testFile */ >> mark) {
            if (mark == "+") {
                std::cin/* testFile */ >> wordChars >> value;
                mymap[Lowercase(wordChars)] = value;
            } else if (mark == "-") {
                std::cin/* testFile */ >> wordChars;
                mymap.erase(Lowercase(wordChars));
            } else {
                mymap.find(Lowercase(mark));
            }
        }
    //}
    return 0;
}