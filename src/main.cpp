#include <iostream>
#include <fstream>
#include <vector>
#include "GUI.hpp"
#include "solver.hpp"

int main(void)
{
    setlocale(LC_ALL, "");
    // get word list from file
    std::ifstream file;
    std::string line;
    std::set<std::wstring> words;
    file.open("sanat.txt");
    while (std::getline(file, line)){
        words.insert(sj::utf8_to_wstring(line));
    }
    file.close();
    GUI gui(960, 640, words);
    gui.run();
    //sj::Solver k(words, L"käkihomohomokäki",4,4);
    //for (auto i : k.Paths()){
    //    std::wcout << i.w_word() << std::endl;
    //}
    return 0;
}
