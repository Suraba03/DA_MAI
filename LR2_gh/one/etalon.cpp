#include <iostream>
#include <fstream>
#include <cstring>

// ???????? ?? ????????? ?????
static inline bool Equal(char *a, char *b)
{
    if (a == 0 || b == 0)
        return 0;
    return (strcmp(a, b) == 0);
}

// ???????? bit-?? ?? ????? ??? ?????
static inline int GetBit(char *key, int bit)
{
    if (bit < 0)
        bit = 0;
    return (key[bit / 8] >> (7 - (bit % 8))) & 1;
}

static inline int FirstDifBit(char *a, char *b)
{
    if (a == 0 || b == 0)
        return 0;

    int i = 0;

    // ??????????? ?? ??????
    while (a[i] == b[i])
        i++;
    i *= 8;

    // ???????: ??????????? ?? ???? ? ????????? ?????
    while (GetBit(a, i) == GetBit(b, i))
        i++;

    return i;
}

static inline void Lowercase(char *str) {
    for (int i = 0; i < strlen(str); ++i) {
        str[i] = str[i] >= 'A' && str[i] <= 'Z' ? str[i] - 'A' + 'a' : str[i];
    }
}

const int MAXLEN = 256;
typedef unsigned long long TValue;
typedef char TKey;

struct TNode
{
    int id = -1;
    int bit;

    TKey *key;
    TValue value;

    TNode *left;
    TNode *right;

    void Initialize(int b, TKey *k, TValue v, TNode *l, TNode *r)
    {
        bit = b;
        if (k)
        {
            key = new char[strlen(k) + 1];
            strcpy(key, k);
        }
        else
            key = k;
        value = v;
        left = l;
        right = r;
    }

    TNode()
    {
        Initialize(-1, 0, 0, this, this);
    }

    TNode(int b, TKey *k, TValue v)
    {
        Initialize(b, k, v, this, this);
    }

    TNode(int b, TKey *k, TValue v, TNode *l, TNode *r)
    {
        Initialize(b, k, v, l, r);
    }

    ~TNode()
    {
        delete [] key;
    }
};

struct TTrie
{
    TNode *root;
    int size;

    TTrie()
    {
        root = new TNode();
        size = 0;
    }

    void DestructR(TNode *node)
    {
        if (node->left->bit > node->bit)
            DestructR(node->left);
        if (node->right->bit > node->bit)
            DestructR(node->right);
        delete node;
    }

    ~TTrie()
    {
        DestructR(root);
    }

    TNode *Find(TKey *key)
    {
        TNode *p = root;
        TNode *q = root->left;

        while (p->bit < q->bit)
        {
            p = q;
            q = (GetBit(key, q->bit) ? q->right : q->left);
        }

        if (!Equal(key, q->key))
            return 0;

        return q;
    }

    TNode *Insert(TKey *key, TValue value)
    {
        TNode *p = root;
        TNode *q = root->left;
        while (p->bit < q->bit)
        {
            p = q;
            q = (GetBit(key, q->bit) ? q->right : q->left);
        }

        if (Equal(key, q->key))
            return 0;

        int lBitPos = FirstDifBit(key, q->key);

        p = root;
        TNode *x = root->left;

        while (p->bit < x->bit && x->bit < lBitPos)
        {
            p = x;
            x = (GetBit(key, x->bit) ? x->right : x->left);
        }

        try
        {
            q = new TNode();
        }
        catch (const std::bad_alloc &e)
        {
            std::cout << "ERROR: fail to allocate the requested storage space\n";
            return 0;
        }

        q->Initialize(lBitPos, key, value,
                      (GetBit(key, lBitPos) ? x : q),
                      (GetBit(key, lBitPos) ? q : x));

        if (GetBit(key, p->bit))
            p->right = q;
        else
            p->left = q;

        size++;
        return q;
    }

    void KVCopy(TNode *src, TNode *dest)
    {
        if (strlen(dest->key) < strlen(src->key))
        {
            delete[] dest->key;
            dest->key = new char[strlen(src->key) + 1];
        }
        strcpy(dest->key, src->key);

        dest->value = src->value;
    }

