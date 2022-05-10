#ifndef PATRICIA_
#define PATRICIA_

#include "base.hpp"

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