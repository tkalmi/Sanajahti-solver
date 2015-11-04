#include <iostream>
#include "solver.hpp"

/****************************************************************************/
Tile::Tile(std::string kirjain, int xk, int yk) : xkoord(xk), ykoord(yk), kirj(kirjain) {}
int Tile::x() const{
    return xkoord;
}
int Tile::y() const{
    return ykoord;
}
std::string Tile::c() const{
    return kirj;
}
bool Tile::operator==(const Tile& t) const{
    return ( t.x() == xkoord && t.y() == ykoord && t.c() == kirj);
}

/****************************************************************************/
Path::Path(Tile t){
    tiles.push_back(t);
    whole_word = t.c();
}
std::vector<Tile> Path::path(){
    return tiles;
}
std::string Path::word() const{
    return whole_word;
}
std::string Path::path_str() const{
    std::stringstream ss;
    ss << whole_word;
    for (Tile i : tiles){
        ss << " (" << i.x() << "," << i.y() << ")";
    }
    return ss.str();   
}
void Path::append(Tile t){
    tiles.push_back(t);
    whole_word += t.c();
}
Tile Path::last(){
    return tiles[tiles.size()-1];
}
bool Path::in_path(const Tile t) const{
    return(std::find(tiles.begin(),tiles.end(), t) != tiles.end());
}
bool Path::operator<(const Path t) const{
    return whole_word.length() < t.word().length();
}
/****************************************************************************/
std::vector<Path> find_words(std::vector<std::vector<Tile>>& matriisi, std::set<std::string> sanat) {
    std::map<std::string, std::set<std::string>> sanalistat;
    std::stringstream ss;
    for (std::string sana : sanat){
        ss << sana[0];
        sanalistat[ss.str()].insert(sana);
        ss.str("");
    }
    std::vector<Path> found_words;
    for (unsigned int i=0; i<matriisi.size(); i++){
        for (unsigned int j=0; j<matriisi[0].size(); j++){
            Tile k = matriisi[i][j];
            Path path(k);
            std::vector<Tile> t = find_nearby(matriisi, path);
            for (auto i : t){
                find_words(sanalistat[k.c()], matriisi, path, i, found_words);
            }
        }
    }
    std::sort(found_words.begin(), found_words.end());
    return found_words;
}
/****************************************************************************/
void find_words(std::set<std::string> words,std::vector<std::vector<Tile>>& matriisi, Path p, Tile next_tile, std::vector<Path>& found_words){
    p.append(next_tile);
    auto word = words.find(p.word());
    if (word != words.end()){
        found_words.push_back(p);
    }
    std::set<std::string> remaining_words;
    auto first_word = words.lower_bound(p.word());
    for (auto i = first_word; i != words.end(); i++){
        if (boost::starts_with(*i, p.word())){
            remaining_words.insert(*i);
        }
    }
    if (remaining_words.size() > 0){
        std::vector<Tile> t = find_nearby(matriisi, p);
        for (auto i : t){
            find_words(remaining_words, matriisi, p, i, found_words);
        }
    }
    
}
/****************************************************************************/
std::vector<Tile> find_nearby(std::vector<std::vector<Tile>>& matriisi, Path path){
    std::vector<Tile> t;
    int x = path.last().x();
    int y = path.last().y();
    int xmax = matriisi.size();
    int ymax = matriisi[0].size();
    //Left Up
    if(x-1 >= 0 && y-1 >= 0 && not(path.in_path(matriisi[x-1][y-1]))){
        t.push_back(matriisi[x-1][y-1]);
    }
    //Up
    if(y-1 >= 0 && not(path.in_path(matriisi[x][y-1]))){
        t.push_back(matriisi[x][y-1]);
    }
    //Right Up
    if(x+1 < xmax && y-1 >= 0 && not(path.in_path(matriisi[x+1][y-1]))){
        t.push_back(matriisi[x+1][y-1]);
    }
    //Left
    if(x-1 >= 0 && not(path.in_path(matriisi[x-1][y]))){
        t.push_back(matriisi[x-1][y]);
    }
    //Right
    if(x+1 < xmax && not(path.in_path(matriisi[x+1][y]))){
        t.push_back(matriisi[x+1][y]);
    }
    //Left Down
    if(x-1 >= 0 && y+1 < ymax && not(path.in_path(matriisi[x-1][y+1]))){
        t.push_back(matriisi[x-1][y+1]);
    }
    //Down
    if(y+1 < ymax && not(path.in_path(matriisi[x][y+1]))){
        t.push_back(matriisi[x][y+1]);
    }
    //Right Down
    if(x+1 < xmax && y+1 < ymax && not(path.in_path(matriisi[x+1][y+1]))){
        t.push_back(matriisi[x+1][y+1]);
    }
    
    return t;
}
std::vector<std::vector<Tile>> create_matrix(int x_size, int y_size, std::string words){   
    std::stringstream ss;
    std::vector<std::vector<Tile>> matrix;
    std::vector<Tile> line;
    for (int i = 0; i<x_size; i++){
        for (int j = 0; j < y_size; j++){
            ss << words[x_size*j + i];
            std::string letter = ss.str();
            line.push_back(Tile(letter,i,j));
            ss.str("");
        }
        matrix.push_back(line);
        line.clear();
    }
    return matrix;
}