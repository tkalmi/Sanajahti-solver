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
#include <string>

bool s_interrupt = false;

static void handler(int signal) {
    switch (signal) {
        case SIGINT:
            fprintf(stderr, "Caught interrupt signal \n");
            s_interrupt = true;
            exit(1);
            break;
/* Not in use at the moment. Further developmen may require.
        case SIGILL:
            fprintf(stderr, "Illegal instruction\n");
            break;
        case SIGABRT:
            fprintf(stderr, "Caught SIGABRT: usually caused by and abort() or assert()\n");
            break;
        case SIGFPE:
            fprintf(stderr, "Caught SIGFPE: arithmetic exception\n");
            break;
*/
        case SIGSEGV:
            fprintf(stderr, "Caught SIGSEGV meaning segmentation fault. Perhaps GPU drivers fails? \n");
            exit(1);
            break;
        case SIGTERM:
        default:
            fprintf(stderr, "SIGTERM: termination request was sent to this program. Good bye!\n");
            exit(1);
            break;
    }
}

struct settings {
    bool text_ui = false; // Default values allowed in C++11.
    bool ocr_on = false;
    bool android_input = false;
    bool with_paths = false;
    int event_num = 3;
    int M = 4;
    int N = 4;

    std::string words_filename = "sanat.txt"; //default wordlist. Changeable by command flags.
    std::string ocr_filename = "scrot.png"; // default image file (PNG) to be used with OCR-library.
    std::string matrix_as_string;
};

struct settings parse_settings(int argc, char **argv) {
    settings opt;
    int options;
    std::string str; //Helper variable for option -m
    bool loytyy_x = false; // Helper variable with matrix validity check.

    opterr = 0; // getopt() parameters

    while ((options = getopt(argc, argv, "ac:e:hm:oO:pw:")) != -1) {
        switch (options) {
            case 'a': // Android support
                opt.android_input = true;
                break;
            case 'c': // Disabling GUI, printing to shell.
                if (opt.ocr_on == false)
                    opt.matrix_as_string = optarg;
                opt.text_ui = true;
                break;
            case 'e':
                opt.event_num = atoi(optarg);
                std::cout << "event number: " << optarg << std::endl;
                break;
	    case 'h':
		std::cout 
			<< "ssolver - Sanajahti solver" << std::endl
			<< "Solves words from Sanajahti game with a fast algorithm." << std::endl
			<< std::endl << std::endl
			<< "Usage: ssolver [-ahop] [-c matrix_as_string] [-e event_num] [-m MxN] [-O image.png] [-w wordlist]" << std::endl
			<< "	-a		Enable Android input. Inputs words to Android-phone via ADB bridge, if device is available." << std::endl
			<< "	-c		Input matrix as string. E.g. wordwordwordword." << std::endl
			<< "	-e		Change the to what Android's /dev/input/event<num> we will input. Default without using -e is 3." << std::endl
			<< "	-h		Display this help print." << std::endl
			<< "	-m		Input custom size matrix with syntax MxN. For example, ./ssolver -m 4x4" << std::endl
			<< "	-o		Enable OCR (optical character recognition) support. Fetches screenshot from live game via ADB and solves words from it." << std::endl
			<< "	-O		Enale OCR with custom image. For example, -O image.png. Does not fetch screenshot from Android." << std::endl
			<< "	-p		Print word with their corresponding path. Coordinate (0,0) is the top left corner." << std::endl
			<< "	-w		Use custom word list. -w words.txt" << std::endl;
		exit(0);
		break; 
            case 'm': // Matrix input, input as MxN with the 'x' between numbers.
                if (optarg[0] == '0' || optarg[0] == 'x') {
                    fprintf(stderr, "Option -m input format is: MxN, for example 4x4\n");
                    exit(1);
                }
                for (unsigned int i = 0; i < strlen(optarg); i++) {
                    if (optarg[i] == 'x') {
                        if (loytyy_x) {// For double 'x's, for example 4x4x4 not allowed
                            fprintf(stderr, "Only 2d matrises are supported, sorry.\n");
                            exit(1);
                        }
                        loytyy_x = true;
                        opt.M = std::stoi(str);
                        str.erase();
                        continue;
                    }
                    str.push_back(optarg[i]);
                }
                opt.N = std::stoi(str);
                if (!loytyy_x) {
                    fprintf(stderr, "Option -m input format is: MxN, for example 4x4\n");
                    exit(1);
                }
                fprintf(stderr, "M: %d, N: %d\n", opt.M, opt.N);
                break;
            case 'o':
                opt.ocr_on = true;
                try { // If tesseract fails
                    system("adb shell screencap -p /sdcard/scrot.png && adb pull /sdcard/scrot.png && `adb shell rm /sdcard/scrot.png &>/dev/null`");
                    opt.matrix_as_string = ocr(opt.ocr_filename);
//                    if (std::remove(opt.ocr_filename.c_str()) != 0)
                    //    std::cout << "Could not remove temporary screenshot file: " << opt.ocr_filename << std::endl;
                } catch (std::exception e) {
                    exit(1);
                }
                break;
            case 'O':
                opt.ocr_on = true;
                opt.ocr_filename = optarg;
                try { // If tesseract fail
                    opt.matrix_as_string = ocr(opt.ocr_filename);
                } catch (std::exception e) {
                    exit(1);
                }
                break;
            case 'p': // Printing words paths as well
                opt.with_paths = true;
                break;
            case 'w': // Defining other wordlist to be used.
                opt.words_filename = optarg;
                std::cout << "filename: " << opt.words_filename << std::endl;
                break;
            case '?':

                if (optopt == 'c' || optopt == 'e' || optopt == 'w' || optopt == 'C') {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt); // TODO add here help page reference
                    exit(1);
                } else {
                    fprintf(stderr, "Invalid option -%c\n", optopt);
                    exit(1);
                }
                break;
            default:
                exit(1);
        }
    }
    return opt;
}

