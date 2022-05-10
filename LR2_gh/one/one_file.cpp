#ifndef BASE_
#define BASE_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstring>

const int BINARY_LENGTH_OF_CHAR = 8;

typedef uint_fast64_t uf64;

typedef struct node_t
{ 
    std::string word; // key
    uf64 value;       // value
    int bit;          // starts from -1 for header
    node_t *right;    // if 1
    node_t *left;     // if 0

    int index = -1;   // for save/load
    ~node_t();
    node_t();
    node_t(std::string word, uf64 value, int bit, node_t *left, node_t *right);
    void Initialize(std::string word, uf64 value, int bit, node_t *left, node_t *right);
    void Initialize(std::string word_, uf64 value_, int bit_, node_t *left_);
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

#endif // BASE_

#ifndef PATRICIA_
#define PATRICIA_


class PATRICIA {
private:
    node_t *header;
    uf64 size;

    node_t *ReturnNodePrivate(std::string word,
        node_t *ptr, bool end, node_t *parent);
    
    node_t *ReturnNodePrivateUtil(std::string word,
        node_t *ptr, bool end, int *diffBit, node_t *parent);
    
    node_t *ReturnNodeParentPrivate(std::string word,
        node_t *ptr, bool end, node_t *parent);
    
    node_t *ReturnUpperParentNodePrivate(node_t *searchNode, node_t *ptr,
        bool end, node_t *parent);
    
    node_t *ReturnUpperParentNode(node_t *searchNode);

    node_t *AddNodePrivate(std::string word, uf64 value, node_t *ptr,
        bool end, int diffBit, node_t *parent);

    void DeleteNodeNoLogging(std::string word);

    void TraversePrivate(node_t *ptr);
    
    void FillNodesVectorPrivate(node_t *node, std::vector<node_t *> &nodesVector, int &index);
    
    void DestructTrie(node_t *node);
public:
    PATRICIA();
    node_t *CreateNode(std::string word, uf64 value, int bit);
    node_t *CreateNode(std::string word, uf64 value, int bit, node_t *left, node_t *right);

    node_t *ReturnNode(std::string word);
    node_t *AddNode(std::string word, uf64 value);
    
    void DeleteNode(std::string word);
    
    void Traverse();
    std::vector<node_t *> FillNodesVector();
    void Save(std::ofstream &file, std::vector<node_t *> nodesVector);
    void Load(std::ifstream &file);
    ~PATRICIA();

    void Print(std::vector<node_t *> &nodesVector);
};

#endif // PATRICIA_



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


PATRICIA::PATRICIA()
    : header(NULL),
      size(0) {}

/* void PATRICIA::DestructTrie(node_t *ptr) {
    if (header == NULL) {
        return;
    }
    else {
        if (ptr == header) {d
            if (ptr->left->bit > ptr->bit) {
                DestructTrie(ptr->left);
            }
            DeleteNodeNoLogging(ptr->word);    
        } else {
            if (ptr->left->bit > ptr->bit) {
                DestructTrie(ptr->left);
            }
            DeleteNodeNoLogging(ptr->word);
            if (ptr->right->bit > ptr->bit) {
                DestructTrie(ptr->right);
            }
        }
    }
} */

void PATRICIA::DestructTrie(node_t *ptr) {
    std::vector<node_t *> nodes = FillNodesVector();
    for (auto a : nodes) {
        //std::cout << a->word << " has been deleted\n"; 
        delete a;
    }
    //delete header;
}

PATRICIA::~PATRICIA() {
    DestructTrie(header);
}

node_t *PATRICIA::CreateNode(std::string word, uf64 value, int bit) {
    node_t *temp   = new node_t;
    temp->word     = word;
    temp->value    = value;
    if (bit == -1) {
        temp->left = temp;
    } else {
        temp->left = NULL;
    }
    temp->right    = NULL;
    temp->bit      = bit;

    temp->index    = -1;
    return temp;
}


node_t *PATRICIA::ReturnNode(std::string word) {
    int diffBit;
    return ReturnNodePrivate(word, header, false, header);
}

// Search
node_t *PATRICIA::ReturnNodePrivate(std::string word, node_t *ptr, bool end, node_t *parent) {
    if (header == NULL) {
        return NULL; // std::cout << "the container is empty\n";
    }
    if (ptr == header) {
        if (end) {
            if (ptr->word == word) {
                return ptr;
            } else {
                return NULL;
            }
        }
        if (ptr->left->bit <= ptr->bit) {
            end = true;
        }
        return ReturnNodePrivate(word, ptr->left, end, ptr);
    } else {
        if (end) {
            if (ptr->word == word) {
                return ptr;
            } else {
                return NULL;
            }
        }
        if (/* word[word.length() - 1 - ptr->bit] */ RetrieveBit(word, ptr->bit) == '1') { // -----------------------------------------------------
            if (ptr->right->bit <= ptr->bit) {
                end = true;
            }
            return ReturnNodePrivate(word, ptr->right, end, ptr);
        } else {
            if (ptr->left->bit <= ptr->bit) {
                end = true;
            }
            return ReturnNodePrivate(word, ptr->left, end, ptr);
        }
    }
}

node_t *PATRICIA::ReturnNodePrivateUtil(std::string word, node_t *ptr, bool end, int *diffBit, node_t *parent) {
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
        return ReturnNodePrivateUtil(word, ptr->left, end, diffBit, ptr);
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
            return ReturnNodePrivateUtil(word, ptr->right, end, diffBit, ptr);
        } else {
            if (ptr->left->bit <= ptr->bit) {
                end = true;
            }
            return ReturnNodePrivateUtil(word, ptr->left, end, diffBit, ptr);
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
    node_t *addNode = ReturnNodePrivateUtil(word, header, false, &diffBit, header);
    if (addNode != NULL) {
        return NULL;
    }
    addNode = AddNodePrivate(word, value, header, false, diffBit, header);
    size++;
    return addNode;
}

node_t *PATRICIA::AddNodePrivate(std::string word, uf64 value, node_t *ptr, bool end, int diffBit, node_t *parent) {
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
        }
        return AddNodePrivate(word, value, ptr->left, end, diffBit, ptr);
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
            }
            return AddNodePrivate(word, value, ptr->right, end, diffBit, ptr);
        } else {
            if (ptr->left->bit <= ptr->bit || diffBit <= ptr->left->bit) { // something there...
                end = true;
            }
            return AddNodePrivate(word, value, ptr->left, end, diffBit, ptr);
        }
    }

}

