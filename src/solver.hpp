#ifndef AALTO_ELEC_CPP_RECTANGLE
#define AALTO_ELEC_CPP_RECTANGLE
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <sstream>

void rectangle();

class Tile {
public:
    Tile(std::string kirjain, int xk, int yk);
    int x() const;
    int y() const;
    std::string c() const;
    bool operator==(const Tile& t) const;
    
private:
    int xkoord;
    int ykoord;
    std::string kirj;
};

class Path {
public:
    Path(Tile t);
    std::vector<Tile> path();
    std::string word() const;
    std::string path_str() const;
    void append(Tile t);
    Tile last();
    bool in_path(Tile t) const;
    bool operator<(const Path t) const;
    
private:
    std::vector<Tile> tiles;
    std::string whole_word;
};

std::vector<Path> find_words(std::vector<std::vector<Tile>>& matriisi, std::set<std::string> sanat);
std::vector<Tile> find_nearby(std::vector<std::vector<Tile>>& matriisi, Path path);
void find_words(std::set<std::string> words,std::vector<std::vector<Tile>>& matriisi, Path p, Tile next_tile, std::vector<Path>& found_words);
std::vector<std::vector<Tile>> create_matrix(int x_size, int y_size, std::string words);

#endif
