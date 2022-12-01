#include "patricia.hpp"

int main(int argc, char const *argv[])
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    //std::ofstream fout;
    //std::ifstream fin;

    //for (int counter = 3; counter <= 3; ++counter) {

        PATRICIA *patricia;
        patricia = new PATRICIA();
        // interface
        std::string mark;
        std::string wordChars;
        uf64 value;       // value
        std::ifstream testFile("tests/Delete0__inputPow10_6.txt");
        std::ifstream fin("dict_pow6.bin", std::ios::binary);
        patricia->Load(fin);
        fin.close();
        while (/* std::cin */testFile >> mark) {
            if (mark == "+") {
                /* std::cin */testFile >> wordChars >> value;
                node_t *node = patricia->AddNode(wordChars, value);
            } else if (mark == "-") {
                /* std::cin */testFile >> wordChars;
                patricia->DeleteNode(wordChars);
            } else if (mark == "!") {
                /* std::cin */testFile >> wordChars;
                if (wordChars == "Save") {
                    /* std::cin */testFile >> wordChars;
                    std::ofstream fout(wordChars, std::ios::binary);
                    patricia->Save(fout, patricia->FillNodesVector());
                    fout.close();
                } else if (wordChars == "Load") {
                    /* std::cin */testFile >> wordChars;
                    std::ifstream fin(wordChars, std::ios::binary);
                    if (is_empty(fin)) {
                        patricia->Clear();
                        patricia = new PATRICIA();
                        continue;
                    }
                    patricia->Clear();
                    patricia->Load(fin);
                    fin.close();
                };
            } else { // full comand: "word" -- finds node
                node_t *node = patricia->ReturnNode(mark); // ------------------------------------------
                //if (node != NULL)
                    //std::cout << "OK";
            }
            //patricia->Traverse();
        }

        delete patricia;
        //std::cout << "\n\n\n\n\n\n\n";
    //}
    return 0;
}