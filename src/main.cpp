#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include "GUI.hpp"
#include "solver.hpp"
#include "ocr.hpp"

#include <signal.h>

static void handler(int signal)
//adb shell screencap -p | perl -pe 's/\x0D\x0A/\x0A/g' > screen.png
{
switch (signal) {
	case SIGINT: 
	fprintf(stderr, "Caught interrupt signal \n");
	break;
}
exit(signal);
}

int res_x = 1080;
int res_y = 1920;

//adb shell screencap -p /tmp/scrot.png && adb pull /tmp/scrot.png");
int main(int argc, char **argv)
{
    signal(SIGINT, handler);
    setlocale(LC_ALL, "");
//Settings
    bool text_ui = false; // Default is to use with GUI
    bool ocr_on = false;
    bool android_input = false; // Also no inputing to Android
    bool with_paths = false; // We do not print paths if we use GUI
   
    std::string str; //Luvut // Apumuuttujia optioon -m
    bool loytyy_x = false;
    
    int options;
    int M = 4, N = 4; //Matrix dimensions, default is 4x4 as defined by Sanajahti.
    std::string filename = "sanat.txt"; //default wordlist. Changeable by command flags.
    std::wstring matrix_as_string;
    while ((options = getopt(argc, argv, "ac:pw:m:o::")) != -1) {
        switch(options) {
	    case 'a': // Android support
		android_input = true;
		break;
            case 'c': // Disabling GUI, printing to shell.
		if (ocr_on == false)
			matrix_as_string = sj::utf8_to_wstring(optarg);
		text_ui = true;
                break;
     	    case 'p': // Printing words paths as well
		with_paths = true;
		break;
            case 'w': // Defining other wordlist to be used.
                filename = optarg;
                std::wcout << L"filename: " << sj::utf8_to_wstring(filename) << std::endl; // Jotain hämärää, hävittää ääkköset. Testaa ./main -c 1 ja sen jälkeen ./main -c 1 -w sanat.txt
                break;
            case 'm':
                if (optarg[0] == '0' || optarg[0] == 'x') {
                    fprintf(stderr,"Option -m input format is: MxN, for example 4x4\n");
                    return 1;
                }
                for (unsigned int i = 0; i < strlen(optarg); i++) {
                    if (optarg[i] == 'x') {
                        if (loytyy_x) {// For double 'x's, for example 4x4x4 not allowed
                            fprintf(stderr, "Only 2d matrixes are supported, sorry.\n");
                            return 1;
                        }
                        loytyy_x = true;
                        M = std::stoi (str);
                        str.erase();
                        continue;
                    }
                    str.push_back(optarg[i]);
                }
                N = std::stoi (str);
                if (!loytyy_x) {
                    fprintf(stderr, "Option -m input format is: MxN, for example 4x4\n");
                    return 1;
                }
                fprintf(stderr,"M: %d, N: %d\n",M,N);
                break;
            case 'o':
 		setlocale(LC_NUMERIC, "C");
		ocr_on = true;
		system("adb shell screencap -p /sdcard/scrot.png && adb pull /sdcard/scrot.png");
                matrix_as_string = sj::utf8_to_wstring(ocr());
		for (unsigned int i = 0; i < 16; i++) {
		std::wcout << matrix_as_string[i];
		if ((i+1) % 4 == 0)
			std::wcout << std::endl;
		}
//		std::wcout << matrix_as_string[0] << " Length: " << matrix_as_string.size() << std::endl;
		setlocale(LC_ALL, "");
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
        solver.Android_Solve(res_x, res_y); // Speksattu toistaiseksi vilin S3:selle, lisätään resoluutioflagit
    }
    return 0;
}