    bool Delete(TKey *k)
    {
        // ?????? ?????????? ???? pp, ???????? p ? ??? ??? t (???? ????????? ???????)
        TNode *p, *t, *pp = 0;

        p = root;
        t = (p->left);

        // ?????? pp, p ? t
        while (p->bit < t->bit)
        {
            pp = p;
            p = t;
            t = (GetBit(k, t->bit) ? t->right : t->left);
        }

        // ???? ????? ????????-?? ? ??? -- ???????
        if (!Equal(k, t->key))
            return false;

        TNode *x, *r;
        char *key;

        // ???? p == t, ?? ? t ???? ???????????. ? ????? ?????? ?????????? ????
        // ????????????? ? ???????? (pp) "????????" ????????? t, ??????? ?? ???????????
        if (p != t)
        {
            // ????? ??, ?????? ???? ? ????. p ? t, ????? ????? ??????? ?????? p, ? ?? t
            KVCopy(p, t);

            key = p->key;
            r = p;
            x = (GetBit(key, p->bit) ? p->right : p->left);

            // ???? ????, ??? ?? p ???????????? (????? ?????? ? r; ? ?, ?? ????, ????? ? ???????? ????????? p)
            while (r->bit < x->bit)
            {
                r = x;
                x = (GetBit(key, x->bit) ? x->right : x->left);
            }

            // ? ?????? ??????????? ?? p, ????? ????????????? ?? t
            if (GetBit(key, r->bit))
                r->right = t;
            else
                r->left = t;
        }

        // ???????? ????????? ? ???????? p (pp) "????????" ????????? p, ??????? ?? ???????????
        TNode *ch = (GetBit(k, p->bit) ? p->left : p->right);
        if (GetBit(k, pp->bit))
            pp->right = ch;
        else
            pp->left = ch;

        // ? ?????????? ??????? p: ?????? ?? ???? ????? ?? ?????????, ???? ?? ??????????
        // ? ?? ??????????? ?? ????, ? ?? ????????????? (pp) ?????????? ??????
        delete p;

        size--;

        return true;
    }

    void Save(std::ofstream &file)
    {
        // ?????? ?????? ??????
        file.write((const char *)&(size), sizeof(int));

        // ???????????? ?????, ????????????? ??????? ??????????
        int index = 0;
        TNode **nodes;
        try
        {
            nodes = new TNode *[size + 1];
        }
        catch (const std::bad_alloc &e)
        {
            std::cout << "ERROR: fail to allocate the requested storage space\n";
            return;
        }
        enumerate(root, nodes, index);

        // ?????? ?????? ??????????????? (??? ??? ?????? ? enumerate)
        // ?????? ??? ???? ?? ?????, ?? ?????? ?????????? left/right ??????
        // ???? ????? (?????? ??? ???? ??? ?????? ? enumerate) left/right
        TNode *node;
        for (int i = 0; i < (size + 1); ++i)
        {
            node = nodes[i];
            file.write((const char *)&(node->value), sizeof(TValue));
            file.write((const char *)&(node->bit), sizeof(int));
            int len = node->key ? strlen(node->key) : 0;
            file.write((const char *)&(len), sizeof(int));
            file.write(node->key, sizeof(char) * len);
            file.write((const char *)&(node->left->id), sizeof(int));
            file.write((const char *)&(node->right->id), sizeof(int));
        }
        delete[] nodes;
    }

    void enumerate(TNode *node, TNode **nodes, int &index)
    {
        // ?????, ??? index ?????????? ?? ??????: ???? ????? ?? ????? ???????????
        node->id = index;
        nodes[index] = node;
        ++index;
        if (node->left->bit > node->bit)
        {
            enumerate(node->left, nodes, index);
        }
        if (node->right->bit > node->bit)
        {
            enumerate(node->right, nodes, index);
        }
    }

    void Load(std::ifstream &file)
    {
        // ????????? ??????
        int n;
        file.read((char *)&n, sizeof(int));
        size = n;
        // ???? ?? ???? - ???????
        if (!size)
            return;

        TNode **nodes = new TNode *[size + 1];
        // ??? ??? ?????????????????, ????? ?? ????? ??????? new Trie()
        // ??????? ????? ?????? ????????
        nodes[0] = root;
        for (int i = 1; i < (size + 1); ++i)
            // ? ??? ?????? ???? ???? ????????
            nodes[i] = new TNode();

        // ???? ?????, ??????? ??? ????????? ?????????
        int bit;
        int len;
        TKey *key = 0;
        TValue value;
        int idLeft, idRight;

        for (int i = 0; i < (size + 1); ++i)
        {
            file.read((char *)&(value), sizeof(TValue));
            file.read((char *)&(bit), sizeof(int));
            file.read((char *)&(len), sizeof(int));
            if (len)
            {
                key = new char[len + 1];
                key[len] = 0;
            }
            file.read(key, len);
            // ????????? ????????? ? ??? ?? ???????, ??? ? ?????? ? Load-e
            // ???? ?????-??????? ????? ????????? ???? ???????, ? ?????? ????????? ????? ??
            file.read((char *)&(idLeft), sizeof(int));
            file.read((char *)&(idRight), sizeof(int));
            nodes[i]->Initialize(bit, key, value, nodes[idLeft], nodes[idRight]);
            delete[] key;
        }

        delete[] nodes;

        return;
    }

#define safeKey(node) (node->key ? node->key : "root")

