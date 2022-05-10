#include "base.hpp"

void DiffBit(std::string word1, std::string word2, int *diffBit) {
    *diffBit = 0;
    while (RetrieveBit(word1, *diffBit) == RetrieveBit(word2, *diffBit)) {
        ++*diffBit;
    }
}

char RetrieveBit(std::string word, int bit) {
    if (bit > BINARY_LENGTH_OF_CHAR * word.length() - 1)
        return '0';
    char letter = word[word.length() - 1 - bit / BINARY_LENGTH_OF_CHAR];
    return CharToBinary(letter)[BINARY_LENGTH_OF_CHAR - 1 - bit % BINARY_LENGTH_OF_CHAR];
}

std::string Lowercase(std::string str) {
    for (int i = 0; i < str.length(); ++i) {
        str[i] = str[i] >= 'A' && str[i] <= 'Z' ? str[i] - 'A' + 'a' : str[i];
    }
    return str;
}

/* std::string CharToBinary(char value) {
    std::string binary = "";
    if (65 <= value && value <= 90)
        value += 32;
    unsigned int code = value;
    int i = 0;
    while (code > 0 && i < 5) {
        if ((code & 1) == 1)
            binary.append("1", 1);
        else
            binary.append("0", 1);
        i++;
        code >>= 1;
    }
    std::reverse(binary.begin(), binary.end());
    return binary;
} */

std::string CharToBinary(char value) {
    std::string binary = "";
    if (65 <= value && value <= 90)
        value += 32;
    unsigned int code = value;
    int i = 0;
    while (code > 0) {
        if ((code & 1) == 1)
            binary.append("1", 1);
        else
            binary.append("0", 1);
        i++;
        code >>= 1;
    }
    binary.push_back('0');
    std::reverse(binary.begin(), binary.end());
    return binary;
}

std::string StringToBinary(std::string word) {
    std::string wordBinary(1280, '0');
    std::string charBinary = "";

    size_t countBinary = word.length() * BINARY_LENGTH_OF_CHAR;
    size_t iter = 1280 - countBinary;

    for (int i = word.length() - 1; i >= 0; --i) {
        charBinary = CharToBinary(word[i]);
        for (int j = 0; j < charBinary.length(); ++j) {
            wordBinary[iter] = charBinary[j];
            iter++;
        }
        charBinary = "";
    }
    return wordBinary;
}

char BinaryToChar(std::string binary) {
    return static_cast<char>(std::stoi(binary, nullptr, 2) + 96);
}

std::string BinaryToString(std::string binary) {
    std::string tempBinary = "";
    std::string charString = "";
    for (int i = 0; i < binary.length(); i += BINARY_LENGTH_OF_CHAR) {
        for (int j = 0; j < BINARY_LENGTH_OF_CHAR; ++j)
            tempBinary.push_back(binary[i + j]);
        if (BinaryToChar(tempBinary) != 96) {
            charString.push_back(BinaryToChar(tempBinary));
            tempBinary = "";
        }
    }
    reverse(charString.begin(), charString.end());
    return charString;
}

node_t::~node_t() {
    //delete this;
}

node_t::node_t() {
    Initialize("", 0, -1, this, this);
}

node_t::node_t(std::string word_, uf64 value_, int bit_, node_t *left_, node_t *right_) {
    word = word_;
    value = value_;
    bit = bit_;
    left = left_;
    if (bit == -1)
        right = NULL;
    else 
        right = right_;
}

void node_t::Initialize(std::string word_, uf64 value_, int bit_, node_t *left_, node_t *right_) {
    word = word_;
    value = value_;
    bit = bit_;
    left = left_;
    if (bit == -1)
        right = NULL;
    else 
        right = right_;
}

void node_t::Initialize(std::string word_, uf64 value_, int bit_, node_t *left_) {
    word = word_;
    value = value_;
    bit = bit_;
    left = left_;
}