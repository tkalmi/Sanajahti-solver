#include <iostream>
#include "Screens.hpp"
#define BACKGROUND_COLOR sf::Color(255, 150, 0) // background color yellow-orange-ish

using namespace sj;

/****************************************************************************/
/* MainMenu screen */
std::string MainMenu::Run(sf::RenderWindow &window)
{
	/* COMPONENTS INIT */
	// title for main menu
	TextLabel title(
		L"Sanajahti Solver",
		FONT,
		0.0f,
		(window.getSize().y / 40.0f),
		(unsigned int)(window.getSize().y / 8.0f)
		);
	title.centerLabelX(window.getSize().x / 2.0f); // set title x position to middle of the screen
	
	// button for inputting own matrix
	Button teeItse(
		(unsigned int)(window.getSize().x / 4.0f),
		(unsigned int)(window.getSize().y / 20.0f * 9.0f),
		(unsigned int)(window.getSize().x / 5.0f),
		(unsigned int)(window.getSize().x / 5.0f),
		"Tee oma matriisi"
		);
	// button for getting matrix from Android
	Button android(
		(unsigned int)(window.getSize().x / 4.0f * 3.0f - (unsigned int)window.getSize().x / 5.0f),
		(unsigned int)(window.getSize().y / 20.0f * 9.0f),
		(unsigned int)(window.getSize().x / 5.0f),
		(unsigned int)(window.getSize().x / 5.0f),
		"Matriisi Androidista"
		);

	/* COMPONENTS END INIT */

	// run the menu
	while (true) {
		sf::Event event;
		while (window.pollEvent(event)) {
			/* user closed the window */
			if (event.type == sf::Event::Closed)
				return "exit";
			/* user moved the mouse */
			if (event.type == sf::Event::MouseMoved) { // check if mouse hovers over any of the buttons
				teeItse.isInside(sf::Mouse::getPosition(window));
				android.isInside(sf::Mouse::getPosition(window));
			}
			/* user clicked */
			if (event.type == sf::Event::MouseButtonPressed) {
				if (teeItse.isInside(sf::Mouse::getPosition(window))) // user clicked "Tee oma matriisi"
					return "size_query";
				if (android.isInside(sf::Mouse::getPosition(window))) // user clicked "Hae Androidista"
					try {
						M = 4;
						N = 4;
						matrix_as_string = sj::utf8_to_wstring(ocr(ocr_filename));
						if (matrix_as_string.size() != 16)
							throw std::exception();
						return "solve_screen";
					} catch (std::exception e) {
						std::cout << e.what() << std::endl;
					}
			}
		}

		/* actual rendering */
		window.clear(BACKGROUND_COLOR);
		teeItse.draw(window);
		android.draw(window);
		title.draw(window);

		window.display();
	}

	return "main_menu";
}

