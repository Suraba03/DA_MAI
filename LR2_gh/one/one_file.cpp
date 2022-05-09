#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstring>

const int BINARY_LENGTH_OF_CHAR = 5;

typedef uint_fast64_t uf64;

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

class PATRICIA {
private:
    node_t *header;
    node_t *ReturnNodePrivate(std::string word,
        node_t *ptr, bool end, int *diffBit, node_t *parent); // clear
    node_t *AddNodePrivate(std::string word, uf64 value, node_t *ptr,
        bool end, int diffBit, node_t *parent, bool changeParent); // clear
    node_t *ReturnNodeParentPrivate(std::string word,
        node_t *ptr, bool end, int *diffBit, node_t *parent);
    node_t *ReturnUpperParentNodePrivate(node_t *searchNode, node_t *ptr,
        bool end, node_t *parent);
    node_t *ReturnUpperParentNode(node_t *searchNode);
    void DeleteNodeNoLogging(std::string word);
    void TraversePrivate(node_t *ptr);
    void FillNodesVectorPrivate(node_t *node, std::vector<node_t *> &nodesVector);
public:
    PATRICIA();
    node_t *CreateNode(std::string word, uf64 value, int bit); // clear
    node_t *ReturnNode(std::string word); // clear
    node_t *AddNode(std::string word, uf64 value); // clear
    void DeleteNode(std::string word);
    void Traverse();
    std::vector<node_t *> FillNodesVector();
    void Save(std::ofstream &file, std::vector<node_t *> nodesVector);
    void Load(std::ifstream &file);
    ~PATRICIA();
};

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

