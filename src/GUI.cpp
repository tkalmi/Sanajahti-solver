#include "GUI.hpp"
#include "Screens.hpp"
#include <map>

/****************************************************************************/
/* Check if mouse clicked inside the button. If it did, change hover to true */
bool Button::isInside(sf::Vector2i mouse_pos)
{
	if (mouse_pos.x >= (int) x && mouse_pos.x <= (int) x + (int) width && mouse_pos.y >= (int) y && mouse_pos.y <= (int) y + (int) height) {
		hover = true;
		return true;
	}
	hover = false;
	return false;
}

void Button::draw(sf::RenderWindow &window) {
	sf::RectangleShape button(sf::Vector2f((float)width, (float)height)); // button is a rectangle
	button.setPosition(sf::Vector2f((float)x, (float)y));
	if (hover) // base color depends on whether the mouse hovers on the button or not
		button.setFillColor(sf::Color(255, 0, 0));
	else
		button.setFillColor(sf::Color(40, 97, 194));
	window.draw(button); // draw the rectangle

	// draw button title
	sf::Text text;
	sf::Font font;
	if (!font.loadFromFile(FONT))
	{
		// error...
	}
	text.setFont(font);
	text.setString(title);

	// Set the new text height
	text.setCharacterSize(static_cast<unsigned int>(height));

	// Get the size of the text
	sf::FloatRect bounds = text.getLocalBounds();
	if (bounds.width > width)
		text.setCharacterSize(static_cast<unsigned int>(floor((height * width) / bounds.width)));

	// The text still has to fit inside the area
	while (bounds.width > width)
	{
		// Adjust the text size
		text.setCharacterSize(text.getCharacterSize() - 1);
		bounds = text.getLocalBounds();
	}

	text.setPosition(x + width / 2.0f - bounds.width / 2.0f, y - text.getCharacterSize() - 5.0f);
	text.setColor(sf::Color::White);
	window.draw(text);
}

/****************************************************************************/
/* Check if mouse clicked inside the button. If it did, change hover to true */
bool ButtonTitleInside::isInside(sf::Vector2i mouse_pos)
{
	if (mouse_pos.x >= (int)x && mouse_pos.x <= (int)x + (int)width && mouse_pos.y >= (int)y && mouse_pos.y <= (int)y + (int)height) {
		hover = true;
		return true;
	}
	hover = false;
	return false;
}

void ButtonTitleInside::draw(sf::RenderWindow &window) {
	sf::RectangleShape button(sf::Vector2f((float)width, (float)height)); // button is a rectangle
	button.setPosition(sf::Vector2f((float)x, (float)y));
	if (hover) // base color depends on whether the mouse hovers over the button or not
		button.setFillColor(sf::Color(255, 0, 0));
	else
		button.setFillColor(sf::Color(40, 97, 194));
	window.draw(button); // draw the rectangle

	// draw button title
	sf::Text text;
	sf::Font font;
	if (!font.loadFromFile(FONT))
	{
		// error...
	}
	text.setFont(font);
	text.setString(title);

	// Set the new text height
	text.setCharacterSize(static_cast<unsigned int>(height));

	// Get the size of the text
	sf::FloatRect bounds = text.getLocalBounds();
	if (bounds.width > width)
		text.setCharacterSize(static_cast<unsigned int>(floor((height * width) / bounds.width)));

	// The text still has to fit inside the area
	while (bounds.width > width)
	{
		// Adjust the text size
		text.setCharacterSize(text.getCharacterSize() - 1);
		bounds = text.getLocalBounds();
	}

	// Center the content text
	text.setPosition(x + width / 2 - bounds.width / 2,
		y + height / 2 - bounds.height / 2 - bounds.top
		);
	text.setColor(sf::Color::White);
	window.draw(text);
}

/****************************************************************************/
bool TextField::getSelected()
{
	return selected;
}

void TextField::setSelected(bool is_selected)
{
	selected = is_selected;
}

std::wstring TextField::getContent()
{
	return content;
}

void TextField::setContent(std::wstring new_content)
{
	content = new_content;
}

void TextField::draw(sf::RenderWindow &window)
{
	sf::RectangleShape textfield(sf::Vector2f((float)width, (float)height)); // button is a rectangle
	textfield.setPosition(sf::Vector2f((float)x, (float)y));
	if (selected) // if this textfield is selected, we paint it white
		textfield.setFillColor(sf::Color::White);
	else // otherwise we paint the textfield grey
		textfield.setFillColor(sf::Color(200, 200, 200));
	if (in_path) // if textfield is in path, we paint it green
		textfield.setFillColor(sf::Color(0, 255, 0));
	window.draw(textfield); // draw the rectangle

	// draw textfield content
	sf::Text text;
	sf::Font font;
	if (!font.loadFromFile(FONT)) // HUOM! fontin oltava samassa kansiossa, tai kirjoita path
	{
		// error...
	}
	text.setFont(font);
	text.setString(content);

	// Set the new text height
	text.setCharacterSize(static_cast<unsigned int>(height));

	// Get the size of the text
	sf::FloatRect bounds = text.getLocalBounds();
	if (bounds.width > width)
		text.setCharacterSize(static_cast<unsigned int>(floor((height * width) / bounds.width)));

	// The text still has to fit inside the area
	while (bounds.width > width)
	{
		// Adjust the text size
		text.setCharacterSize(text.getCharacterSize() - 1);
		bounds = text.getLocalBounds();
	}

	// Center the content text
	text.setPosition(x + width / 2 - bounds.width / 2,
					 y + height / 2 - bounds.height / 2 - bounds.top
					 );
	text.setColor(sf::Color::Black);
	window.draw(text);
}

