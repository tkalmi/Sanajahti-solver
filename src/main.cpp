#include <iostream>
#include <fstream>
#include <vector>
#include "GUI.hpp"
#include "solver.hpp"

//adb shell screencap -p | perl -pe 's/\x0D\x0A/\x0A/g' > screen.png
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
    //sj::Solver k(words, "lpliaiagtuianioi",4,4);
    //for (auto i : k.Paths()){
    //    std::cout << i.path_str() << std::endl;
    //}
    //k.Android_Solve(720, 1280);
    return 0;
}
