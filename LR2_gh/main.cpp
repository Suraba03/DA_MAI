#include "patricia.hpp"
#include "base.hpp"

int main(int argc, char const *argv[])
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    //std::ofstream fout;
    //std::ifstream fin;

    //for (int counter = 1; counter <= 41; ++counter) {

        PATRICIA *patricia;
        patricia = new PATRICIA();
        // interface
        std::string mark;
        std::string wordChars;
        uf64 value;       // value
        //std::string fileName = "../tests/" + std::to_string(counter);
        //std::ifstream testFile(fileName);
        //std::cout << "test " << counter << " ------------------------------------------------------------\n";
        
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
                    //fout.open(wordChars, std::ios::out | std::ios::binary | std::ios::trunc);
                    std:: ofstream fout(wordChars, std::ios::binary);
                    patricia->Save(fout, patricia->FillNodesVector());
                    std::cout << "OK\n";
                    fout.close();

                } else if (wordChars == "Load") {
                    std::cin/* testFile */ >> wordChars;
                    std:: ifstream fin(wordChars, std::ios::binary);
                    if (is_empty(fin)) {
                        patricia->Clear();
                        patricia = new PATRICIA();
                        std::cout << "OK\n";
                        continue;
                    }
                    patricia->Clear();
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
        //std::cout << "\n\n\n\n\n\n\n";
    //}
    return 0;
}