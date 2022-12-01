#ifndef BASE_
#define BASE_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstring>

const int BINARY_LENGTH_OF_CHAR = 5;

typedef unsigned long long uf64;

typedef struct node_t
{ 
    std::string word; // key
    uf64 value;       // value
    int bit;          // starts from -1 for header
    node_t *right;    // if 1
    node_t *left;     // if 0
} node_t;

typedef struct node_light_t
{ 
    std::string word; // key
    uf64 value;       // value
} node_light_t;

void DiffBit(std::string word1, std::string word2, int *i);
char RetrieveBit(std::string word, int bit);
std::string Lowercase(std::string str);
std::string CharToBinary(char value);
std::string StringToBinary(std::string word);
char BinaryToChar(std::string binary);
std::string BinaryToString(std::string binary);

bool is_empty(std::ifstream& pFile);

#endif // BASE_