#include "patricia.hpp"
#include "base.hpp"


int main(int argc, char const *argv[])
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    //for (int counter = 34; counter <= 34; ++counter) {
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
                    patricia.Traverse();

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
            //patricia.Traverse();
        }
        //std::cout << "\n\n\n\n\n\n\n";
    //}
    return 0;
}