bool TextField::isInside(sf::Vector2i mouse_pos)
{
	if (mouse_pos.x >= (int)x && mouse_pos.x <= (int)x + (int)width && mouse_pos.y >= (int)y && mouse_pos.y <= (int)y + (int)height)
		return true;
	return false;

}

void TextField::setInPath(bool is_in_path)
{
	in_path = is_in_path;
}

/****************************************************************************/
TextFieldMatrix::TextFieldMatrix(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int M, unsigned int N)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->M = M;
	this->N = N;

	// calculate parameters for textfields
	unsigned int tf_width = width / M - 2;
	unsigned int tf_height = height / N - 2;
	unsigned int cur_x = x;
	unsigned int cur_y = y;
	unsigned int incr_x = width / M;
	unsigned int incr_y = height / N;

	// initialize cells of matrix
	for (unsigned int y = 0; y < N; y++) {
		tiles.push_back(std::vector<TextField>());
		for (unsigned int x = 0; x < M; x++) {
			TextField tile(cur_x, cur_y, tf_width, tf_height, false);
			tiles[y].push_back(tile);
			cur_x += incr_x;
		}
		cur_y += incr_y;
		cur_x = this->x;
	}
}

TextField& TextFieldMatrix::getTile(unsigned int index)
{
	unsigned int row = (unsigned int) floor(index / M);
	unsigned int col = index - M * row;
	return tiles[row][col];
}

void TextFieldMatrix::draw(sf::RenderWindow &window) 
{
	for (unsigned int y = 0; y < N; y++) {
		for (unsigned int x = 0; x < M; x++) {
			tiles[y][x].draw(window);
		}
	}
}

/* Returns the index of the matrix cell that the mouse touches, or else returns -1 */
int TextFieldMatrix::isInside(sf::Vector2i mouse_pos) {
	if (mouse_pos.x >= (int)x && mouse_pos.x <= (int)x + (int)width && mouse_pos.y >= (int)y && mouse_pos.y <= (int)y + (int)height) { // check if mouse is inside the matrix
		for (unsigned int i = 0; i < M * N; i++) { // loop through all textfields
			if (this->getTile(i).isInside(mouse_pos))
				return i;
		}
	}
	return -1;
}

/* set tile at index to selected */
void TextFieldMatrix::selectTile(unsigned int index) {
	for (unsigned int i = 0; i < M * N; i++) { // loop through all textfields and set them to false
		this->getTile(i).setSelected(false);
	}
	this->getTile(index).setSelected(true); // set the right tile to selected
}

TextField& TextFieldMatrix::getSelectedTile()
{
	for (unsigned int i = 0; i < M * N; i++) { // loop through all textfields
		if (this->getTile(i).getSelected())
			return this->getTile(i);
	}
	// return first tile if something went wrong in the loop
	return this->getTile(0);
}

unsigned int TextFieldMatrix::getSelectedTileIndex()
{
	for (unsigned int i = 0; i < M * N; i++) { // loop through all textfields
		if (this->getTile(i).getSelected())
			return i;
	}
	// return 0 if something went wrong in the loop
	return 0;
}

void TextFieldMatrix::selectNextEmpty()
{
	unsigned int size = M * N;
	unsigned int selected_ind = this->getSelectedTileIndex();

	for (unsigned int i = selected_ind + 1; i < size; i++) { // select next empty tile AFTER the currently selected tile
		if (this->getTile(i).getContent() == L"") {
			this->getTile(selected_ind).setSelected(false);
			this->getTile(i).setSelected(true);
			return;
		}
	}
	// if no empty tiles after the currently selected tile, select FIRST empty tile
	for (unsigned int i = 0; i < selected_ind; i++) {
		if (this->getTile(i).getContent() == L"") {
			this->getTile(selected_ind).setSelected(false);
			this->getTile(i).setSelected(true);
			return;
		}
	}
	// if no empty tiles, then nothing is done
}

/* checks if all matrix cells are filled */
bool TextFieldMatrix::isMatrixFilled()
{
	for (unsigned int i = 0; i < M * N; i++) { // loop through all textfields
		if (this->getTile(i).getContent() == L"")
			return false;
	}
	return true;
}

std::wstring TextFieldMatrix::getAsString() 
{
	std::wstring contents = L"";
	for (unsigned int i = 0; i < M * N; i++) {
		contents += this->getTile(i).getContent();
	}
	return contents;
}

/****************************************************************************/
/* main loop for GUI*/
int GUI::run()
{
	sf::RenderWindow window(sf::VideoMode(width, height, 32), "Sanajahti-solver"); // create window

	std::map<std::string, Screen*> screens; // store different screens into a map
	std::string screen = "main_menu";

	// init screens and insert to screen map
	MainMenu s0;
	screens["main_menu"] = &s0;

	unsigned int M;
	unsigned int N;
	std::wstring matrix_as_string;

	SizeQuery *s1 = new SizeQuery(M, N);
	screens["size_query"] = s1; 

	InputMatrix *s2 = new InputMatrix(M, N, matrix_as_string);
	screens["input_matrix"] = s2;

	SolveScreen *s3 = new SolveScreen(M, N, matrix_as_string, words);
	screens["solve_screen"] = s3;

	// main run loop
	while (window.isOpen() && screen != "exit") {
		screen = screens[screen]->Run(window);
	}

	window.close();
	return EXIT_SUCCESS;
}