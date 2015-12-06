#ifndef OCR_HPP
#define OCR_HPP

#include <tesseract/baseapi.h>
#include <tesseract/publictypes.h>
#include <leptonica/allheaders.h>
#include <tesseract/genericvector.h>
#include <iostream>
#include <algorithm>
#include "solver.hpp"

// Just testing 

std::string ocr(int res_x, int res_y);

std::pair<int, int> get_res();

// TODO

#endif /* OCR_H */