    void PrintDefinitions(TNode *node, std::ofstream &out)
    {
        out << ' ' << safeKey(node) << "[label=\"" << safeKey(node) << ", " << node->bit << "\"];\n";
        if (node->left->bit > node->bit)
            PrintDefinitions(node->left, out);
        if (node->right->bit > node->bit)
            PrintDefinitions(node->right, out);
    }

    void PrintRelations(TNode *node, std::ofstream &out)
    {
        if (node->left->bit > node->bit)
        {
            out << ' ' << safeKey(node) << "->" << safeKey(node->left) << "[label=\"l\"];\n";
            PrintRelations(node->left, out);
        }
        else
        {
            out << ' ' << safeKey(node) << "->" << safeKey(node->left) << "[label=\"l\"];\n";
        }
        if (node->right->bit > node->bit)
        {
            out << ' ' << safeKey(node) << "->" << safeKey(node->right) << "[label=\"r\"];\n";
            PrintRelations(node->right, out);
        }
        else
        {
            out << ' ' << safeKey(node) << "->" << safeKey(node->right) << "[label=\"r\"];\n";
        }
    }
};

int main()
{
    // ???????????
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    // ?????? i/o -- ????? ??? ????????????/??????????????, ????????/?????????? ??????????? ????????
    std::ofstream fout;
    std::ofstream dotout;
    std::ifstream fin;


    for (int counter = 34; counter <= 34; ++counter) {
        char input[MAXLEN];
        TValue value;

        // ???????? ?????? trie
        TTrie *trie;
        try
        {
            trie = new TTrie();
        }
        catch (const std::bad_alloc &e)
        {
            std::cout << "ERROR: fail to allocate the requested storage space\n";
            exit(0);
        }

        TNode *node;

        std::string fileName = "tests/" + std::to_string(counter);
        std::ifstream testFile(fileName);
        //std::cout << "test " << counter << " ------------------------------------------------------------\n";
        
        while ((/* std::cin */testFile >> input))
        {
            if (!std::strcmp(input, "+"))
            {
                /* std::cin */testFile >> input;
                Lowercase(input);
                /* std::cin */testFile >> value;

                std::cout << (trie->Insert(input, value) ? "OK" : "Exist");
                std::cout << '\n';
            }
            else if (!std::strcmp(input, "-"))
            {
                /* std::cin */testFile >> input;
                Lowercase(input);

                std::cout << (trie->Delete(input) ? "OK" : "NoSuchWord");
                std::cout << '\n';
            }
            else if (!std::strcmp(input, "!"))
            {
                /* std::cin */testFile >> input;
                if (!std::strcmp(input, "Save"))
                {
                    /* std::cin */testFile >> input;
                    fout.open(input, std::ios::out | std::ios::binary | std::ios::trunc);
                    if (!fout.is_open()) {
                        std::cout << "ERROR: can't create file\n";
                        continue;
                    }

                    trie->Save(fout);
                    std::cout << "OK\n";

                    fout.close();
                }
                else if (!std::strcmp(input, "Load"))
                {
                    /* std::cin */testFile >> input;
                    fin.open(input, std::ios::in | std::ios::binary);
                    if (!fin.is_open()) {
                        std::cout << "ERROR: can't open file\n";
                        continue;
                    }

                    delete trie;
                    trie = new TTrie();
                    trie->Load(fin);

                    std::cout << "OK\n";

                    fin.close();
                }
            }
            else if (!std::strcmp(input, "1p"))
            {
                std::ofstream dot;
                dot.open("source.dot", std::ios::out | std::ios::trunc);

                dot << "digraph {\n";
                trie->PrintDefinitions(trie->root, dot);

                trie->PrintRelations(trie->root, dot);

                dot << "}\n";

                dot.flush(), dot.close();

                if (system("dot source.dot -Tpng -o res.png") == -1) {
                    std::cout << "ERROR: fail compiling source.dot file into png\n";
                    continue;
                } 
                
                if (system("xdg-open res.png") == -1) {
                    std::cout << "ERROR: fail trying to open res.png\n";
                    continue;
                } 
            }
            else
            {
                Lowercase(input);
                node = trie->Find(input);
                if (!node)
                    std::cout << "NoSuchWord";
                else
                    std::cout << "OK: " << node->value;
                std::cout << '\n';
            }
        }

        delete trie;
        //std::cout << "\n\n\n\n\n\n\n";
    }
    return 0;
}