std::string CharToBinary(char value) {
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

PATRICIA::PATRICIA()
    : header(NULL) {}

PATRICIA::~PATRICIA() {
    std::vector<node_t *> nodes = FillNodesVector();
    for (auto a : nodes) {
        if (header != NULL)
            DeleteNodeNoLogging(a->word);
    }
}

node_t *PATRICIA::CreateNode(std::string word, uf64 value, int bit) {
    node_t *temp   = new node_t;
    temp->word     = word;
    //temp->wordChar = BinaryToString(word);
    temp->value    = value;
    if (bit == -1) {
        temp->left = temp;
    } else {
        temp->left = NULL;
    }
    temp->right    = NULL;
    temp->bit      = bit;
    return temp;
}

node_t *PATRICIA::ReturnNode(std::string word) {
    int diffBit;
    return ReturnNodePrivate(word, header, false, &diffBit, header);
}

// Search
node_t *PATRICIA::ReturnNodePrivate(std::string word, node_t *ptr, bool end, int *diffBit, node_t *parent) {
    if (header == NULL) {
        *diffBit = -1;
        return NULL; // std::cout << "the container is empty\n";
    }
    if (ptr == header) {
        if (end) {
            if (ptr->word == word) {
                return ptr;
            } else {
                DiffBit(word, ptr->word, diffBit);
                return NULL;
            }
        }
        if (ptr->left->bit <= ptr->bit) {
            end = true;
        }
        return ReturnNodePrivate(word, ptr->left, end, diffBit, ptr);
    } else {
        if (end) {
            if (ptr->word == word) {
                return ptr;
            } else {
                DiffBit(word, ptr->word, diffBit);
                return NULL;
            }
        }
        if (/* word[word.length() - 1 - ptr->bit] */ RetrieveBit(word, ptr->bit) == '1') { // -----------------------------------------------------
            if (ptr->right->bit <= ptr->bit) {
                end = true;
            }
            return ReturnNodePrivate(word, ptr->right, end, diffBit, ptr);
        } else {
            if (ptr->left->bit <= ptr->bit) {
                end = true;
            }
            return ReturnNodePrivate(word, ptr->left, end, diffBit, ptr);
        }
    }
}

node_t *PATRICIA::ReturnUpperParentNode(node_t *searchNode) {
    return ReturnUpperParentNodePrivate(searchNode, header, false, header);
}

node_t *PATRICIA::ReturnUpperParentNodePrivate(node_t *searchNode, node_t *ptr, bool end, node_t *parent) {
    if (header == NULL) { // container is empty
        return NULL;
    }
    // search another time
    if (ptr == header) {
        if (end) {
            return parent;
        }
        if (searchNode->bit <= ptr->left->bit) { // something there...
            end = true;
        }
        return ReturnUpperParentNodePrivate(searchNode, ptr->left, end, ptr);
    } else {
        if (end) {
            return parent;
        }
        if (/* searchNode->word[searchNode->word.length() - 1 - ptr->bit] */RetrieveBit(searchNode->word, ptr->bit) == '1') { // -------------------------------------------------
            if (searchNode->bit <= ptr->right->bit) { // something there...
                end = true;
            }
            return ReturnUpperParentNodePrivate(searchNode, ptr->right, end, ptr);;
        } else {
            if (searchNode->bit <= ptr->left->bit) { // something there...
                end = true;
            }
            return ReturnUpperParentNodePrivate(searchNode, ptr->left, end, ptr);;
        }
    }
}

// Insert

node_t *PATRICIA::AddNode(std::string word, uf64 value) {
    int diffBit;
    node_t *addNode = ReturnNodePrivate(word, header, false, &diffBit, header); // should be included to prev if statement
    if (addNode != NULL) {
        return NULL;    
    }
    return AddNodePrivate(word, value, header, false, diffBit, header, false);;
}

node_t *PATRICIA::AddNodePrivate(std::string word, uf64 value, node_t *ptr, bool end, int diffBit, node_t *parent, bool changeParent) {
    if (header == NULL) { // container is empty
        header = CreateNode(word, value, -1);
        return header;
    }
    // search another time
    if (ptr == header) {
        if (end) {
            // create new node
            node_t *addNode = CreateNode(word, value, diffBit);
            // set pointers
            if (/* addNode->word[addNode->word.length() - 1 - diffBit] */ RetrieveBit(addNode->word, diffBit) == '1') { // ----------------------------------------------------------------
                addNode->right = addNode;
                addNode->left  = ptr;
            } else {
                addNode->left  = addNode;
                addNode->right = ptr;
            }
            // set pointer for parent
            if (ptr == parent->right) {
                parent->right = addNode;
            }
            if (ptr == parent->left) {
                parent->left = addNode;
            }
            return addNode;
        }
        if (ptr->left->bit <= ptr->bit || diffBit <= ptr->left->bit) { // something there...
            end = true;
            if (diffBit <= ptr->left->bit)
                changeParent = true;
        }
        return AddNodePrivate(word, value, ptr->left, end, diffBit, ptr, changeParent);
    } else {
        if (end) {
            // create new node
            node_t *addNode = CreateNode(word, value, diffBit);
            // set pointers
            if (/* addNode->word[addNode->word.length() - 1 - diffBit] */ RetrieveBit(addNode->word, diffBit) == '1') { // -----------------------------------------------------------------------
                addNode->right = addNode;
                addNode->left  = ptr;
            } else {
                addNode->left  = addNode;
                addNode->right = ptr; 
            }
            // set pointer for parent
            if (ptr == parent->right) {
                parent->right = addNode;
            }
            if (ptr == parent->left) {
                parent->left = addNode;
            }
            return addNode;
        }
        if (/* word[word.length() - 1 - ptr->bit] */ RetrieveBit(word, ptr->bit) == '1') { // -------------------------------------------------------------------------
            if (ptr->right->bit <= ptr->bit || diffBit <= ptr->right->bit) { // something there...
                end = true;
                if (diffBit <= ptr->right->bit)
                    changeParent = true;
            }
            return AddNodePrivate(word, value, ptr->right, end, diffBit, ptr, changeParent);
        } else {
            if (ptr->left->bit <= ptr->bit || diffBit <= ptr->left->bit) { // something there...
                end = true;
                if (diffBit <= ptr->left->bit)
                    changeParent = true;
            }
            return AddNodePrivate(word, value, ptr->left, end, diffBit, ptr, changeParent);
        }
    }

}

// delete
void PATRICIA::DeleteNode(std::string word) {
    // to optimize
    int diffBit;
    node_t *aNode = ReturnNodePrivate(word, header, false, &diffBit, header); // node to delete
    // case 0 -- doesn't exist
    if (aNode == NULL) {
        std::cout << "NoSuchWord";
        return;
    }

    // case 0.1 -- tree consists of only header
    if (header->left == header) {
        std::cout << "OK";
        delete header;
        header = NULL;
        return;
    }

    int diffBit1;
    node_t *bNode = ReturnNodeParentPrivate(word, header, false, &diffBit1, header); // aNode parent
    // case 1 -- aNode == bNode
    if (aNode == bNode) {
        node_t *aNodeParent = ReturnUpperParentNode(aNode);
        if (aNodeParent == header) {
            if (aNode->left != aNode)
                aNodeParent->left = aNode->left;
            if (aNode->right != aNode)
                aNodeParent->left = aNode->right;
            delete aNode;
            aNode = NULL;
            std::cout << "OK";
            return;
        }
        if (aNode->right != aNode) {
            if (aNodeParent->right == aNode)
                aNodeParent->right = aNode->right;
            else
                aNodeParent->left = aNode->right;
            //aNode->right->parent.... = aNodeParent; // right subtree parent pointer 
        } else {
            if (aNodeParent->right == aNode)
                aNodeParent->right = aNode->left;
            else
                aNodeParent->left = aNode->left;
            //aNode->left->parent.... = aNodeParent; // left subtree parent pointer 
        }
        delete aNode;
        aNode = NULL;
        std::cout << "OK";
        return;
    }

    // case 2 -- aNode != bNode
    else {
        aNode->value = bNode->value;
        aNode->word  = bNode->word;
        node_t *bNodeParent = ReturnUpperParentNode(bNode);
        // case 2.1 bNode is a leaf
        if (bNode->left->bit <= bNode->bit && bNode->right->bit <= bNode->bit) {
            if (bNodeParent->right == bNode) {
                if (bNode->right != bNode)
                    bNodeParent->right = bNode->right;
                else
                    bNodeParent->right = bNode->left;
            } else {
                if (bNode->right != bNode)
                    bNodeParent->left = bNode->right;
                else
                    bNodeParent->left = bNode->left;       
            }
            delete bNode;
            bNode = NULL;
            std::cout << "OK";
            return;
        }
        // case 2.1 bNode isn't a leaf, it has two children and one reverse pointer
        else {
            int diffBit2;
            node_t *gNode = ReturnNodeParentPrivate(bNode->word,
                header, false, &diffBit2, header);
            if (bNode->right == aNode) { // left subtree
                // gNode and aNode case
                if (gNode->right == bNode) {
                    gNode->right = aNode;
                } else {
                    gNode->left = aNode;
                }
                // parent and subtree case       
                if (bNodeParent->right == bNode) {
                    bNodeParent->right = bNode->left;
                } else {
                    bNodeParent->left = bNode->left;
                }
                //bNode->left->parent.... = bNodeParent;
            } else { // right subtree
                // gNode and aNode case
                if (gNode->right == bNode) {
                    gNode->right = aNode;
                } else {
                    gNode->left = aNode;
                }   
                // parent and subtree case   
                if (bNodeParent->right == bNode) {
                    bNodeParent->right = bNode->right;
                } else {
                    bNodeParent->left = bNode->right;
                }
                //bNode->right->parent.... = bNodeParent;    
            }
            std::cout << "OK";
            delete bNode;
            bNode = NULL;
            return;
        } 
    }
}


void PATRICIA::DeleteNodeNoLogging(std::string word) {
    // to optimize
    int diffBit;
    node_t *aNode = ReturnNodePrivate(word, header, false, &diffBit, header); // node to delete
    // case 0 -- doesn't exist
    if (aNode == NULL) {
        return;
    }

    // case 0.1 -- tree consists of only header
    if (header->left == header) {
        delete header;
        header = NULL;
        return;
    }

    int diffBit1;
    node_t *bNode = ReturnNodeParentPrivate(word, header, false, &diffBit1, header); // aNode parent
    // case 1 -- aNode == bNode
    if (aNode == bNode) {
        node_t *aNodeParent = ReturnUpperParentNode(aNode);
        if (aNodeParent == header) {
            if (aNode->left != aNode)
                aNodeParent->left = aNode->left;
            if (aNode->right != aNode)
                aNodeParent->left = aNode->right;
            delete aNode;
            aNode = NULL;
            return;
        }
        if (aNode->right != aNode) {
            if (aNodeParent->right == aNode)
                aNodeParent->right = aNode->right;
            else
                aNodeParent->left = aNode->right;
            //aNode->right->parent.... = aNodeParent; // right subtree parent pointer 
        } else {
            if (aNodeParent->right == aNode)
                aNodeParent->right = aNode->left;
            else
                aNodeParent->left = aNode->left;
            //aNode->left->parent.... = aNodeParent; // left subtree parent pointer 
        }
        delete aNode;
        aNode = NULL;
        return;
    }

    // case 2 -- aNode != bNode
    else {
        aNode->value = bNode->value;
        aNode->word  = bNode->word;
        node_t *bNodeParent = ReturnUpperParentNode(bNode);
        // case 2.1 bNode is a leaf
        if (bNode->left->bit <= bNode->bit && bNode->right->bit <= bNode->bit) {
            if (bNodeParent->right == bNode) {
                if (bNode->right != bNode)
                    bNodeParent->right = bNode->right;
                else
                    bNodeParent->right = bNode->left;
            } else {
                if (bNode->right != bNode)
                    bNodeParent->left = bNode->right;
                else
                    bNodeParent->left = bNode->left;       
            }
            delete bNode;
            bNode = NULL;
            return;
        }
        // case 2.1 bNode isn't a leaf, it has two children and one reverse pointer
        else {
            int diffBit2;
            node_t *gNode = ReturnNodeParentPrivate(bNode->word,
                header, false, &diffBit2, header);
            if (bNode->right == aNode) { // left subtree
                // gNode and aNode case
                if (gNode->right == bNode) {
                    gNode->right = aNode;
                } else {
                    gNode->left = aNode;
                }
                // parent and subtree case       
                if (bNodeParent->right == bNode) {
                    bNodeParent->right = bNode->left;
                } else {
                    bNodeParent->left = bNode->left;
                }
                //bNode->left->parent.... = bNodeParent;
            } else { // right subtree
                // gNode and aNode case
                if (gNode->right == bNode) {
                    gNode->right = aNode;
                } else {
                    gNode->left = aNode;
                }   
                // parent and subtree case   
                if (bNodeParent->right == bNode) {
                    bNodeParent->right = bNode->right;
                } else {
                    bNodeParent->left = bNode->right;
                }
                //bNode->right->parent.... = bNodeParent;    
            }
            delete bNode;
            bNode = NULL;
            return;
        } 
    }
}


node_t *PATRICIA::ReturnNodeParentPrivate(std::string word, node_t *ptr, bool end, int *diffBit, node_t *parent) {
    if (header == NULL) {
        *diffBit = -1;
        return NULL; // std::cout << "the container is empty\n";
    }
    if (ptr == header) {
        if (end) {
            if (ptr->word == word) {
                return parent;
            } else {
                DiffBit(word, ptr->word, diffBit);
                return NULL;
            }
        }
        if (ptr->left->bit <= ptr->bit) {
            end = true;
        }
        return ReturnNodeParentPrivate(word, ptr->left, end, diffBit, ptr);
    } else {
        if (end) {
            if (ptr->word == word) {
                return parent;
            } else {
                DiffBit(word, ptr->word, diffBit);
                return NULL;
            }
        }
        if (/* word[word.length() - 1 - ptr->bit] */ RetrieveBit(word, ptr->bit) == '1') { // -------------------------------------------------------
            if (ptr->right->bit <= ptr->bit) {
                end = true;
            }
            return ReturnNodeParentPrivate(word, ptr->right, end, diffBit, ptr);
        } else {
            if (ptr->left->bit <= ptr->bit) {
                end = true;
            }
            return ReturnNodeParentPrivate(word, ptr->left, end, diffBit, ptr);
        }
    }
}

// traverse
void PATRICIA::Traverse() {
    TraversePrivate(header);
}

void PATRICIA::TraversePrivate(node_t *ptr) {
    if (header == NULL) {
        return;
    }
    else {
        if (ptr == header) {
            if (ptr->left->bit > ptr->bit) {
                TraversePrivate(ptr->left);
            }
            std::cout << "[" << ptr->word << ", "
                << ptr->value << ", " << ptr->bit << ", left - " << ptr->left->word << "]\n";
        } else {
            if (ptr->left->bit > ptr->bit) {
                TraversePrivate(ptr->left);
            }
            std::cout << "[" << ptr->word << ", "
                << ptr->value << ", " << ptr->bit << ", left - " << ptr->left->word
                << ", right - " << ptr->right->word << "]\n";
            if (ptr->right->bit > ptr->bit) {
                TraversePrivate(ptr->right);
            }
        }
    }
}

// save

std::vector<node_t *> PATRICIA::FillNodesVector() {
    std::vector<node_t *> nodesVector;
    if (header == NULL) {
        //std::cout << "Trie is empty\n";
        return nodesVector;
    }
    FillNodesVectorPrivate(header, nodesVector);
    return nodesVector;
}

void PATRICIA::FillNodesVectorPrivate(node_t *node, std::vector<node_t *> &nodesVector) {
    if (node == header) {
        if (node->left->bit > node->bit) {
            FillNodesVectorPrivate(node->left, nodesVector);
        }
        nodesVector.push_back(node);
    } else {
        if (node->left->bit > node->bit) {
            FillNodesVectorPrivate(node->left, nodesVector);
        }
        nodesVector.push_back(node);
        if (node->right->bit > node->bit) {
            FillNodesVectorPrivate(node->right, nodesVector);
        }
    }
}

void PATRICIA::Save(std::ofstream &file, std::vector<node_t *> nodesVector) {
    uf64 size = nodesVector.size();
    file.write((char *)&(size), sizeof(uf64));
    for (auto a : nodesVector) {
        char wordCStyle[256];
        strcpy(wordCStyle, a->word.c_str());
        int len = strlen(wordCStyle);
        //std::cout << "[Save]: word = " << a->word << ", wordCStyle = " << wordCStyle << ", length = " << len << ", value = " << a->value << std::endl;
        file.write((char *)&(len), sizeof(int));
        file.write(wordCStyle, BINARY_LENGTH_OF_CHAR * len);
        file.write((char *)&(a->value), sizeof(uf64));
    }
}

void PATRICIA::Load(std::ifstream &file) {
    uf64 size;
    file.read((char *)&(size), sizeof(uf64));
    //std::cout << "[Load]: " << size << " start...\n";
    for (int i = 0; i < size; ++i) {
        int len;
        uf64 value;
        file.read((char *)&(len), sizeof(int));
        //std::cout << "length" << len << std::endl;
        char wordCStyle[256];
        file.read(wordCStyle, BINARY_LENGTH_OF_CHAR * len);
        file.read((char *)&(value), sizeof(uf64));
        std::string word(wordCStyle);
        //std::cout << "[Load]: word = " << word << ", wordCStyle = " << wordCStyle << ", length = " << len << ", value = " << value << std::endl;
        node_t *newNode = AddNode(word, value);
    }
}

int main(int argc, char const *argv[])
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    std::ofstream fout;
    std::ifstream fin;

    PATRICIA *patricia = new PATRICIA();
    // interface
    std::string mark;
    std::string wordChars;
    uf64 value;       // value
    //std::ifstream testFile("33");
    while (std::cin/* testFile */ >> mark) {
        if (mark == "+") { // full comand: "+ word 34" -- inserts node
            std::cin/* testFile */ >> wordChars >> value;
            node_t *addedNode = patricia->AddNode(/* StringToBinary(wordChars) */Lowercase(wordChars), value); // --------------------------------
            if (addedNode == NULL) {
                std::cout << "Exist";
            } else {
                std::cout << "OK";
            }
            std::cout << "\n";
        } else if (mark == "-") { // full comand: "- word" -- removes node
            std::cin/* testFile */ >> wordChars;
            patricia->DeleteNode(/* StringToBinary(wordChars) */Lowercase(wordChars)); // --------------------------------------------------------
            std::cout << "\n";
        } else if (mark == "!") { // full comands: "! Save /path/to/file" or "! Load /path/to/file"
            std::cin/* testFile */ >> wordChars;

            if (wordChars == "Save") {
                std::cin/* testFile */ >> wordChars;
                fout.open(wordChars, std::ios::out | std::ios::binary | std::ios::trunc);
                patricia->Save(fout, patricia->FillNodesVector());
                std::cout << "OK\n";
                fout.close();

            } else if (wordChars == "Load") {
                std::cin/* testFile */ >> wordChars;
                fin.open(wordChars, std::ios::in | std::ios::binary);
                delete patricia;
                patricia = new PATRICIA();

                patricia->Load(fin);
                std::cout << "OK\n";
                fin.close();
            };
        } else { // full comand: "word" -- finds node
            node_t *foundNode = patricia->ReturnNode(/* StringToBinary(mark) */Lowercase(mark)); // ------------------------------------------
            if (foundNode == NULL) {
                std::cout << "NoSuchWord";
            } else {
                std::cout << "OK: " << foundNode->value;
            }
            std::cout << "\n";
        }
        //patricia->Traverse();
    }
    delete patricia;
    return 0;
}
