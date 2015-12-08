#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include "GUI.hpp"
#include "solver.hpp"
#include "ocr.hpp"
#include <algorithm>
#include <locale.h>
#include <signal.h>
#include <thread>

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

int event_num = 3;

int main(int argc, char **argv)
{
    signal(SIGINT, handler);
    setlocale(LC_ALL, "");
    setlocale(LC_NUMERIC, "en_US.utf8");

    bool text_ui = false; // Default is to use with GUI
    bool ocr_on = false;
    bool android_input = false; // Also no inputing to Android
    bool with_paths = false; // We do not print paths if we use GUI
   
    std::string str; //Luvut // Apumuuttujia optioon -m
    bool loytyy_x = false;
    
    int options;
    int M = 4, N = 4; //Matrix dimensions, default is 4x4 as defined by Sanajahti.
    std::string filename = "sanat.txt"; //default wordlist. Changeable by command flags.
    std::string ocr_filename = "scrot.png"; // default image file (PNG) to be used with OCR-library.

    std::string matrix_as_string;
    while ((options = getopt(argc, argv, "ac:pw:m:o::l")) != -1) {
        switch(options) {
	    case 'a': // Android support
		android_input = true;
		break;
            case 'c': // Disabling GUI, printing to shell.
		if (ocr_on == false)
                    matrix_as_string = optarg;   	
		text_ui = true;
                break;
     	    case 'p': // Printing words paths as well
		with_paths = true;
		break;
            case 'w': // Defining other wordlist to be used.
                filename = optarg;
                std::cout << "filename: " << filename << std::endl; // Jotain hämärää, hävittää ääkköset. Testaa ./main -c 1 ja sen jälkeen ./main -c 1 -w sanat.txt
                break;
            case 'm': // Matrix input, input as MxN with the 'x' between numbers.
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
 		//setlocale(LC_NUMERIC, "en_US.utf8"); // Needed for OCR to work
		ocr_on = true;
		system("adb shell screencap -p /sdcard/scrot.png && adb pull /sdcard/scrot.png");
		matrix_as_string = ocr(ocr_filename);
                if (sj::utf8_to_wstring(matrix_as_string).size() != 16){
                    std::cout << "Could not detect matrix correctly\n";
                    return 1;
                }
                //setlocale(LC_NUMERIC, ""); // Needed for chars to work
		for (unsigned int i = 0; i < 16; i++) {
                    std::cout << char_to_str(matrix_as_string, i);
                    if ((i+1) % 4 == 0)
                            std::cout << std::endl;
		}
		break;
	   case 'l': // Android testing. Debugging purposes
                //setlocale(LC_NUMERIC, "en_US.utf8"); // Needed for OCR to work
                ocr_on = true;
                //system("adb shell screencap -p /sdcard/scrot.png && adb pull /sdcard/scrot.png");
                matrix_as_string = ocr(ocr_filename);
                if (sj::utf8_to_wstring(matrix_as_string).size() != 16){
                    std::cout << "Could not detect matrix correctly";
                    return 1;
                }
                //setlocale(LC_NUMERIC, ""); // Needed for chars to work
                for (unsigned int i = 0; i < 16; i++) {
                    std::cout << char_to_str(matrix_as_string, i);
                    if ((i+1) % 4 == 0)
                            std::cout << std::endl;
                }
                break;
            case '?':
                if (optopt == 'c')
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint(optopt))
                    fprintf(stderr, "Unknown option -%c \n", optopt);
                return 1;
            default:
                exit(1);
                text_ui = true; // TODO
        }
    }
    if (sj::utf8_to_wstring(matrix_as_string).size() != static_cast<unsigned int>(M*N) && text_ui == true){
        std::cout << "Matrix size does not match dimensions!\n";
        return 1;
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
    if (text_ui == true || ocr_on == true) {
       /// fprintf(stderr, "TODO, not yet implemented");
	std::cout << "Printing words with matrix: " << matrix_as_string << std::endl;;
	sj::Solver solver(words, matrix_as_string, M, N);
        std::vector<sj::Path> solved_words = solver.Paths();
	for (auto i : solved_words) { 
	    if(with_paths == true)
            	std::cout << i.path_str() << std::endl;
	    else
		std::cout << i.word() << std::endl;
        }
	std::cout << "Total words: " << solved_words.size() << std::endl;
    }        
    else {
        GUI gui(960, 640, words);
        gui.run();
        return 0;
    }
    if (android_input == true) {
	sj::Solver solver(words, matrix_as_string, M, N);
	std::pair<int,int> res = get_res(ocr_filename);
	int res_x = res.first;
	int res_y = res.second;
        solver.Android_Solve(res_x, res_y, event_num); // Speksattu toistaiseksi vilin S3:selle, lisätään resoluutioflagit
    }
    return 0;
}