std::set<std::wstring> read_words_from_file(std::string words_filename) { // get word list from file
    std::ifstream file;
    std::string line;
    std::set<std::wstring> words;
    file.open(words_filename);

    while (std::getline(file, line)) {
        words.insert(sj::utf8_to_wstring(line));
    }
    file.close();
    return words;
}

int main(int argc, char **argv) {
    signal(SIGINT, handler); // CTRL+C
    signal(SIGSEGV, handler);
    signal(SIGTERM, handler);


    setlocale(LC_ALL, "");
    setlocale(LC_NUMERIC, "en_US.utf8"); // OCR requirement.

    struct settings opt = parse_settings(argc, argv);

    //std::cout << opt.words_filename;
    std::set<std::wstring> words = read_words_from_file(opt.words_filename); // Read words from file to memory.


    if (opt.text_ui == true || opt.ocr_on == true) {
        if (sj::utf8_to_wstring(opt.matrix_as_string).size() != static_cast<unsigned int> (opt.M * opt.N)) {
            std::cout << "Matrix size does not match dimensions!\n";
            return 1;
        }
        for (unsigned int i = 0; i < sj::utf8_to_wstring(opt.matrix_as_string).size(); i++) {
            std::cout << char_to_str(opt.matrix_as_string, i);
            if ((i + 1) % opt.M == 0)
                std::cout << std::endl;
        }
        sj::Solver solver(words, opt.matrix_as_string, opt.M, opt.N);
        std::vector<sj::Path> solved_words = solver.Paths();
        std::cout << "Found words:" << std::endl;
        for (auto i : solved_words) {
            if (opt.with_paths == true)
                std::cout << i.path_str() << std::endl;
            else
                std::cout << i.word() << std::endl;
        }
        std::cout << "Total words: " << solved_words.size() << std::endl;
        if (opt.android_input == false)
            return 1;
    }
    if (opt.android_input == true) { // Input solver's results to Android via ADB-connection.
        if (opt.matrix_as_string.size() == 0)
            fprintf(stderr, "Nothing to do. Give me matrix!\n");
        sj::Solver solver(words, opt.matrix_as_string, opt.M, opt.N);
        std::pair<int, int> res = get_res(opt.ocr_filename);
        int res_x = res.first;
        int res_y = res.second;
        solver.Android_Solve(res_x, res_y, opt.event_num);
    } else if (opt.text_ui == false) { // Start GUI.
        GUI gui(960, 640, words);
        gui.run();
        return 0;
    }

    return 0;
}
