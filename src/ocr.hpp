#ifndef OCR_HPP
#define OCR_HPP

#include <tesseract/baseapi.h>
#include <tesseract/publictypes.h>
#include <leptonica/allheaders.h>
#include <tesseract/genericvector.h>
#include <iostream>
#include <fstream> // For reading the PNG's dimensions
#include <arpa/inet.h> // For converting unsigned integer's values between host and network byte order. ntohl()
#include <ctype.h>

#include "solver.hpp"
#include <sstream>


// Just testing 

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

// TODO

#endif /* OCR_H */

