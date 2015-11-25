#include "ocr.hpp"

std::string ocr();
{
    int res_x = 1080;
    int res_y = 1920;
    
    int x_offset = res_x/8.3721;
    int y_offset = res_y/2.4903;
    int tile_size_x = res_x/8.4375;
    int tile_size_y = res_y/21.099;
    int tile_offset = res_x/4.6956;
    char *outText;
    
    std::string luettu;
    
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }

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
    std::cout << luettu << std::endl;
    // Destroy used object and release memory
    api->End();
    delete [] outText;
    pixDestroy(&image);
    return luettu;
}