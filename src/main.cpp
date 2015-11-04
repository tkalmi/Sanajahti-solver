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
    std::vector<std::vector<Tile>> matrix = create_matrix(4,4,"tarrottajiiplipa");
    auto t = find_words(matrix, words);
    return 0;
}
