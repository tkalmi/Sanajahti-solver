#include "ocr.hpp"

std::string ocr(int res_x, int res_y)
{
    
    int x_offset = res_x/9; // 129 on fullHD, 8.3721
    int y_offset = res_y/2.3659; // 751 on fullHD
    int tile_size_x = res_x/8.4375; // 128
    int tile_size_y = res_y/17.455; // 110
    int tile_offset = res_x/4.6956; //230
    char *outText;
    
    std::string luettu;

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "fin")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }
    api->ReadConfigFile("tess_config");
    api->SetPageSegMode(tesseract::PSM_SINGLE_CHAR);

    // Open input image with leptonica library
    Pix *image = pixRead("scrot.png");
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
    return luettu;
}
