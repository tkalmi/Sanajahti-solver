#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include "GUI.hpp"


int main(int argc, char **argv)
{
    setlocale(LC_ALL, "");


    bool text_ui = false;
    int options;
    int M = 4, N = 4; //Matrix dimensions, default is 4x4 as defined by Sanajahti.
    std::wstring matrix_as_string = L"UEMNTTIKEAÄTHÄPYA";
    std::string filename = "sanat.txt"; //default wordlist. Changeable by command flags.
    while ((options = getopt(argc, argv, "c:w:m:n:")) != -1) {
        switch(options) {
            case 'c':
                text_ui = true;
                break;
            case 'w':
                filename = optarg;
                std::wcout << L"filename: " << sj::utf8_to_wstring(filename) << std::endl; // Jotain hämärää, hävittää ääkköset. Testaa ./main -c 1 ja sen jälkeen ./main -c 1 -w sanat.txt
                break;
            case 'm':
                M = atoi(optarg); // Täytyy kattoa toimiiko char * kanssa, vai vaatiiko \0:n
                break;
            case 'n':
                N = atoi(optarg);
                break;
            case '?':
                if (optopt == 'c')
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint(optopt))
                    fprintf(stderr, "Unknown option -%c \n", optopt);
                return 1;
            default:
                abort();
                text_ui = true; // TODO
        }
    }
    
    // get word list from file
    std::ifstream file;
    std::string line;
    std::set<std::wstring> words;
    file.open(filename);
    while (std::getline(file, line)){
        words.insert(boost::locale::conv::utf_to_utf<wchar_t>(line.c_str(),line.c_str() + line.size()));
    }
    file.close();
    if (text_ui == true) {
       /// fprintf(stderr, "TODO, not yet implemented");
        (void) M;
        (void) N;// TODO
	std::wcout << L"Printing words with matrix: " << std::endl << matrix_as_string << std::endl;;
        auto char_matrix = sj::create_matrix(M, N, matrix_as_string);
        std::vector<sj::Path> solved_words = sj::find_words(char_matrix, words);
        for (auto i : solved_words) {
            std::wcout << i.w_word() << std::endl;
        }
    }        
    else {
        GUI gui(960, 640, words);
        gui.run();
        return 0;
    }
    //auto t = sj::create_matrix(4,5,L"homohomokäkihomohomo");
    //auto k = sj::find_words(t,words);
    //for (auto i : k){
    //    std::wcout << i.w_word() << std::endl;
    //}
//
//    GUI gui(960, 640, words);
 //   gui.run();
    return 0;
}
