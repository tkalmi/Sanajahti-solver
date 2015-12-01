#ifndef GUI_H
#define GUI_H

#define FONT "Lazy Opossum.ttf" // font that is used. Note that the font file should be in the same folder as caller
#define PADDING 20 // how many pixels padding is 

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "solver.hpp"

/****************************************************************************/
/* Helper class for creating buttons */
class Button {
public:
	Button(unsigned int x, unsigned int y, unsigned int width, unsigned int height, std::string title) : x(x), y(y), width(width), height(height), title(title) {};
	void draw(sf::RenderWindow &window);
	bool isInside(sf::Vector2i mouse_pos);
private:
	unsigned int x; // x-coordinate of position
	unsigned int y; // y-coordinate of position
	unsigned int width;
	unsigned int height;
	std::string title;
	bool hover = false; // does mouse hover over button?
};

/****************************************************************************/
/* Helper class for creating buttons with title inside */
class ButtonTitleInside {
public:
	ButtonTitleInside(unsigned int x, unsigned int y, unsigned int width, unsigned int height, std::string title) : x(x), y(y), width(width), height(height), title(title) {};
	void draw(sf::RenderWindow &window);
	bool isInside(sf::Vector2i mouse_pos);
	void setTitle(std::string new_title);
private:
	unsigned int x; // x-coordinate of position
	unsigned int y; // y-coordinate of position
	unsigned int width;
	unsigned int height;
	std::string title;
	bool hover = false; // does mouse hover over button?
};

/****************************************************************************/
/* Helper class for creating textfields */
class TextField {
public:
	TextField(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool selected) : x(x), y(y), width(width), height(height), selected(selected) {};
	bool getSelected();
	void setSelected(bool is_selected);
	std::wstring getContent();
	void setContent(std::wstring new_content);	
	void draw(sf::RenderWindow &window);
	bool isInside(sf::Vector2i mouse_pos);
	void setInPath(bool is_in_path);
private:
	unsigned int x; // x-coordinate of position
	unsigned int y; // y-coordinate of position
	unsigned int width;
	unsigned int height;
	bool selected; // selected for input
	std::wstring content = L"";
	bool in_path = false; // is TextField in path, ie. should it be painted green
};

/****************************************************************************/
/* Helper class for creating a matrix of Textfields */
class TextFieldMatrix {
public:
	TextFieldMatrix(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int M, unsigned int N);
	TextField& getTile(unsigned int index);
	void draw(sf::RenderWindow &window);
	int isInside(sf::Vector2i mouse_pos);
	void selectTile(unsigned int index);
	TextField& getSelectedTile();
	unsigned int getSelectedTileIndex();
	void selectNextEmpty();
	bool isMatrixFilled();
	std::wstring getAsString();
private:
	unsigned int x; // x-coordinate of position
	unsigned int y; // y-coordinate of position
	unsigned int width;
	unsigned int height;
	unsigned int M; // how many textfields in a row
	unsigned int N; // how many textfields in a column
	std::vector<std::vector<TextField>> tiles;
};

/****************************************************************************/
/* Helper class for creating text labels without a specific background, such as titles and instructions */
class TextLabel {
public:
	TextLabel(std::wstring content, std::string font_name, float x_pos, float y_pos, unsigned int char_size);
	void draw(sf::RenderWindow &window);
	void centerLabelX(float midPointX);
	void fitLabelX(float containerXLimit);
	void setColor(sf::Color newColor);
	void setContent(std::wstring newContent);
private:
	std::wstring content;
	sf::Font font;
	float x;
	float y;
	unsigned int charSize;
	sf::Color color = sf::Color::White;
	sf::Text actualLabel;
};

/****************************************************************************/
class GUI {
public:
	GUI(unsigned int width, unsigned int height, std::set<std::wstring> dictionary) : width(width), height(height), dictionary(dictionary) {};

	int run();

private:
	unsigned int width;
	unsigned int height; 
	std::set<std::wstring> dictionary;
};

#endif // GUI_H
