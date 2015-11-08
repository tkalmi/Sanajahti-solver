#include <iostream>
#include "solver.hpp"
using namespace sj;

/****************************************************************************/
sj::Tile::Tile(std::string character, int xk, int yk) : xkoord(xk), ykoord(yk), ch(character) {}
int Tile::x() const{
    return xkoord;
}
int Tile::y() const{
    return ykoord;
}
std::string Tile::c() const{
    return ch;
}
bool Tile::operator==(const Tile& t) const{
    return ( t.x() == xkoord && t.y() == ykoord && t.c() == ch);
}

/****************************************************************************/
sj::Path::Path(Tile t){
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
    if (whole_word.length() == t.word().length())
        return whole_word < t.word();
    else
        return whole_word.length() < t.word().length();
}
bool Path::operator==(const Path t) const{
    return whole_word == t.word();
}
/* find_words
 * Description:
 * ------------
 * Finds the possible words in the matrix
 * 
 * Parameters:
 * -----------
 * matrix : matrix to find words. Use create_matrix
 *          function to create matrix.
 * wordlist : dictionary to check for words
 */
std::vector<Path> sj::find_words(std::vector<std::vector<Tile>>& matrix, std::set<std::string> dictionary) {
    std::map<std::string, std::set<std::string>> dictionaries;
    std::stringstream ss;
    for (std::string word : dictionary){
        ss << word[0];
        dictionaries[ss.str()].insert(word);
        ss.str("");
    }
    std::vector<Path> found_words;
    /*Use each tile in matrix as starting point for path*/
    for (unsigned int i=0; i<matrix.size(); i++){
        for (unsigned int j=0; j<matrix[0].size(); j++){
            Tile k = matrix[i][j];
            Path path(k);
            std::vector<Tile> nearby_tiles = find_nearby(matrix, path);
            for (Tile next_tile : nearby_tiles){
                move_to_tile(dictionaries[k.c()], matrix, path, next_tile, found_words);
            }
        }
    }
    std::sort(found_words.begin(), found_words.end());
    return found_words;
}

/* move_to_tile
 * Description:
 * ------------
 * Moves to next tile and append it to path. If path
 * contains word, it is appended to found_words. 
 * Recursively moves on to nearby tiles if there are 
 * possible words in the dictionary.
 * 
 * Parameters:
 * -----------
 * dictionary : possible words
 * matrix : matrix to find words from.
 * p : path traversed so far.
 * next_tile : next tile to move to
 * found_words : vector of paths that contain a word
 *               found from the matrix
 */
void sj::move_to_tile(std::set<std::string> dictionary,std::vector<std::vector<Tile>>& matrix, Path p, Tile next_tile, std::vector<Path>& found_words){
    p.append(next_tile);
    /*Word is found from dictionary, length more than 2, word does not exist in found words*/
    auto word = dictionary.find(p.word());
    if (word != dictionary.end() && p.word().length() > 2 && std::find(found_words.begin(), found_words.end(), p) == found_words.end()){
        found_words.push_back(p);
    }
    /*Find words from dictionary that begin with the same characters that the path contains*/
    std::set<std::string> remaining_words;
    auto first_word = dictionary.lower_bound(p.word());
    for (auto i = first_word; i != dictionary.end(); i++){
        if (i->substr(0, p.word().length()) == p.word()){
            remaining_words.insert(*i);
        }
    }
    /*If there are possible words in the dictionary, all nearby tiles are checked*/
    if (remaining_words.size() > 0){
        std::vector<Tile> nearby_tiles = find_nearby(matrix, p);
        for (Tile next_tile : nearby_tiles){
            move_to_tile(remaining_words, matrix, p, next_tile, found_words);
        }
    }
    
}
/* find_nearby
 * Description:
 * ------------
 * Finds nearby tiles that are not within the path.
 * 
 * Parameters:
 * -----------
 * matrix : matrix to find words.
 * path : path traversed so far.
 * 
 * Returns:
 * ----------
 * Vector of nearby tiles.
 */
std::vector<Tile> sj::find_nearby(std::vector<std::vector<Tile>>& matrix, Path path){
    std::vector<Tile> t;
    int x = path.last().x();
    int y = path.last().y();
    int xmax = matrix.size();
    int ymax = matrix[0].size();
    //Left Up
    if(x-1 >= 0 && y-1 >= 0 && !(path.in_path(matrix[x-1][y-1]))){
        t.push_back(matrix[x-1][y-1]);
    }
    //Up
    if(y-1 >= 0 && !(path.in_path(matrix[x][y-1]))){
        t.push_back(matrix[x][y-1]);
    }
    //Right Up
    if(x+1 < xmax && y-1 >= 0 && !(path.in_path(matrix[x+1][y-1]))){
        t.push_back(matrix[x+1][y-1]);
    }
    //Left
    if(x-1 >= 0 && !(path.in_path(matrix[x-1][y]))){
        t.push_back(matrix[x-1][y]);
    }
    //Right
    if(x+1 < xmax && !(path.in_path(matrix[x+1][y]))){
        t.push_back(matrix[x+1][y]);
    }
    //Left Down
    if(x-1 >= 0 && y+1 < ymax && !(path.in_path(matrix[x-1][y+1]))){
        t.push_back(matrix[x-1][y+1]);
    }
    //Down
    if(y+1 < ymax && !(path.in_path(matrix[x][y+1]))){
        t.push_back(matrix[x][y+1]);
    }
    //Right Down
    if(x+1 < xmax && y+1 < ymax && !(path.in_path(matrix[x+1][y+1]))){
        t.push_back(matrix[x+1][y+1]);
    }
    
    return t;
}

/* create_matrix
 * Description:
 * ------------
 * Create a matrix that can be used for find_words 
 * function. If matrix_string is "aaaabbbbccccdddd"
 * and size is 4x4, the matrix is following:
 * aaaa
 * bbbb
 * cccc
 * dddd
 * 
 * Parameters:
 * -----------
 * x_size : matrix size in x-axis
 * y_size : matrix size in y-axis
 * matrix_string : matrix in format "xxxxxxxxxxxxxxx"
 */
std::vector<std::vector<Tile>> sj::create_matrix(int x_size, int y_size, std::string matrix_string){   
    std::stringstream ss;
    std::vector<std::vector<Tile>> matrix;
    std::vector<Tile> line;
    for (int i = 0; i<x_size; i++){
        for (int j = 0; j < y_size; j++){
            ss << matrix_string[x_size*j + i];
            std::string letter = ss.str();
            line.push_back(Tile(letter,i,j));
            ss.str("");
        }
        matrix.push_back(line);
        line.clear();
    }
    return matrix;
}
