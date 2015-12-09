#ifndef OCR_HPP
#define OCR_HPP

#include <tesseract/baseapi.h>
#include <tesseract/publictypes.h>
#include <leptonica/allheaders.h>
#include <tesseract/genericvector.h>
#include <iostream>
#include <sstream>
#include <fstream> // For reading the PNG's dimensions
#include <arpa/inet.h> // For converting unsigned integer's values between host and network byte order. ntohl()
#include <ctype.h>
#include "solver.hpp"

    /*
     * Creates a matrix string for the solver from screenshot of Sanajahti.
     * Returns std::string from the characters read. 
     * Parameters:
     * filu : filename of our screenshot.png 
     */

std::string ocr(std::string filu);

    /*
     * Creates a pair of width and height of our image file.
     * Returns a pair
     * Parameters:
     * filu : the filename where to fetch resolution data. 
     */

std::pair<int, int> get_res(std::string filu);

    /*
     * Creates a string from the char of word[index]
     * Special characters such as ä take multiple
     * bytes so normal word[index] does not word
     * Parameters:
     * word : string to get char at index
     * index : index of char in the word
     */
std::string char_to_str(std::string word, int index);

// TODO

#endif /* OCR_H */