// delete
void PATRICIA::DeleteNode(std::string word) {
    // to optimize
    //int diffBit;
    node_t *aNode = ReturnNode(word); // node to delete
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
        size--;
        return;
    }

    node_t *bNode = ReturnNodeParentPrivate(word, header, false, header); // aNode parent
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
            size--;
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
        size--;
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
            size--;
            return;
        }
        // case 2.1 bNode isn't a leaf, it has two children and one reverse pointer
        else {
            int diffBit2;
            node_t *gNode = ReturnNodeParentPrivate(bNode->word, header, false, header);
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
            size--;
            return;
        } 
    }
}


void PATRICIA::DeleteNodeNoLogging(std::string word) {
    // to optimize
    node_t *aNode = ReturnNode(word); // node to delete
    // case 0 -- doesn't exist
    if (aNode == NULL) {
        return;
    }

    // case 0.1 -- tree consists of only header
    if (header->left == header) {
        delete header;
        header = NULL;
        size--;
        return;
    }

    node_t *bNode = ReturnNodeParentPrivate(word, header, false, header); // aNode parent
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
            size--;
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
        size--;
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
            size--;
            return;
        }
        // case 2.1 bNode isn't a leaf, it has two children and one reverse pointer
        else {
            node_t *gNode = ReturnNodeParentPrivate(bNode->word, header, false, header);
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
            size--;
            return;
        } 
    }
}