/****************************************************************************/
/* SizeQuery screen. Asks the user for dimensions of M x N matrix  */
std::string SizeQuery::Run(sf::RenderWindow &window)
{
	/* COMPONENTS INIT */
	TextField M_field(
		(unsigned int)(window.getSize().x / 4.0f),
		(unsigned int)(window.getSize().y / 20.0f * 7.0f),
		(unsigned int)(window.getSize().x / 5.0f),
		(unsigned int)(window.getSize().x / 5.0f), 
		true); // textfield for M value
	TextField N_field(
		(unsigned int)(window.getSize().x / 4.0f * 3.0f - (unsigned int)window.getSize().x / 5.0f),
		(unsigned int)(window.getSize().y / 20.0f * 7.0f),
		(unsigned int)(window.getSize().x / 5.0f),
		(unsigned int)(window.getSize().x / 5.0f),
		false); // textfield for N value
	Button jatka(
		(unsigned int)(window.getSize().x / 2.0f - (unsigned int)window.getSize().x / 24.0f),
		(unsigned int)(window.getSize().y / 20.0f * 15.0f),
		(unsigned int)(window.getSize().x / 12.0f),
		(unsigned int)(window.getSize().x / 20.0f),
		"Jatka"
		); // button for continuing
	ButtonTitleInside takaisin(
		(unsigned int)(window.getSize().x - window.getSize().x / 12.0f - PADDING),
		PADDING,
		(unsigned int)(window.getSize().x / 12.0f),
		(unsigned int)(window.getSize().x / 20.0f),
		"Takaisin"
		); // button for going back
	TextLabel X(
		L"x",
		FONT,
		0,
		(unsigned int)window.getSize().y / 20.0f * 9.0f,
		(unsigned int)(window.getSize().x / 20.0f)
		); // multiplication operator in between the dimension fields
	X.centerLabelX(window.getSize().x / 2.0f); // set the x position of X to the middle of the screen
	X.setColor(sf::Color::Black);

	TextLabel title(
		L"Syötä matriisin mitat",
		FONT,
		0,
		window.getSize().y / 40.0f,
		(unsigned int)(window.getSize().y / 8.0f)
		); // title of screen
	title.centerLabelX(window.getSize().x / 2.0f); // set the x position of title to the middle of the screen

	/* COMPONENTS END INIT */

	bool backspaceUsed = true; // for ensuring that backspace is used for one digit at a time
	bool returnUsed = true; // for ensuring that enter is used for one change of focus at a time

	// run the screen
	while (true) {
		sf::Event event;
		while (window.pollEvent(event)) {
			/* user closed the window */
			if (event.type == sf::Event::Closed)
				return "exit";
			/* user moved mouse */
			if (event.type == sf::Event::MouseMoved) { // check if mouse hovers over any of the buttons
				jatka.isInside(sf::Mouse::getPosition(window));
				takaisin.isInside(sf::Mouse::getPosition(window));
			}
			/* user clicked */
			if (event.type == sf::Event::MouseButtonPressed) {
				if (M_field.isInside(sf::Mouse::getPosition(window))) { // user clicked M_field
					M_field.setSelected(true);
					N_field.setSelected(false);
				}
				if (N_field.isInside(sf::Mouse::getPosition(window))) { // user clicked N_field
					M_field.setSelected(false);
					N_field.setSelected(true);
				}
				if (jatka.isInside(sf::Mouse::getPosition(window))) { // user clicked "jatka"
					if (M_field.getContent() != L"" && N_field.getContent() != L"") { // check that the M and N fields are properly filled
						// pass the dimensions and move on to input matrix screen
						x_dim = std::stoi(M_field.getContent());
						y_dim = std::stoi(N_field.getContent());
						return "input_matrix";
					}
					else if (M_field.getContent() == L"") {
						M_field.setSelected(true);
						N_field.setSelected(false);
					}
					else {
						M_field.setSelected(false);
						N_field.setSelected(true);
					}
				}
				if (takaisin.isInside(sf::Mouse::getPosition(window))) // user clicked "takaisin"
					return "main_menu";
			}
			/* user entered text */
			if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode >= 48 && event.text.unicode <= 57) { // check that input was numeral
					if (M_field.getSelected()) {
						if (event.text.unicode != 48 || M_field.getContent() != L"") // check that 0 is not the first digit
							M_field.setContent(M_field.getContent() + static_cast<wchar_t>(event.text.unicode));
					}
					else {
						if (event.text.unicode != 48 || N_field.getContent() != L"") // check that 0 is not the first digit
							N_field.setContent(N_field.getContent() + static_cast<wchar_t>(event.text.unicode));
					}
				}
			}
			/* user pressed backspace */
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && !backspaceUsed) {
				if (M_field.getSelected())
					M_field.setContent(M_field.getContent().substr(0, M_field.getContent().size() - 1));
				else
					N_field.setContent(N_field.getContent().substr(0, N_field.getContent().size() - 1));
				backspaceUsed = true; // ensure that every backspace stroke deletes only one digit
			}
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) // backspace is lifted
				backspaceUsed = false;
			/* user pressed enter */
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !returnUsed) { // pressing enter shifts focus to unfilled field, or changes screen
				if (M_field.getSelected()) {
					M_field.setSelected(false);
					N_field.setSelected(true);
				}
				if (N_field.getSelected() && N_field.getContent() != L"") { // move on to next screen
					if (M_field.getContent() == L"") {
						M_field.setSelected(true);
						N_field.setSelected(false);
					}
					else {
						// pass the dimensions and move on to input matrix screen
						x_dim = std::stoi(M_field.getContent());
						y_dim = std::stoi(N_field.getContent());
						return "input_matrix";
					}
				}
				returnUsed = true; // ensure that every enter stroke moves focus only once
			}
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) // backspace is lifted
				returnUsed = false;
		}

		/* actual rendering */
		window.clear(BACKGROUND_COLOR);
		M_field.draw(window);
		N_field.draw(window);
		jatka.draw(window);
		takaisin.draw(window);
		X.draw(window);
		title.draw(window);
		
		window.display();
	}

	return "size_query";
}

