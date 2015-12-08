#include <iostream>
#include "solver.hpp"
using namespace sj;

/****************************************************************************/
sj::Tile::Tile(std::string character, int xk, int yk) : xkoord(xk), ykoord(yk), ch(character) {
    wch = sj::utf8_to_wstring(character);
}
sj::Tile::Tile(std::wstring character, int xk, int yk) : xkoord(xk), ykoord(yk), wch(character) {
    ch = sj::utf8_to_string(character);
}
int Tile::x() const{
    return xkoord;
}
int Tile::y() const{
    return ykoord;
}
std::string Tile::c() const{
    return ch;
}
std::wstring Tile::wchar() const{
    return wch;
}
bool Tile::operator==(const Tile& t) const{
    return ( t.x() == xkoord && t.y() == ykoord && t.c() == ch);
}

/****************************************************************************/
sj::Path::Path(Tile t){
    tiles.push_back(t);
    whole_word = t.c();
    wide_word = t.wchar();
}
std::vector<Tile> Path::path(){
    return tiles;
}
std::string Path::word() const{
    return whole_word;
}
std::wstring Path::w_word() const{
    return wide_word;
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
    wide_word += t.wchar();
}
Tile Path::last(){
    return tiles[tiles.size()-1];
}
bool Path::in_path(const Tile t) const{
    return(std::find(tiles.begin(),tiles.end(), t) != tiles.end());
}
bool Path::operator<(const Path t) const{
    if (wide_word.length() == t.w_word().length())
        return wide_word < t.w_word();
    else
        return wide_word.length() > t.w_word().length();
}
bool Path::operator==(const Path t) const{
    return wide_word == t.w_word();
}
/*******************************************************************************/
sj::Solver::Solver(std::set<std::string> dictionary, std::string matrix, int x_size, int y_size) {
    std::vector<std::vector<Tile>> tile_matrix = sj::Solver::create_matrix(x_size,y_size, sj::utf8_to_wstring(matrix));
    for (std::string word : dictionary){
        dict.insert(sj::utf8_to_wstring(word));
    }
    word_paths = sj::Solver::find_words(tile_matrix, dict);
}
sj::Solver::Solver(std::set<std::wstring> dictionary, std::wstring matrix, int x_size, int y_size) {
    std::vector<std::vector<Tile>> tile_matrix = sj::Solver::create_matrix(x_size,y_size, matrix);
    dict = dictionary;
    word_paths = sj::Solver::find_words(tile_matrix, dict);
}
sj::Solver::Solver(std::set<std::string> dictionary, std::wstring matrix, int x_size, int y_size) {
    std::vector<std::vector<Tile>> tile_matrix = sj::Solver::create_matrix(x_size,y_size, matrix);
    for (std::string word : dictionary){
        dict.insert(sj::utf8_to_wstring(word));
    }
    word_paths = sj::Solver::find_words(tile_matrix, dict);
}
sj::Solver::Solver(std::set<std::wstring> dictionary, std::string matrix, int x_size, int y_size) {
    std::vector<std::vector<Tile>> tile_matrix = sj::Solver::create_matrix(x_size,y_size, sj::utf8_to_wstring(matrix));
    dict = dictionary;
    word_paths = sj::Solver::find_words(tile_matrix, dict);
}
std::vector<Path> sj::Solver::Paths(){
    return word_paths;
}