node_t *PATRICIA::ReturnNodeParentPrivate(std::string word, node_t *ptr, bool end, node_t *parent) {
    if (header == NULL) {
        return NULL; // std::cout << "the container is empty\n";
    }
    if (ptr == header) {
        if (end) {
            if (ptr->word == word) {
                return parent;
            } else {
                return NULL;
            }
        }
        if (ptr->left->bit <= ptr->bit) {
            end = true;
        }
        return ReturnNodeParentPrivate(word, ptr->left, end, ptr);
    } else {
        if (end) {
            if (ptr->word == word) {
                return parent;
            } else {
                return NULL;
            }
        }
        if (/* word[word.length() - 1 - ptr->bit] */ RetrieveBit(word, ptr->bit) == '1') { // -------------------------------------------------------
            if (ptr->right->bit <= ptr->bit) {
                end = true;
            }
            return ReturnNodeParentPrivate(word, ptr->right, end, ptr);
        } else {
            if (ptr->left->bit <= ptr->bit) {
                end = true;
            }
            return ReturnNodeParentPrivate(word, ptr->left, end, ptr);
        }
    }
}

// traverse
void PATRICIA::Traverse() {
    std::cout << "size = " << size << std::endl;
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
    std::vector<node_t *> nodesVector(size, NULL);
    if (header == NULL) {
        return nodesVector;
    }
    int index = 0;
    FillNodesVectorPrivate(header, nodesVector, index);
    return nodesVector;
}

void PATRICIA::FillNodesVectorPrivate(node_t *node, std::vector<node_t *> &nodesVector, int &index) {
    node->index = index;
    nodesVector[index] = node;
    ++index; 
    if (node == header) {
        if (node->left->bit > node->bit) {
            FillNodesVectorPrivate(node->left, nodesVector, index);
        }
        //nodesVector.push_back(node);
    } else {
        if (node->left->bit > node->bit) {
            FillNodesVectorPrivate(node->left, nodesVector, index);
        }
        //nodesVector.push_back(node);
        if (node->right->bit > node->bit) {
            FillNodesVectorPrivate(node->right, nodesVector, index);
        }
    }
}

void PATRICIA::Print(std::vector<node_t *> &nodesVector) {
    for (auto a : nodesVector)
        std::cout << "[" << a->word << ", " << a->index << "]\n";
}

void PATRICIA::Save(std::ofstream &file, std::vector<node_t *> nodesVector) {
    uf64 sizeV = nodesVector.size();
    file.write((char *)&(sizeV), sizeof(uf64));
    node_t *node;
    //std::cout << "[Save]: " << size << " start...\n";
    for (int i = 0; i < sizeV; ++i) {
        node = nodesVector[i];
        /* if (node->bit != -1)
            std::cout << "[Save]: word = " << node->word << ", length = " << node->word.length() << ", value = "
                << node->value << ", idLeft = " << node->left->index << ", idRight = " << node->right->index << std::endl;
        else
            std::cout << "[Save]: word = " << node->word << ", length = " << node->word.length() << ", value = "
                << node->value << ", idLeft = " << node->left->index << std::endl; */   
        file.write((const char *)&(node->value), sizeof(uf64));
        file.write((const char *)&(node->bit), sizeof(int));
        int len = node->word.length();
        file.write((const char *)&(len), sizeof(int));
        file.write(node->word.c_str(), BINARY_LENGTH_OF_CHAR * len);
        file.write((const char *)&(node->left->index), sizeof(int));
        if (node->bit == -1) {
            int aa = 0;
            file.write((const char *)&(aa), sizeof(int));
        } else {
            file.write((const char *)&(node->right->index), sizeof(int));
        }
    }
}

