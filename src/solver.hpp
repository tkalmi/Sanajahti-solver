#ifndef AALTO_ELEC_CPP_RECTANGLE
#define AALTO_ELEC_CPP_RECTANGLE
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <sstream>
#include <string>
#include <locale>
#include <boost/locale/encoding_utf.hpp>
#include <string>

namespace sj{
    
class Tile {
public:
    /* Description:
     * ------------
     * Tile constructor
     * 
     * Parameters:
     * ------------
     * character : character in tile e.g "a"
     * xk : x-coordinate of the tile
     * yk : y-coordinate of the tile
     */
    Tile(std::string character, int xk, int yk);
    Tile(std::wstring character, int xk, int yk);
    /* Description:
     * ----------
     * Returns x-coordinate of the tile.
     */
    int x() const;
    /* Description:
     * ----------
     * Returns x-coordinate of the tile.
     */
    int y() const;
    /* Description:
     * ----------
     * Returns character of the tile.
     */
    std::string c() const;
    std::wstring wchar() const;
    bool operator==(const Tile& t) const;
    
private:
    int xkoord;
    int ykoord;
    std::string ch;
    std::wstring wch;
};

class Path {
public:
    /* Description:
     * ------------
     * Path constructor
     * 
     * Parameters:
     * ------------
     * t : Path start tile.
     */
    Path(Tile t);
    /* Description:
     * ------------
     * Returns path tiles as a vector.
     */
    std::vector<Tile> path();
    /* Description:
     * ------------
     * Returns the word of the path.
     */
    std::string word() const;
    std::wstring w_word() const;
    /* Description:
     * ------------
     * Return path word and coordinates in following format:
     * word (x,x) (y, y) (z, z) ...
     */
    std::string path_str() const;
    /* Description:
     * ------------
     * Append tile to path.
     */
    void append(Tile t);
    /* Description:
     * ------------
     * Returns last tile in the path.
     */
    Tile last();
    /* Description:
     * ------------
     * Returns true if tile is in the path, false otherwise.
     * 
     * Parameters:
     * -----------
     * t : Tile to check.
     */
    bool in_path(Tile t) const;
    /* Description:
     * ------------
     * Compares path lengths only.
     */
    bool operator<(const Path t) const;
    bool operator==(const Path t) const;
    
private:
    std::vector<Tile> tiles;
    std::string whole_word;
    std::wstring wide_word;
};

 /*
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
std::vector<Path> find_words(std::vector<std::vector<Tile>>& matrix, std::set<std::string> dictionary);
std::vector<Path> find_words(std::vector<std::vector<Tile>>& matrix, std::set<std::wstring> dictionary);

/*
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
std::vector<Tile> find_nearby(std::vector<std::vector<Tile>>& matrix, Path path);

/*
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
void move_to_tile(std::set<std::string> dictionary,std::vector<std::vector<Tile>>& matrix, Path p, Tile next_tile, std::vector<Path>& found_words);
void move_to_tile(std::set<std::wstring> dictionary,std::vector<std::vector<Tile>>& matrix, Path p, Tile next_tile, std::vector<Path>& found_words);

/*
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

std::vector<std::vector<Tile>> create_matrix(int x_size, int y_size, std::string matrix_string);
std::vector<std::vector<Tile>> create_matrix(int x_size, int y_size, std::wstring matrix_string);
/*
 * Description:
 * ------------
 * Returns first character of a string as a string
 * 
 * Parameters:
 * -----------
 * str : string
 */
std::string first_character(std::string str);
std::wstring utf8_to_wstring(const std::string& str);
std::string utf8_to_string(const std::wstring& str);
int str_len(std::string str);
}



#endif
