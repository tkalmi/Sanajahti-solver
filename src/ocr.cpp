#include "ocr.hpp"
#include "solver.hpp"

std::string ocr(std::string filu)
{
    std::pair<int,int> res;
    res = get_res(filu);
    int res_x = res.first;
    int res_y = res.second;
    int x_offset, y_offset, tile_size_x, tile_size_y, tile_offset, no_dots_offset;

    if (res_x == 1080) { // Honor 7 with the virtual buttons bar
	x_offset = res_x/8.3721;
    	y_offset = res_y/2.56;
	}
    else { // Samsung S3
	x_offset = res_x/9;
	y_offset = res_y/2.3659;
	}
    tile_size_x = res_x/8.4375; // 128
    tile_size_y = res_y/17; // 110
    tile_offset = res_x/4.673; //230
    no_dots_offset = 0.011 * res_y;
    char *outText;
    
    std::stringstream ss;
    std::string luettu;

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "fra")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }
    api->ReadConfigFile("tess_config");
    api->SetPageSegMode(tesseract::PSM_SINGLE_CHAR);

    // Open input image with leptonica library
    Pix *image = pixRead(filu.c_str());
    if (image == NULL) {
	fprintf(stderr, "The file %s could not be read.\n",filu.c_str());
	throw std::exception();
    }    
    api->SetImage(image);
    // Get OCR result
    for (unsigned int j = 0; j < 4 ; j++) {
        for (unsigned int i = 0; i < 4; i++) {
            api->SetRectangle(x_offset+i*tile_offset, y_offset+j*tile_offset, tile_size_x, tile_size_y);
            outText = api->GetUTF8Text();
            if (*outText == ' ') {
                api->SetRectangle(x_offset+i*tile_offset, y_offset+j*tile_offset+no_dots_offset, tile_size_x, tile_size_y);
                outText = api->GetUTF8Text();
                if (*outText == 'O')
                    luettu += "ö";
                else if (*outText == 'A')
                    luettu += "ä";
            }
            else {
                luettu.push_back(tolower(*outText));
            }
        }
    }
    // Destroy used object and release memory
    api->End();
    delete [] outText;
    pixDestroy(&image);
    if (sj::utf8_to_wstring(luettu).size() != 16){
    	std::cout << "Could not detect matrix correctly. \n";
	throw std::exception();
     	}
    return luettu;
}

std::pair<int, int> get_res(std::string filu) { // Get the resolution from PNGs header. For additional reference, see http://www.fileformat.info/format/png/corion.htm (IDHR)
    std::ifstream kuva(filu);
    unsigned int x,y;
    kuva.seekg(16);
    kuva.read((char *)&x, 4);
    kuva.read((char *)&y, 4);

    x = ntohl(x);
    y = ntohl(y);

    std::pair<int, int> res (x,y);
    return res;
}

std::string char_to_str(std::string word, int index){
    std::wstringstream wss;
    std::wstring tmp = sj::utf8_to_wstring(word);
    if (index < 0 || index > static_cast<int>(tmp.size())){
        return "";
    }
    wss << tmp[index];
    tmp = wss.str();
    return sj::utf8_to_string(tmp);
}
