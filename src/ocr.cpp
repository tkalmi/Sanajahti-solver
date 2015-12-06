#include "ocr.hpp"

std::string ocr(int res_x, int res_y, std::string filu = "scrot.png")
{
    
    int x_offset, y_offset;

    if (res_x == 1080) { // Honor 7 with the virtual buttons bar
	x_offset = res_x/8.3721;
    	y_offset = res_y/2.56;
	}
    else { // Samsung S3
	x_offset = res_x/9;
	y_offset = res_y/2.3659;
	}
    int tile_size_x = res_x/8.4375; // 128
    int tile_size_y = res_y/17.455; // 110
    int tile_offset = res_x/4.6956; //230
    char *outText;
    
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
    api->SetImage(image);

    // Get OCR result
    for (unsigned int j = 0; j < 4 ; j++) {
        for (unsigned int i = 0; i < 4; i++) {
        api->SetRectangle(x_offset+i*tile_offset, y_offset+j*tile_offset, tile_size_x, tile_size_y);
        outText = api->GetUTF8Text();
	luettu.push_back(*outText);
        }  
    }
    // Destroy used object and release memory
    api->End();
    delete [] outText;
    pixDestroy(&image);
//    std::replace(luettu.begin(), luettu.end(), ' ', 'p');
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

    std::cout << filu << ": dimensions are " << x << "x" << y << std::endl;
    std::pair<int, int> res (x,y);
    return res;
}
