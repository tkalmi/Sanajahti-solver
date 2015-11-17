#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include "GUI.hpp"
#include "solver.hpp"

#include <signal.h>

static void handler(int signal)
{

switch (signal) {
	case SIGINT: 
	fprintf(stderr, "Caught interrupt signal \n");
	break;
}
exit(signal);
}

//adb shell screencap -p | perl -pe 's/\x0D\x0A/\x0A/g' > screen.png
int main(int argc, char **argv)
{
    signal(SIGINT, handler);
    setlocale(LC_ALL, "");


    bool text_ui = false; // Default is to use with GUI
    bool android_input = false; // Also no inputing to Android
    bool with_paths = false; // We do not print paths if we use GUI
    int options;
    int M = 4, N = 4; //Matrix dimensions, default is 4x4 as defined by Sanajahti.
    std::string filename = "sanat.txt"; //default wordlist. Changeable by command flags.
    std::wstring matrix_as_string;
    while ((options = getopt(argc, argv, "ac:pw:m:n:")) != -1) {
        switch(options) {
	    case 'a':
		android_input = true;
		break;
            case 'c':
		matrix_as_string = sj::utf8_to_wstring(optarg);
		text_ui = true;
                break;
     	    case 'p':
		with_paths = true;
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
        words.insert(sj::utf8_to_wstring(line));
    }
    file.close();
    if (text_ui == true) {
       /// fprintf(stderr, "TODO, not yet implemented");
	std::wcout << L"Printing words with matrix: " << matrix_as_string << std::endl;;
	sj::Solver solver(words, matrix_as_string, M, N);
        std::vector<sj::Path> solved_words = solver.Paths();
	for (auto i : solved_words) { 
	    if(with_paths == true)
            	std::wcout << sj::utf8_to_wstring(i.path_str()) << std::endl;
	    else
		std::wcout << i.w_word() << std::endl;
        }
	std::wcout << "Total words: " << solved_words.size() << std::endl;
    }        
    else {
        GUI gui(960, 640, words);
        gui.run();
        return 0;
    }
    if (android_input == true) {
	sj::Solver solver(words, matrix_as_string, M, N);
	for (auto i : solver.Paths()) {
		std::wcout << sj::utf8_to_wstring(i.path_str()) << std::endl;
	}
    solver.Android_Solve(720, 1280); // Speksattu toistaiseksi vilin S3:selle, lisätään resoluutioflagit
    }
    return 0;
}
