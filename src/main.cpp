#include <iostream>
#include <fstream>
#include <vector>
#include "GUI.hpp"

int main(void)
{
    setlocale(LC_ALL, "");
    // get word list from file
    std::ifstream file;
    std::string line;
    std::set<std::wstring> words;
    file.open("sanat.txt");
    while (std::getline(file, line)){
        words.insert(boost::locale::conv::utf_to_utf<wchar_t>(line.c_str(),line.c_str() + line.size()));
    }
    file.close();
    //auto t = sj::create_matrix(4,5,L"homohomokäkihomohomo");
    //auto k = sj::find_words(t,words);
    //for (auto i : k){
    //    std::wcout << i.w_word() << std::endl;
    //}
    //GUI gui(960, 640, words);
    //gui.run();
    return 0;
}