void PATRICIA::Load(std::ifstream &file) {
    int sizeUtil;
    file.read((char *)&(sizeUtil), sizeof(uf64));
    size = sizeUtil;
    if (!size)
       return;
    int len, bit, idLeft, idRight;
    uf64 value;
    char wordCStyle[256];
    std::string word;


    node_t **nodesVector = new node_t *[size + 1];
    for (int i = 0; i < (size + 1); ++i) {
        if (i == 0) {
            nodesVector[i] = CreateNode(word, value, -1);
            header = nodesVector[i];
        } else {
            nodesVector[i] = CreateNode(word, value, bit);
        }
    }

    //std::cout << "[Load]: " << size << " start...\n";
    for (int i = 0; i < size + 1; ++i) {
        file.read((char *)&(value), sizeof(uf64));
        file.read((char *)&(bit), sizeof(int));
        //int len = node->word.length();
        file.read((char *)&(len), sizeof(int));
        file.read(wordCStyle, BINARY_LENGTH_OF_CHAR * len);
        std::string word(wordCStyle);
        file.read((char *)&(idLeft), sizeof(int));
        //if (bit != -1)
            file.read((char *)&(idRight), sizeof(int));
        /* if (bit != -1)
            std::cout << "[Load]: word = " << word << ", wordCStyle = " << wordCStyle << ", length = " << len
                << ", value = " << value << ", idLeft = " << idLeft << ", idRight = " << idRight << std::endl;
        else
            std::cout << "[Load]: word = " << word << ", wordCStyle = " << wordCStyle << ", length = " << len
                << ", value = " << value << ", idLeft = " << idLeft << std::endl; */
        if (bit == -1)
            nodesVector[i]->Initialize(word, value, bit, nodesVector[idLeft]);
        else
            nodesVector[i]->Initialize(word, value, bit, nodesVector[idLeft], nodesVector[idRight]);
    }
    delete[] nodesVector;
}



int main(int argc, char const *argv[])
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    //for (int counter = 26; counter <= 41; ++counter) {
        PATRICIA patricia;
        std::ofstream fout;
        std::ifstream fin;
        //patricia = new PATRICIA();
        // interface
        std::string mark;
        std::string wordChars;
        uf64 value;       // value

        //std::string fileName = "tests/" + std::to_string(counter);
        //std::ifstream testFile(fileName);
        //std::cout << "test " << counter << " ------------------------------------------------------------\n";
        while (std::cin/* testFile */ >> mark) {
            if (mark == "+") { // full comand: "+ word 34" -- inserts node
                std::cin/* testFile */ >> wordChars >> value;
                std::cout << (patricia.AddNode(Lowercase(wordChars), value) ? "OK" : "Exist");        
                std::cout << '\n';
            } else if (mark == "-") { // full comand: "- word" -- removes node
                std::cin/* testFile */ >> wordChars;
                patricia.DeleteNode(/* StringToBinary(wordChars) */Lowercase(wordChars)); // --------------------------------------------------------
                std::cout << "\n";
            } else if (mark == "!") { // full comands: "! Save /path/to/file" or "! Load /path/to/file"
                std::cin/* testFile */ >> wordChars;
                if (wordChars == "Save") {
                    std::cin/* testFile */ >> wordChars;
                    fout.open(wordChars, std::ios::out | std::ios::binary | std::ios::trunc);
                    patricia.Save(fout, patricia.FillNodesVector());
                    std::cout << "OK\n";
                    fout.close();
                } else if (wordChars == "Load") {
                    std::cin/* testFile */ >> wordChars;
                    fin.open(wordChars, std::ios::in | std::ios::binary);
                    PATRICIA patricia;
                    //patricia.Traverse();

                    patricia.Load(fin);
                    std::cout << "OK\n";
                    fin.close();
                };
            } else { // full comand: "word" -- finds node
                node_t *node = patricia.ReturnNode(Lowercase(mark));
                if (!node)
                    std::cout << "NoSuchWord";
                else
                    std::cout << "OK: " << node->value;
                std::cout << '\n';
            }
/*             std::cout << "traverse:\n";
            patricia.Traverse();
            std::cout << "print:\n";
            std::vector<node_t *> v = patricia.FillNodesVector();
            patricia.Print(v); */

        }
        //std::cout << "\n\n\n\n\n\n\n";
    //}
    return 0;
}