void sj::Solver::Android_Solve(int x_size, int y_size, int event_num){
    int x_base_offset = x_size / 5.538;
    int y_base_offset = y_size / 2.184;
    int offset = x_size / 4.675;
    std::stringstream ss;
    std::stringstream temp;
    int i = 0;
    for (auto word : word_paths){
        ss << "adb shell sendevent /dev/input/event" << event_num << " 3 57 " << i; //ABS_MT_TRACKING_ID
        system(ss.str().c_str());
        ss.str("");
        ss << "adb shell sendevent /dev/input/event" << event_num << " 3 48 3";
        system(ss.str().c_str()); // ABS_MT_TOUCH_MAJOR
        ss.str("");
        
        for (auto path : word.path()){
        	if (!android_solver_running)
        		return;
            ss << "adb shell sendevent /dev/input/event" << event_num << " 3 53 " << path.x() * offset + x_base_offset; //ABS_MT_POSITION_X
            system(ss.str().c_str());
            ss.str("");
            ss << "adb shell sendevent /dev/input/event" << event_num << " 3 54 " << path.y() * offset + y_base_offset; //ABS_MT_POSITION_Y
            system(ss.str().c_str());
            ss.str("");
            if (i > 0){
                if( i % 2 == 0){
                    temp << "adb shell sendevent /dev/input/event" << event_num << " 3 003e 2";
                    system(temp.str().c_str()); //Dunno, somekind of swipe
                }
                else {
                    temp << "adb shell sendevent /dev/input/event" << event_num << " 3 003e 0";
                    system(temp.str().c_str()); //Dunno, somekind of swipe
                }
            }
            temp.str("");
            temp << "adb shell sendevent /dev/input/event" << event_num << " 0 0 0";
            system(temp.str().c_str()); //SYN_REPORT
            temp.str("");
            i++;
        }
        i = 0;
        temp << "adb shell sendevent /dev/input/event" << event_num << " 3 57 4294967295";
        system(temp.str().c_str()); // ABS_MT_TRACKING_ID
        temp.str("");
        temp << "adb shell sendevent /dev/input/event" << event_num << " 0 0 0";
        system(temp.str().c_str()); //SYN_REPORT
        ss.str("");
    }
    this->setAndroidSolverState(false);
}

std::vector<Path> sj::Solver::find_words(std::vector<std::vector<Tile>>& matrix, std::set<std::wstring> dictionary) {
    std::map<std::wstring, std::set<std::wstring>> dictionaries;
    std::wstring letter;
    for (std::wstring word : dictionary){
        letter = word[0];
        dictionaries[letter].insert(word);
    }
    std::vector<Path> found_words;
    /*Use each tile in matrix as starting point for path*/
    for (unsigned int i=0; i<matrix.size(); i++){
        for (unsigned int j=0; j<matrix[0].size(); j++){
            Tile k = matrix[i][j];
            Path path(k);
            std::vector<Tile> nearby_tiles = find_nearby(matrix, path);
            for (Tile next_tile : nearby_tiles){
                move_to_tile(dictionaries[k.wchar()], matrix, path, next_tile, found_words);
            }
        }
    }
    std::sort(found_words.begin(), found_words.end());
    return found_words;
}

void sj::Solver::move_to_tile(std::set<std::wstring> dictionary,std::vector<std::vector<Tile>>& matrix, Path p, Tile next_tile, std::vector<Path>& found_words){
    p.append(next_tile);
    /*Word is found from dictionary, length more than 2, word does not exist in found words*/
    auto word = dictionary.find(p.w_word());
    if (word != dictionary.end() && p.w_word().length() > 2 && std::find(found_words.begin(), found_words.end(), p) == found_words.end()){
        found_words.push_back(p);
    }
    /*Find words from dictionary that begin with the same characters that the path contains*/
    std::set<std::wstring> remaining_words;
    auto first_word = dictionary.lower_bound(p.w_word());
    for (auto i = first_word; i != dictionary.end(); i++){
        if (i->substr(0, p.w_word().length()) == p.w_word()){
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

std::vector<Tile> sj::Solver::find_nearby(std::vector<std::vector<Tile>>& matrix, Path path){
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

std::vector<std::vector<Tile>> sj::Solver::create_matrix(int x_size, int y_size, std::wstring matrix_string){
    std::vector<std::vector<Tile>> matrix;
    std::wstringstream wss;
    std::vector<Tile> column;
    std::string tmp;
    /*Add each column to matrix*/
    for (int i = 0; i<x_size; i++){
        for (int j = 0; j < y_size; j++){ 
            wss << matrix_string[i + j*x_size];
            std::wstring wtemp = wss.str();
            column.push_back(Tile(wtemp,i,j));
            wss.str(L"");
        }
        matrix.push_back(column);
        column.clear();
    }  
    return matrix;
}
std::wstring sj::utf8_to_wstring(const std::string& str){
    return boost::locale::conv::utf_to_utf<wchar_t>(str.c_str(), str.c_str() + str.size());
}
std::string sj::utf8_to_string(const std::wstring& str){
    return boost::locale::conv::utf_to_utf<char>(str.c_str(), str.c_str() + str.size());
}
