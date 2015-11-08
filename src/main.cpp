#include <iostream>
#include <fstream>
#include <vector>
#include "solver.hpp"

int main(void) {
    std::ifstream file;
    std::string line;
    std::set<std::string> words;
    file.open("sanat.txt");
    while(std::getline(file,line)){
        words.insert(line);
    }
    file.close();
    std::vector<std::vector<sj::Tile>> matrix = sj::create_matrix(4,4,"tarrottajiiplipa");
    auto t = sj::find_words(matrix, words);
    for (auto i : t){
        std::cout << i.path_str() << std::endl;
    }
    return 0;
}
