#ifndef SCREENS_H
#define SCREENS_H

#include "GUI.hpp"
#include "ocr.hpp"
#include <thread>

/****************************************************************************/
/* Virtual class for all different screens (main menu, matrice input screen, etc.) */
class Screen {
public:
	virtual std::string Run(sf::RenderWindow &window) = 0;
protected:
	std::string ocr_filename = "scrot.png"; // filename required by OCR functionality
};

/****************************************************************************/
class MainMenu : public Screen {
public:
	MainMenu(unsigned int &M, unsigned int &N, std::wstring &matrix_as_string) : M(M), N(N), matrix_as_string(matrix_as_string) {};
	virtual std::string Run(sf::RenderWindow &window);
private:
	unsigned int M;
	unsigned int N;
	std::wstring matrix_as_string;
};

/****************************************************************************/
class SizeQuery : public Screen {
public:
	SizeQuery(unsigned int &M, unsigned int &N) : x_dim(M), y_dim(N) {};
	virtual std::string Run(sf::RenderWindow &window);
private:
	unsigned int &x_dim; // M-dimension of matrix
	unsigned int &y_dim; // N-dimension of matrix
};

/****************************************************************************/
class InputMatrix : public Screen {
public:
	InputMatrix(unsigned int &M, unsigned int &N, std::wstring &matrix_as_string) : M(M), N(N), matrix_as_string(matrix_as_string) {};
	virtual std::string Run(sf::RenderWindow &window);
private:
	unsigned int &M; // M-dimension of matrix
	unsigned int &N; // N-dimension of matrix
	std::wstring &matrix_as_string;
};

/****************************************************************************/
class SolveScreen : public Screen {
public:
	SolveScreen(unsigned int &M, unsigned int &N, std::wstring &matrix_as_string, std::set<std::wstring> dictionary) : M(M), N(N), matrix_as_string(matrix_as_string), dictionary(dictionary) {};
	virtual std::string Run(sf::RenderWindow &window);
private:
	unsigned int &M; // M-dimension of matrix
	unsigned int &N; // N-dimension of matrix
	std::wstring &matrix_as_string; 
	std::set<std::wstring> dictionary; // kotus-list or similar
};

#endif // SCREENS_H
