#include <gtest/gtest.h> //GoogleTest libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <dirent.h>

#include "../GUI.hpp"
#include "../solver.hpp"
#include "../ocr.hpp"

/*
 * TODO:
 * Code the tests
 */
/*
TEST(Testin_nimi, Mita_testataan){ //Replace the parameters with the correct equivalents
    EXPECT_EQ(2, 2); // Replace this
    EXPECT_EQ("anything", "something else"); // And this
}
*/
TEST(test_algorithm, normal){ //Replace the parameters with the correct equivalents
    std::ifstream file;
    std::string line;
    std::set<std::wstring> words;
    std::string filename = "../sanat.txt";
    file.open(filename);
    while (std::getline(file, line)){
        words.insert(sj::utf8_to_wstring(line));
    }
    file.close();
    auto solved_words = sj::Solver(words,"ointiikzsaatennm", 4, 4);
    auto solved_words2 = sj::Solver(words,"esionaiinaknmtzt", 4, 4); //90 degree flip clockwise
    EXPECT_EQ(solved_words.Paths()[0].word(), "makasiini");
    EXPECT_EQ(solved_words.Paths()[0].w_word(), L"makasiini");
    EXPECT_EQ(solved_words2.Paths()[0].word(), "makasiini");
    EXPECT_EQ(solved_words2.Paths()[0].w_word(), L"makasiini");
    EXPECT_EQ(solved_words.Paths().size(), solved_words.Paths().size());
}
TEST(test_algorithm2, long_words){
    std::ifstream file;
    std::string line;
    std::set<std::wstring> words;
    std::string filename = "../sanat.txt";
    file.open(filename);
    while (std::getline(file, line)){
        words.insert(sj::utf8_to_wstring(line));
    }
    file.close();
    auto solved_words = sj::Solver(words,"aamujuxxxapmxsulnaxleval", 6, 4);
    EXPECT_EQ(solved_words.Paths()[0].word(), "aamujumalanpalvelus");
    solved_words = sj::Solver(words,"xuupuenvaaskolotixukkielhaasbbccdddc,", 9, 4);
    EXPECT_EQ(solved_words.Paths()[0].word(), "sukupuolenvaihtoleikkaus");
}
TEST(test_algorithm3, single_word){
    std::ifstream file;
    std::string line;
    std::set<std::wstring> words;
    std::string filename = "../sanat.txt";
    file.open(filename);
    while (std::getline(file, line)){
        words.insert(sj::utf8_to_wstring(line));
    }
    file.close();
    auto solved_words = sj::Solver(words,"xixxsxxaxxax", 3, 4);
    EXPECT_EQ(solved_words.Paths()[0].word(), "aasi");
    EXPECT_EQ(solved_words.Paths().size(), 1);
}

TEST(test_algorithm4, special_characters){
    std::ifstream file;
    std::string line;
    std::set<std::wstring> words;
    std::string filename = "../sanat.txt";
    file.open(filename);
    while (std::getline(file, line)){
        words.insert(sj::utf8_to_wstring(line));
    }
    file.close();
    auto solved_words = sj::Solver(words,"xxxxxöytxlätxxix", 4, 4);
    EXPECT_EQ(solved_words.Paths()[0].word(), "öylätti");
}

TEST(test_algorithm5, dictionaries){
    std::ifstream file;
    std::string line;
    std::set<std::wstring> words;
    std::string filename = "../american-english";
    file.open(filename);
    while (std::getline(file, line)){
        words.insert(sj::utf8_to_wstring(line));
    }
    file.close();
    auto solved_words = sj::Solver(words,"nxxxetsxxtiexxfmxxxo", 4, 5);
    EXPECT_EQ(solved_words.Paths()[0].word(), "oftentimes");
}

TEST(test_algorithm6, constructors){
    std::ifstream file;
    std::string line;
    std::set<std::wstring> words;
    std::string filename = "../sanat.txt";
    file.open(filename);
    while (std::getline(file, line)){
        words.insert(sj::utf8_to_wstring(line));
    }
    file.close();
    //Each solved_words has different constructor
    auto solved_words = sj::Solver(words,L"ointiikzsaatennm", 4, 4);
    auto solved_words2 = sj::Solver(words,"ointiikzsaatennm", 4, 4);
    file.open(filename);
    std::set<std::string> words2;
    while(std::getline(file, line)){
        words2.insert(line);
    }
    file.close();
    auto solved_words3 = sj::Solver(words2,L"ointiikzsaatennm", 4, 4);
    auto solved_words4 = sj::Solver(words2,"ointiikzsaatennm", 4, 4);
    
    EXPECT_EQ(solved_words.Paths()[0].word(), solved_words2.Paths()[0].word());
    EXPECT_EQ(solved_words.Paths()[0].w_word(), solved_words2.Paths()[0].w_word());
    
    EXPECT_EQ(solved_words.Paths()[0].word(), solved_words3.Paths()[0].word());
    EXPECT_EQ(solved_words.Paths()[0].w_word(), solved_words3.Paths()[0].w_word());
    
    EXPECT_EQ(solved_words.Paths()[0].word(), solved_words4.Paths()[0].word());
    EXPECT_EQ(solved_words.Paths()[0].w_word(), solved_words4.Paths()[0].w_word());
    
    EXPECT_EQ(solved_words2.Paths()[0].word(), solved_words3.Paths()[0].word());
    EXPECT_EQ(solved_words2.Paths()[0].w_word(), solved_words3.Paths()[0].w_word());
    
    EXPECT_EQ(solved_words2.Paths()[0].word(), solved_words4.Paths()[0].word());
    EXPECT_EQ(solved_words2.Paths()[0].w_word(), solved_words4.Paths()[0].w_word());
    
    EXPECT_EQ(solved_words3.Paths()[0].word(), solved_words4.Paths()[0].word());
    EXPECT_EQ(solved_words3.Paths()[0].w_word(), solved_words4.Paths()[0].w_word());
}

TEST(test_algorithm7, OCR) {
    DIR *dip;
    struct dirent *dit;

    int count = -3; // . .. and answers.txt

    if ((dip = opendir("testpics/")) == NULL) {
        fprintf(stderr,"Could not find testpics folder.\n");
	exit(1);
	}
    while ((dit = readdir(dip)) != NULL) {
	count++;
	}
    
    std::ifstream file;
    file.open("testpics/answers.txt");
    std::stringstream ss;
    std::string scrot;
    std::string line;

    for (int i = 0; i < count; i++) {
	std::cout << count;
	ss << "testpics/scrot_" << i << ".png";
	scrot = ocr(ss.str());
	std::cout << "Picture: " << ss.str() << " - " << scrot << std::endl; 
	std::getline(file,line);
	ss.str("");
	EXPECT_EQ(scrot,line);
	}
    file.close();
}