/****************************************************************************/
/* InputMatrix screen. Asks the user to fill the matrix */
std::string InputMatrix::Run(sf::RenderWindow &window)
{
	/* COMPONENTS INIT */
	TextFieldMatrix matrix(
		PADDING,
		PADDING,
		(unsigned int)(window.getSize().y - window.getSize().x / 20.0f),
		(unsigned int)(window.getSize().y - window.getSize().x / 20.0f),
		M,
		N
		);
	matrix.getTile(0).setSelected(true); // initially select the first tile
	ButtonTitleInside takaisin(
		(unsigned int)(window.getSize().x - window.getSize().x / 12.0f - PADDING),
		PADDING,
		(unsigned int)(window.getSize().x / 12.0f),
		(unsigned int)(window.getSize().x / 20.0f),
		"Takaisin"
		); // button for going back
	ButtonTitleInside ratkaise(
		(unsigned int)(window.getSize().x - window.getSize().x / 12.0f - PADDING),
		(unsigned int)(window.getSize().y - window.getSize().x / 20.0f - PADDING),
		(unsigned int)(window.getSize().x / 12.0f),
		(unsigned int)(window.getSize().x / 20.0f),
		"Ratkaise"
		); // button for solving the matrix
	TextLabel instructions(
		L"Täytä ruudukko kirjaimilla\nja paina ratkaise",
		FONT,
		window.getSize().y - window.getSize().x / 20.0f + 2 * PADDING,
		window.getSize().x / 20.0f + 2 * PADDING,
		(unsigned int)(window.getSize().x / 20.0f)
		); // instructions for filling the matrix
	instructions.fitLabelX(window.getSize().x - PADDING); // make the label fit nicely to screen

	/* COMPONENTS END INIT */

	bool backspaceUsed = true; // for ensuring that backspace is used for one digit at a time
	bool returnUsed = true; // for ensuring that enter is used for one change of focus at a time

	while (true) {

		sf::Event event;
		while (window.pollEvent(event)) {
			/* user closed the window */
			if (event.type == sf::Event::Closed)
				return "exit";
			/* user moved the mouse */
			if (event.type == sf::Event::MouseMoved) { // check if mouse hovers over any of the buttons
				takaisin.isInside(sf::Mouse::getPosition(window));
				ratkaise.isInside(sf::Mouse::getPosition(window));
			}
			/* user clicked */
			if (event.type == sf::Event::MouseButtonPressed) {
				if (takaisin.isInside(sf::Mouse::getPosition(window))) // user clicked "takaisin"
					return "size_query";
				if (matrix.isInside(sf::Mouse::getPosition(window)) > -1) { // user clicked one of the cells
					int index = matrix.isInside(sf::Mouse::getPosition(window));
					matrix.selectTile(index);
				}
				if (ratkaise.isInside(sf::Mouse::getPosition(window))) { // user clicked "ratkaise"
					if (!matrix.isMatrixFilled()) {
						if (matrix.getSelectedTile().getContent() != L"")
							matrix.selectNextEmpty();
					}
					else {
						// turn matrix to a string and move on to solve screen
						for (wchar_t &c : matrix_as_string) { // change every character in matrix_as_string to lower case
							c = tolower(c);
						}
						matrix_as_string = matrix.getAsString();
						return "solve_screen";
					}
				}
			}
			/* user entered text */
			if (event.type == sf::Event::TextEntered) { // user entered text
				if ((event.text.unicode > 32 && event.text.unicode < 127) || event.text.unicode > 160) { // check that input was a valid letter 
					matrix.getSelectedTile().setContent(L""); // clear content, so that content is only one char long at a time
					matrix.getSelectedTile().setContent(matrix.getSelectedTile().getContent() + static_cast<wchar_t>(event.text.unicode)); // set the content of cell to typed char
					matrix.selectNextEmpty(); // select next unfilled tile
				}
			}
			/* user pressed backspace */
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && !backspaceUsed) {
				matrix.getSelectedTile().setContent(L"");
				if (matrix.getSelectedTileIndex() > 0)
					matrix.selectTile(matrix.getSelectedTileIndex() - 1);
				backspaceUsed = true; // ensure that every stroke of backspace deletes only one digit
			}
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) // backspace is lifted
				backspaceUsed = false;

			/* user pressed enter */
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !returnUsed) { // pressing enter shifts focus to unfilled field, or changes screen
				if (matrix.isMatrixFilled()) {
					// turn matrix to a string and move on to solve screen
					matrix_as_string = matrix.getAsString();
					for (wchar_t &c : matrix_as_string) { // change every character in matrix_as_string to lower case
						c = tolower(c);
					}
					return "solve_screen";
				}
				matrix.selectNextEmpty();
				returnUsed = true; // ensure that every enter stroke moves focus only once
			}
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) // backspace is lifted
				returnUsed = false;
		}

		/* actual rendering */
		window.clear(BACKGROUND_COLOR);
		matrix.draw(window);
		takaisin.draw(window);
		ratkaise.draw(window);
		instructions.draw(window);

		window.display();
	}
	return "input_matrix";
}

