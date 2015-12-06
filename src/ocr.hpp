#ifndef OCR_HPP
#define OCR_HPP

#include <tesseract/baseapi.h>
#include <tesseract/publictypes.h>
#include <leptonica/allheaders.h>
#include <tesseract/genericvector.h>
#include <iostream>
#include <fstream> // For reading the PNG's dimensions
#include <arpa/inet.h> // For converting unsigned integer's values between host and network byte order. ntohl()
#include <algorithm>
#include "solver.hpp"


// Just testing 

std::string ocr(int res_x, int res_y, std::string filu);

std::pair<int, int> get_res(std::string filu);

// TODO

#endif /* OCR_H */

