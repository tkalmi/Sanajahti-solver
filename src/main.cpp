#include <iostream>
#include <fstream>
#include <vector>
#include "GUI.hpp"

int main(void)
{
	// get word list from file
	std::ifstream file;
	std::string line;
	std::set<std::string> words;
	file.open("sanat.txt");
	while (std::getline(file, line)){
		words.insert(line);
	}
	file.close();

	GUI gui(960, 640, words);
	gui.run();
	return 0;
}