/****************************************************************************/
std::vector<std::wstring> getSolvedWordsAsStrings(std::vector<Path> solved_words)
{
	std::vector<std::wstring> found_words;
	for (Path solved_word : solved_words) {
		found_words.push_back(solved_word.w_word());
	}
	return found_words;
}

std::vector<unsigned int> getPathAsIndices(Path path, unsigned int M) {
	std::vector<unsigned int> indices;
	std::vector<Tile> path_as_tiles = path.path();
	for (Tile tile : path_as_tiles) {
		unsigned int index = tile.x() + tile.y() * M;
		indices.push_back(index);
	}
	return indices;
}

/****************************************************************************/

std::string SolveScreen::Run(sf::RenderWindow &window) 
{
	/* COMPONENTS INIT */
	TextFieldMatrix matrix(
		PADDING,
		PADDING,
		(unsigned int)(window.getSize().y - window.getSize().x / 20.0f),
		(unsigned int)(window.getSize().y - window.getSize().x / 20.0f),
		M,
		N
		);
	TextFieldMatrix solved_list(
		(unsigned int)(window.getSize().y - window.getSize().x / 20.0f) + 2 * PADDING,
		(unsigned int)(window.getSize().x / 20.0f * 2) + 2 * PADDING + 2,
		(unsigned int)(window.getSize().x - (window.getSize().y - window.getSize().x / 20.0f)) - 3 * PADDING,
		(unsigned int)(window.getSize().y - 4 * window.getSize().x / 20.0f - 4 * PADDING),
		2,
		15
		);
	ButtonTitleInside takaisin(
		(unsigned int)(window.getSize().x - window.getSize().x / 12.0f - PADDING),
		PADDING,
		(unsigned int)(window.getSize().x / 12.0f),
		(unsigned int)(window.getSize().x / 20.0f),
		"Takaisin"
		); // button for going back
	ButtonTitleInside menu(
		(unsigned int)(window.getSize().x - window.getSize().x / 12.0f - PADDING),
		(unsigned int)(window.getSize().y - window.getSize().x / 20.0f - PADDING),
		(unsigned int)(window.getSize().x / 12.0f),
		(unsigned int)(window.getSize().x / 20.0f),
		"Main Menu"
		); // button for returning to main menu
	ButtonTitleInside roll(
		(unsigned int)(window.getSize().y - window.getSize().x / 20.0f) + window.getSize().x / 12.0f + 3 * PADDING,
		(unsigned int)(window.getSize().y - window.getSize().x / 20.0f - PADDING),
		(unsigned int)(window.getSize().x - window.getSize().x / 6.0f - 5 * PADDING - window.getSize().y + window.getSize().x / 20.0f),
		(unsigned int)(window.getSize().x / 20.0f),
		"Roll All"
		); // button for showing each solved word's path graphically, one at a time
	ButtonTitleInside android(
		(unsigned int)(window.getSize().y - window.getSize().x / 20.0f) + 2 * PADDING,
		(unsigned int)(window.getSize().y - window.getSize().x / 20.0f - PADDING),
		(unsigned int)(window.getSize().x / 12.0f),
		(unsigned int)(window.getSize().x / 20.0f),
		"Android"
		); // button for sending solutions to connected Android phone
	ButtonTitleInside up(
		(unsigned int)(window.getSize().y - window.getSize().x / 20.0f) + 2 * PADDING,
		(unsigned int)(window.getSize().x / 20.0f) + 2 * PADDING,
		(unsigned int)(window.getSize().x - (window.getSize().y - window.getSize().x / 20.0f)) - 3 * PADDING,
		(unsigned int)(window.getSize().x / 20.0f),
		"Edelliset"
		); // button for scrolling solved_list up
	ButtonTitleInside down(
		(unsigned int)(window.getSize().y - window.getSize().x / 20.0f) + 2 * PADDING,
		(unsigned int)(window.getSize().y - 2 * window.getSize().x / 20.0f - 2 * PADDING),
		(unsigned int)(window.getSize().x - (window.getSize().y - window.getSize().x / 20.0f)) - 3 * PADDING,
		(unsigned int)(window.getSize().x / 20.0f),
		"Seuraavat"
		); // button for scrolling solved_list down
	TextLabel highlighted(
		L"",
		FONT,
		window.getSize().y - window.getSize().x / 20.0f + 2 * PADDING,
		PADDING,
		(unsigned int)(window.getSize().x / 20.0f)
		);
	/* COMPONENTS END INIT */

	bool update_needed = true; // boolean which determines whether the view should be updated. This should improve fps.

	unsigned int page_count = 0; // found words are divided into X "pages". One page is shown at a time in the solved_list. This variable counts on which table we are on
	bool page_count_changed = true; // this variable determines whether the solved_list should be updated

	sf::Clock clock; // clock for roll all

	bool roll_all_on = false;
	unsigned int roll_index = 0; // current index of the word highlighted by roll all

	// solve the matrix
	sj::Solver solver(dictionary, matrix_as_string, M, N);
	solver.setAndroidSolverState(false);
	std::vector<Path> solved_words = solver.Paths();
	std::vector<std::wstring> solved_words_string = getSolvedWordsAsStrings(solved_words);
	unsigned int page_count_max = (unsigned int)ceil(solved_words_string.size() / (float)(2 * 15)); // how many pages there are

	// fill textfield matrix with correct letters
	for (unsigned int i = 0; i < matrix_as_string.length(); i++) {
		matrix.getTile(i).setContent(matrix_as_string.substr(i, 1));
	}

	while (true) {

		sf::Event event;

		while (window.pollEvent(event)) {
			/* user closed the window */
			if (event.type == sf::Event::Closed)
				return "exit";
			/* user moved the mouse */
			if (event.type == sf::Event::MouseMoved) { // check if mouse hovers over any of the buttons
				takaisin.isInside(sf::Mouse::getPosition(window));
				menu.isInside(sf::Mouse::getPosition(window));
				roll.isInside(sf::Mouse::getPosition(window));
				up.isInside(sf::Mouse::getPosition(window));
				down.isInside(sf::Mouse::getPosition(window));
				android.isInside(sf::Mouse::getPosition(window));
				update_needed = true;
			}
			/* user clicked */
			if (event.type == sf::Event::MouseButtonPressed) {
				if (takaisin.isInside(sf::Mouse::getPosition(window))) { // user clicked "takaisin"
					solver.setAndroidSolverState(false);
					return "input_matrix";
				}
				if (menu.isInside(sf::Mouse::getPosition(window))) { // user clicked "main menu"
					solver.setAndroidSolverState(false);
					return "main_menu";
				}
				if (android.isInside(sf::Mouse::getPosition(window))) { // user clicked "android"
					if (M == 4 && N == 4) { // check that matrix is 4x4
						if (solver.getAndroidSolverState()) {
							solver.setAndroidSolverState(false);
						}
						else {
							solver.setAndroidSolverState(true);
							std::pair<int,int> res = get_res(ocr_filename);
							std::thread thread(&Solver::Android_Solve, &solver, res.first, res.second, 3); // fixed screen resolution
							thread.detach();
						}
					}
				}
				if (roll.isInside(sf::Mouse::getPosition(window))) { // user clicked "roll all"
					roll_index = 0;
					roll_all_on = true;
					solved_list.getSelectedTile().setSelected(false);
					clock.restart();
				}
				if (up.isInside(sf::Mouse::getPosition(window))) { // user clicked "edelliset"
					if (page_count > 0) {
						page_count--;
						page_count_changed = true;
						solved_list.getSelectedTile().setSelected(false); // change selected word to not selected
						roll_all_on = false; // cancel roll all if solved word is clicked
						update_needed = true;
					}
				}
				if (down.isInside(sf::Mouse::getPosition(window))) { // user clicked "seuraavat"
					if (page_count < page_count_max - 1) {
						page_count++;
						page_count_changed = true;
						solved_list.getSelectedTile().setSelected(false); // change selected word to not selected
						roll_all_on = false; // cancel roll all if solved word is clicked
						update_needed = true;
					}
				}
				if (solved_list.isInside(sf::Mouse::getPosition(window)) > -1) { // user clicked one of the solved words
					int index = solved_list.isInside(sf::Mouse::getPosition(window));
					if (index + 2 * 15 * page_count < solved_words.size()) {
						for (unsigned int i = 0; i < M * N; i++) { // clear old path
							matrix.getTile(i).setInPath(false);
						}
						solved_list.selectTile(index); // set the clicked word as selected
						std::vector<unsigned int> indices = getPathAsIndices(solved_words[index + 2 * 15 * page_count], M);
						for (unsigned int i : indices) { // mark the path into matrix
							matrix.getTile(i).setInPath(true);
						}

						highlighted.setContent(solved_words_string[index + 2 * 15 * page_count]); // write the selected word next to the matrix
						highlighted.fitLabelX(window.getSize().x * 11.0f / 12.0f - 2 * PADDING); // fit the selected word to window
						roll_all_on = false; // cancel roll all if solved word is clicked
						update_needed = true;
					}
				}
			}
		}
		// set an appropriate title for the android solver -button
		if (solver.getAndroidSolverState())
			android.setTitle("Stop");
		else
			android.setTitle("Android");

		// fill solved list with found words
		if (page_count_changed) {
			for (unsigned int i = 2 * 15 * page_count; i < 2 * 15 * (page_count + 1); i++) {
				if (solved_words_string.size() > i)
					solved_list.getTile(i - 2 * 15 * page_count).setContent(solved_words_string[i]);
				else
					solved_list.getTile(i - 2 * 15 * page_count).setContent(L"");
			}
			page_count_changed = false;
		}

		if (roll_all_on) { // show every path one at a time
			if (clock.getElapsedTime().asSeconds() >= 1.0f) { // show next path is second has elapsed
				update_needed = true;
				if (roll_index < solved_words_string.size()) {
					for (unsigned int i = 0; i < M * N; i++) { // clear old path
						matrix.getTile(i).setInPath(false);
					}

					std::vector<unsigned int> indices = getPathAsIndices(solved_words[roll_index], M);
					for (unsigned int i : indices) { // mark the path into matrix
						matrix.getTile(i).setInPath(true);
					}

					highlighted.setContent(solved_words_string[roll_index]); // write the selected word next to the matrix
					highlighted.fitLabelX(window.getSize().x * 11.0f / 12.0f - 2 * PADDING); // fit the selected word to window

					roll_index++;
					clock.restart();
				}
				else {
					for (unsigned int i = 0; i < M * N; i++) { // clear old path
						matrix.getTile(i).setInPath(false);
					}
					highlighted.setContent(L""); // empty the highlighted string
					roll_all_on = false;
				}
			}
		}

		/* actual rendering */
		if (update_needed) {
			window.clear(BACKGROUND_COLOR);
			takaisin.draw(window);
			menu.draw(window);
			roll.draw(window);
			matrix.draw(window);
			solved_list.draw(window);
			up.draw(window);
			down.draw(window);
			highlighted.draw(window);
			android.draw(window);

			window.display();

			update_needed = false;
		}
	}
	return "solve_screen";
}
