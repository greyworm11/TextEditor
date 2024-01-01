#include "model.h"

Model::Model(View* view_)
{
	view = view_;

	Command nn(&(this->ff));
	this->cmd = nn;

	openFile(filename);
	setDownLine();
	updateMainWin();
	updateStatusBar();
}

void Model::setMode(int new_mode)
{
	current_mode = new_mode;
}

int Model::getMode()
{
	return current_mode;
}

void Model::goToStringView()
{
	view->clearStatusBar();
	x_status = 21 + string_num.size();
	view->setCoords(x_status, y_status);
	view->GoToString(string_num.data());
}

void Model::commandView()
{
	view->clearStatusBar();
	x_status = 47 + command.size();
	view->setCoords(x_status, y_status);
	view->showStatusBar(current_mode, current_x, current_y, command.data());
}

void Model::goFindView(int symbol)
{
	view->clearStatusBar();
	x_status = 19 + finding_str.size();
	view->setCoords(x_status, y_status);
	view->find(symbol, finding_str.data());
}

void Model::updateAll()
{
	updateMainWin();
	view->setCoords(x_scr, y_scr);
	updateStatusBar();
	updateColumnWindow(); ///
}

void Model::updateColumnWindow()
{
	view->clearColumnBar();

	if (getCommand().comd->get())
	{
		int current_numbers_column[HEIGHT_WINDOW_COLUMN] = { 0 };
		int copy_y_scr = y_scr;

		for (int i = 0; i < HEIGHT_WINDOW_COLUMN; ++i)
		{
			if (copy_y_scr != 0)
			{
				current_numbers_column[i] = current_y + 1 - copy_y_scr;
				--copy_y_scr;
			}
			else {
				current_numbers_column[i] = current_y + 1 + i - y_scr;
			}
		}
		view->showColumns(current_numbers_column);
	}
}


int Model::countLinesInString(int number_line)
{
	// new
	if (number_line < 0) return 0;
	// end
	if (text[number_line].size() % WIDTH_WINDOW == 0 && text[number_line].size() != 0)
		return text[number_line].size() / WIDTH_WINDOW - 1;
	else
		return text[number_line].size() / WIDTH_WINDOW;
}

void Model::setDownLine()
{
	int size_vector = text.size();
	for (int current_line = up_line, count_str = 0;; count_str++, current_line++)
	{
		if (current_line >= size_vector)
		{
			down_line = current_line - 1;
			part_down_line = countLinesInString(current_line - 1);
			break;
		}
		count_str += countLinesInString(current_line);
		if (count_str == HEIGHT_WINDOW_MAIN - 1)
		{
			down_line = current_line;
			part_down_line = countLinesInString(current_line);
			break;
		}
		if (count_str > HEIGHT_WINDOW_MAIN - 1)
		{
			down_line = current_line;
			part_down_line = countLinesInString(current_line);
			while (count_str != HEIGHT_WINDOW_MAIN - 1)
			{
				part_down_line--;
				count_str--;
			}
			break;
		}
	}
}

void Model::setUpLine()
{
	int size_vector = text.size();
	if (size_vector < HEIGHT_WINDOW_MAIN)
	{
		up_line = 0;
		part_up_line = 0;
		return;
	}
	for (int count = 1, now_line = size_vector - 1;; count++, now_line--)
	{
		if (!(text[now_line].size() / WIDTH_WINDOW == 1 && text[now_line].size() == WIDTH_WINDOW))
			count += text[now_line].size() / WIDTH_WINDOW;
		if (count == HEIGHT_WINDOW_MAIN)
		{
			up_line = now_line;
			part_up_line = 0;
			break;
		}
		if (count > HEIGHT_WINDOW_MAIN)
		{
			up_line = now_line;
			part_up_line = count - HEIGHT_WINDOW_MAIN;
			break;
		}
	}
}

void Model::openFile(MyString file_name)
{
	MyString str;
	ifstream file;
	char ch;
	int pos = 0;
	int number = 0;

	file.open(file_name.data());
	if (file.is_open())
	{
		while (file.get(ch)) {

			if (pos == 0) {
				text.push_back("");
				pos++;
			}

			if (ch == '\n') {
				number++;
				pos = 0;
				continue;
			}

			text[number].append(1, ch);
		}
		file.close();
	}
	else
	{
		exit(-1);
	}
}

void Model::updateStatusBar()
{
	view->clearStatusBar();
	view->showStatusBar(current_mode, current_x, current_y, command.c_str());
}

void Model::updateMainWin()
{
	view->clearMainBar();
	view->showText(text, up_line, part_up_line);
}

void Model::nextDownLine()
{
	if (part_down_line < countLinesInString(down_line))
		part_down_line++;
	else
	{
		down_line++;
		part_down_line = 0;
	}
}

void Model::nextUpLine()
{
	if (part_up_line < countLinesInString(up_line))
		part_up_line++;
	else
	{
		up_line++;
		part_up_line = 0;
	}
}

void Model::previousDownLine()
{
	if (part_down_line > 0)
		part_down_line--;
	else
	{
		down_line--;
		part_down_line = countLinesInString(down_line);
	}
}

void Model::previousUpLine()
{
	if (part_up_line > 0)
		part_up_line--;
	else
	{
		up_line--;
		part_up_line = countLinesInString(up_line);
	}
}

void Model::getTrueColsRowsUP()
{
	y_scr--;
	if (y_scr == -1)
	{
		y_scr++;
		previousDownLine();
		previousUpLine();
	}
	if (part_current_line == 0)		// In the top string of the line, goto next
	{
		current_y--;
		if (countLinesInString(current_y) == 0)		// If new line is 1 string in screen
		{
			part_current_line = 0;
			current_x = text[current_y].size() > x_scr ? x_scr : x_scr = text[current_y].size();  // deal with cursor pos
		}
		else
		{											// If new line includes >= 2 strings in screen
			part_current_line = countLinesInString(current_y);
			if (text[current_y].size() - countLinesInString(current_y) * WIDTH_WINDOW >= x_scr)	// x coord fits in size of last str
				current_x = part_current_line * WIDTH_WINDOW + x_scr;
			else
			{												// Does not fit -> move to the end
				current_x = text[current_y].size();
				x_scr = text[current_y].size() - countLinesInString(current_y) * WIDTH_WINDOW;
			}
		}
	}
	else
	{		// in the middle of str -> make coords offset
		part_current_line--;
		current_x -= WIDTH_WINDOW;
	}
}

void Model::getTrueColsRowsDOWN()
{
	y_scr++;
	if (y_scr == HEIGHT_WINDOW_MAIN)	// User pressed goDownside while being in the bot
	{
		y_scr--;
		nextDownLine();
		nextUpLine();
	}
	if (countLinesInString(current_y) == 0)		// If new line is 1 string in screen
	{
		current_y++;								// goto new str
		part_current_line = 0;						// begin with the first part
		current_x = text[current_y].size() > x_scr ? x_scr : x_scr = text[current_y].size(); // If str size is not bigger than the current x_scr, move to the same place ; Case bigger - move to the end
	}
	else
	{												// If new line includes >= 2 strings in screen
		if (part_current_line == countLinesInString(current_y))	//   
		{
			current_y++;										// goto new str
			part_current_line = 0;								// begin with the first part
			current_x = text[current_y].size() > x_scr ? x_scr : x_scr = text[current_y].size();
		}
		else
		{
			if (part_current_line == countLinesInString(current_y) - 1)	// To move the cursor correctly, offset
			{
				part_current_line++;
				if (text[current_y].size() - countLinesInString(current_y) * WIDTH_WINDOW >= x_scr)	// Check if we get into a proper place, this case we don't move to the end
					current_x += WIDTH_WINDOW;
				else
				{																	// count new coords or move to the end
					current_x = text[current_y].size();
					x_scr = text[current_y].size() - countLinesInString(current_y) * WIDTH_WINDOW;
				}
			}
			else
			{																		// In the middle of a new str
				current_x += WIDTH_WINDOW;
				part_current_line++;
			}
		}
	}
}

void Model::getTrueColsRowsLeft()
{
	x_scr--;
	if (x_scr == -1 && current_y == up_line && part_current_line == part_up_line)		// For new str
	{
		y_scr++;
		previousDownLine();
		previousUpLine();
	}
	if (x_scr >= 0)	// Check if the most left pos
		current_x -= 1;
	else
	{
		y_scr--;					// move upside by 1 coord
		if (part_current_line > 0)		// case several str in line
		{
			part_current_line--;
			current_x -= 1;
			x_scr = WIDTH_WINDOW - 1;
		}
		else
		{
			current_y--;
			part_current_line = countLinesInString(current_y);
			if (text[current_y].size() <= WIDTH_WINDOW)	// check if only one str inside
				current_x = x_scr = text[current_y].size();	// move cursor to the end
			else
			{
				current_x = text[current_y].size();
				x_scr = text[current_y].size() % WIDTH_WINDOW;
				if (text[current_y].size() % WIDTH_WINDOW == 0)		// Case line fills up all window
					x_scr = WIDTH_WINDOW;
			}
		}
	}
}

void Model::getTrueColsRowsRight()
{
	x_scr++;
	if (part_current_line == part_down_line && down_line == current_y)		//,        
	{
		if ((part_current_line != countLinesInString(current_y) && x_scr == WIDTH_WINDOW) ||
			(part_current_line == countLinesInString(current_y) && x_scr == text[current_y].size() + 1 - WIDTH_WINDOW * countLinesInString(current_y)))
		{
			y_scr--;
			nextDownLine();
			nextUpLine();
		}
	}
	if (countLinesInString(current_y) == 0)		//  
	{
		if (text[current_y].size() >= x_scr)
			current_x++;
		else
		{
			x_scr = 0;
			current_x = 0;
			current_y++;
			y_scr++;
		}
	}
	else
	{													//  
		if (part_current_line == countLinesInString(current_y))	//   
		{
			if (text[current_y].size() - WIDTH_WINDOW * countLinesInString(current_y) >= x_scr)	//   
				current_x++;
			else
			{
				part_current_line = 0;
				x_scr = 0;
				current_x = 0;
				current_y++;
				y_scr++;
			}
		}
		else
		{
			if (WIDTH_WINDOW > x_scr)
				current_x++;
			else
			{
				current_x++;
				part_current_line++;
				x_scr = 0;
				y_scr++;
			}
		}
	}
}

void Model::keyPageUp()
{
	x_scr = 0;
	y_scr = 0;
	for (int i = 0; i < HEIGHT_WINDOW_MAIN; i++)
	{
		if (up_line == 0 && part_up_line == 0)
			break;
		previousDownLine();
		previousUpLine();
	}
	current_y = up_line;
	part_current_line = part_up_line;
	current_x = part_current_line * WIDTH_WINDOW;
	updateAll();
}

void Model::keyPageDown()
{
	x_scr = 0;
	y_scr = 0;
	for (int i = 0; i < HEIGHT_WINDOW_MAIN; i++)
	{
		if (down_line == text.size() - 1 && part_down_line == text[down_line].size() / WIDTH_WINDOW)
			break;
		nextDownLine();
		nextUpLine();
	}
	current_y = up_line;
	part_current_line = part_up_line;
	current_x = part_current_line * WIDTH_WINDOW;
	updateAll();
}

void Model::KeyUp()
{
	if (y_scr == 0 && current_y == 0 && part_current_line == 0)			//The very top of the text
	{
		return;
	}
	getTrueColsRowsUP();
	updateAll();
}

void Model::KeyDown()
{
	if (down_line == text.size() - 1 && part_down_line == part_current_line && current_y == down_line)		//If the last line and the last part of it
	{
		return;
	}
	getTrueColsRowsDOWN();
	updateAll();
}

void Model::KeyLeft()
{
	if (current_y == 0 && part_current_line == 0 && x_scr == 0)			//The leftmost part
	{
		return;
	}
	getTrueColsRowsLeft();
	updateAll();
}

void Model::KeyRight()
{
	if (current_y == text.size() - 1 && current_x == text[current_y].size())			//The rightmost part
	{
		return;
	}
	getTrueColsRowsRight();
	updateAll();
}

void Model::goToTheFirstPage()
{
	part_down_line = down_line = 0;
	part_up_line = up_line = 0;
	x_scr = y_scr = current_x = current_y = part_current_line = 0;
	setDownLine();
	updateAll();
}

void Model::goToTheLastPage()
{
	down_line = text.size() - 1;
	part_down_line = countLinesInString(down_line);
	setUpLine();
	x_scr = y_scr = 0;
	current_y = up_line;
	part_current_line = part_up_line;
	current_x = part_current_line * WIDTH_WINDOW;
	updateAll();
}

void Model::replaceCurrentSymbol(int symbol)
{
	if (current_x == text[current_y].size())
		return;
	unsigned char ch = symbol;
	MyString tmp(1, ch);
	text[current_y].replace(current_x, 1, tmp.data());
}

void Model::cleanCurrentString()
{
	if (text[current_y].size() == 0)
		return;
	this->goToStartCurrentString();
	text[current_y] = "";
	setDownLine();
	updateAll();
}

void Model::goToStartCurrentString()
{
	if (up_line != current_y)		//If the row is not the top one
	{
		y_scr -= part_current_line;
		current_x = 0;
		part_current_line = 0;
		x_scr = 0;
	}
	if (up_line == current_y)		//If the top row
	{
		if (part_up_line == 0)		//If the entire
		{
			y_scr -= part_current_line;
			current_x = 0;
			part_current_line = 0;
			x_scr = 0;
		}
		else
		{							//If not all of it is displayed, then lift the cursor up part_current_line once
			for (int i = part_current_line; i != 0; i--)
			{
				this->getTrueColsRowsUP();
			}
			current_x = 0;
			x_scr = 0;
		}
	}
	updateAll();
}


void Model::goToEndCurrentString()
{
	if (current_y != down_line)
	{
		y_scr += (countLinesInString(current_y) - part_current_line);
		x_scr = text[current_y].size() - countLinesInString(current_y) * WIDTH_WINDOW;
		current_x = text[current_y].size();
		part_current_line = countLinesInString(current_y);
	}
	if (current_y == down_line)		//if the bottom line
	{
		if (part_down_line == countLinesInString(current_y))		//If the entire string
		{
			y_scr += (countLinesInString(current_y) - part_current_line);
			x_scr = text[current_y].size() - countLinesInString(current_y) * WIDTH_WINDOW;
			current_x = text[current_y].size();
			part_current_line = countLinesInString(current_y);
		}
		else
		{							//Not entire
			for (int i = countLinesInString(current_y) - part_current_line; i != 0; i--)
			{
				this->getTrueColsRowsDOWN();
			}
			current_x = text[current_y].size();
			x_scr = text[current_y].size() - countLinesInString(current_y) * WIDTH_WINDOW;
		}
	}
	updateAll();
}

int Model::checkWordRightSide(int line, int pos)
{
	for (int i = pos; i < text[line].size(); i++)
	{
		if (*(text[line].data() + i) != ' ')
			return 1;
	}
	return 0;
}

int Model::checkWordLeftSide(int line, int pos)
{
	if (pos - 1 < 0)
		return 0;
	for (int i = pos - 1; i >= 0; i--)
	{
		if (*(text[line].data() + i) != ' ')
			return 1;
	}
	return 0;
}

void Model::goToStartWord()
{
	if (current_y != 0)
		while (text[current_y - 1].size() == 0 && checkWordLeftSide(current_y, current_x) == 0)
		{
			this->KeyUp();
			if (text[current_y].size() == 0)
				this->KeyLeft();
		}
	int flag = 0;	//flag at the beginning of the word
	int index = 0;
	if (current_x == 0)
	{
		if (*(text[current_y].data() + current_x) != ' ')	//check for the beginning of the word
		{
			flag = 1;
		}
	}
	else
	{
		if (*(text[current_y].data() + current_x - 1) == ' ')	//check for the beginning of the word
		{
			flag = 1;
		}
	}
	if (flag)
	{
		this->KeyLeft();
	}
	while (*(text[current_y].data() + current_x) == ' ')
	{
		this->KeyLeft();
	}
	while (*(text[current_y].data() + current_x) != ' ')
	{
		if (current_x == 0)
			return;
		this->KeyLeft();
	}
	this->KeyRight();
}

void Model::goToEndWord()
{

	if (checkWordRightSide(current_y, current_x) == 0 && text.size() - 1 != current_y)	//check for the presence of words on the right
	{
		while (current_x != text[current_y].size())
			this->KeyRight();
		this->KeyRight();
	}
	while (text[current_y].size() == 0)
	{
		this->KeyDown();
	}
	int flag = 0;	//flag at the end of the word
	int index = 0;
	if (*(text[current_y].data() + current_x) == ' ')	//check for the end of the word
	{
		flag = 1;
	}
	if (flag)
	{
		this->KeyRight();
		while (*(text[current_y].data() + current_x + 1) == ' ')
		{
			this->KeyRight();
		}
	}
	index = text[current_y].find(" ", current_x + 1);
	if (index != -1)
	{
		while (index != current_x)
		{
			this->KeyRight();
		}
	}
	else
	{
		if (flag == 1)
			this->KeyLeft();
		if (checkWordRightSide(current_y, current_x))
		{
			while (current_x != text[current_y].size())
				this->KeyRight();
		}
	}
}

void Model::deleteCurrentSymbol()
{
	int size = countLinesInString(current_y);
	if (current_x != text[current_y].size())
	{
		text[current_y].erase(current_x, 1);
		if (countLinesInString(current_y) != size)
		{
			setDownLine();
		}
	}

}

void Model::copyString()
{
	if (text[current_y].size() != 0)
	{
		copiedStr = text[current_y].data();
	}
	else
	{
		copiedStr = "";
	}
}

void Model::cutCurrentStr()
{
	this->copyString();
	goToStartCurrentString();
	if (current_y != 0)
	{
		KeyUp();
		text.erase(text.begin() + current_y + 1);
		setDownLine();
	}
	else
	{
		text.erase(text.begin());
		setDownLine();
	}
}

void Model::insertString()
{
	if (copiedStr.size() == 0)
		return;
	text[current_y].insert(current_x, copiedStr.data());
	setDownLine();
}

void Model::copyWord()
{
	if (text[current_y].size() == current_x)
		return;
	if (*(text[current_y].data() + current_x) == ' ')
		return;
	copiedStr = "";
	for (int x = current_x; x >= 0; x--)
	{
		if (*(text[current_y].data() + x) == ' ')
		{
			x++;
			for (int i = x;; i++)
			{
				if (*(text[current_y].data() + i) == ' ')
					break;
				if (x == text[current_y].size())
					break;
				copiedStr.append(1, *(text[current_y].data() + i));
			}
			break;
		}
		if (x == 0)
		{
			char tmp;
			for (int i = 0;; i++)
			{
				if (*(text[current_y].data() + i) == ' ')
					break;
				if (i == text[current_y].size())
					break;
				tmp = *(text[current_y].data() + i);
				copiedStr.append(1, tmp);
			}
			break;
		}
	}
}

void Model::deleteCurrentWord()
{
	if (text[current_y].size() == current_x)
		return;
	if (*(text[current_y].data() + current_x) == ' ')
		return;
	if (text[current_y].find(" ") == -1)
	{
		cutCurrentStr();
		return;
	}
	for (int x = current_x; x >= 0; x--)
	{
		KeyLeft();
		if (*(text[current_y].data() + x) == ' ')
		{
			x++;
			KeyRight();
			for (int i = x;;)
			{
				if (*(text[current_y].data() + i) == ' ')
				{
					text[current_y].erase(i, 1);
					break;
				}
				if (i == text[current_y].size())
					break;
				text[current_y].erase(i, 1);
			}
			break;
		}
		if (x == 0)
		{
			char tmp;
			for (int j = 0;;)
			{
				if (*(text[current_y].data() + j) == ' ')
				{
					text[current_y].erase(j, 1);
					break;
				}
				if (j == text[current_y].size())
					break;
				text[current_y].erase(j, 1);
			}
			break;
		}
	}
}

void Model::goToStringNum(int symbol)
{
	MyString tmp(1, symbol);
	int number;
	if (symbol == KEY_CODE_BACKSPACE && string_num.size() > 0)
	{
		string_num.erase(string_num.size() - 1, 1);
		goToStringView();
		return;
	}

	if (symbol - 48 >= 0 && symbol - 48 <= 9)
	{
		string_num += tmp;
	}
	else if (symbol == 'G')
	{
		number = atoi(string_num.data());
		if (number >= text.size())
			return;
		up_line = current_y = number;
		current_x = 0;
		x_scr = y_scr = 0;
		part_up_line = part_current_line = 0;
		setDownLine();
	}
	else return;

	goToStringView();
}

void Model::Enter()
{
	if (current_x == 0)		//Adding a new line
	{
		text.insert(text.begin() + current_y, MyString());
		setDownLine();
		this->KeyDown();
		updateAll();
		return;
	}
	if (current_x == text[current_y].size())
	{
		text.insert(text.begin() + current_y + 1, MyString());
		setDownLine();
		this->KeyDown();
		updateAll();
		return;
	}
	MyString tmp(text[current_y].data() + current_x);
	text[current_y].erase(current_x, text[current_y].size() - current_x);
	text.insert(text.begin() + current_y + 1, tmp);
	setDownLine();
	this->KeyDown();
	x_scr = current_x = 0;
	updateAll();
}

void Model::Backspace()
{
	if (current_y == 0 && current_x == 0)	//At the very beginning
		return;
	if (current_x == text[current_y].size() && current_x - countLinesInString(current_y) * WIDTH_WINDOW == 0 && current_x != 0)
	{
		text[current_y].erase(current_x - 1, 1);
		this->KeyLeft();
		this->KeyLeft();
		setDownLine();
		return;
	}
	if (text[current_y].size() > 0 && current_x != 0)		//simple character removal
	{
		text[current_y].erase(current_x - 1, 1);
		this->KeyLeft();
		setDownLine();
		return;

	}
	if (current_x == 0 && current_y != 0)	//combining strings
	{
		this->KeyLeft();
		if (text[current_y + 1].size() != 0)
			text[current_y] += text[current_y + 1];
		text.erase(text.begin() + current_y + 1);
		setDownLine();
		updateAll();
		return;
	}
}


void Model::insertSymbol(int symbol)
{

	text[current_y].insert(current_x, 1, (char)symbol);
	this->KeyRight();
	if (text[current_y].size() % WIDTH_WINDOW == 1)	//when switching to the next line
	{
		previousDownLine();
		if (x_scr == 0)		//a controversial condition, but it seems to be normal with it
			x_scr++;
	}
	updateAll();
}

void Model::readCommand(int symbol)
{
	MyString tmp(1, symbol);
	int number;
	if (symbol == KEY_CODE_BACKSPACE && command.size() > 0)
	{
		command.erase(command.size() - 1, 1);
		commandView();
		return;
	}
	if (symbol == KEY_CODE_BACKSPACE && command.size() == 0)
	{
		return;
	}
	switch (symbol)
	{
	case KEY_CODE_ENTER:
		parseCommand();
		break;
	default:
		command += tmp;
		break;
	}
	commandView();
}

void Model::parseCommand()
{
	MyString tmp;

	if (command.size() > 2)
	{
		if (command[0] == 'o')
		{
			tmp = command.substr(2);
			string_num = "";
			command = "";
			finding_str = "";
			part_down_line = down_line = 0;
			part_up_line = up_line = 0;
			x_scr = y_scr = current_x = current_y = part_current_line = 0;
			current_mode = COMMAND;
			filename = tmp;
			text.clear();
			text.shrink_to_fit();
			view->clearStatusBar();
			view->clearMainBar();
			openFile(filename);
			setDownLine();
			updateMainWin();
			updateStatusBar();
			return;
		}
		if (command[0] == 'w')
		{
			tmp = command.substr(2);
			writeToFile(tmp);
			return;
		}
	}

	if (command[0] == 'x')
	{
		writeToFile(filename);
		cleanAll();
		openFile("example.txt");
		return;
	}
	else if (command[0] == 'w')
	{
		writeToFile(filename);
		return;
	}
	else if (command[0] == 'q')
	{
		if (command[1] != '!')
			writeToFile(filename);
		endwin();
		exit(0);
	}
	else if (command[0] == 'h')
	{
		showHelp();
		return;
	}
	else if (command[0] - 48 >= 0 && command[0] - 48 <= 9)
	{
		int i = 0;
		while (command[i] - 48 >= 0 && command[i] - 48 <= 9) ++i;
		command.c_str()[i] = '\0';
		int number = atoi(command.c_str());
		if (number >= text.size())
			return;
		up_line = current_y = number;
		current_x = 0;
		x_scr = y_scr = 0;
		part_up_line = part_current_line = 0;
		setDownLine();
		updateMainWin();
		return;
	}
	else if (command.length() == 7) // set num command
	{
		if (command[0] == 's' && command[1] == 'e' && command[2] == 't' && command[3] == ' ' &&
			command[4] == 'n' && command[5] == 'u' && command[6] == 'm')
		{
			// enable line numbering here
			if (getCommand().comd->get())
				cmd.comd->Undo();
			else
				cmd.comd->Execute();
			updateAll();
			return;
		}
	}
}

void Model::showHelp()
{
	int ch;
	view->clearStatusBar();
	view->showHelpInfo();
	ch = getch();
}

void Model::writeToFile(MyString current_filename)
{
	ofstream file;
	file.open(current_filename.data(), ios_base::trunc);
	if (file.is_open())
	{
		for (int i = 0; i < text.size(); i++)
		{
			if (text[i].data() != nullptr)
				file << text[i].data() << endl;
			else
				file << endl;
		}
		file.close();
	}
	else return;
}

void Model::cleanAll()
{
	clean_all = true;
	string_num = "";
	command = "";
	finding_str = "";
	part_down_line = down_line = 0;
	part_up_line = up_line = 0;
	x_scr = y_scr = current_x = current_y = part_current_line = 0;
	current_mode = NAVIGATION;
	filename = "example.txt";
	text.clear();
	text.shrink_to_fit();
	view->clearStatusBar();
	view->clearMainBar();
	view->clearColumnBar();//
}

void Model::findTextAfter(int symbol)
{
	if (pressedEnter == true)
	{
		switch (symbol)
		{
		case (int)'n':
			nextSearch();
			break;
		case (int)'N':
			previousSearch();
			break;
		case KEY_CODE_ESC:
			pressedEnter = false;
			break;
		default:
			break;
		}
		goFindView('/');
		updateMainWin();
		view->setCoords(x_scr, y_scr);
		updateColumnWindow(); //
		return;
	}
	MyString tmp(1, symbol);
	int number;
	if (symbol == KEY_CODE_BACKSPACE && finding_str.size() > 0)
	{
		finding_str.erase(finding_str.size() - 1, 1);
		goFindView('/');
		return;
	}
	if (symbol == KEY_CODE_BACKSPACE && finding_str.size() == 0)
	{
		return;
	}
	switch (symbol)
	{
	case KEY_CODE_ENTER:
		pressedEnter = true;
		nextSearch();
		goFindView('/');
		updateMainWin();
		view->setCoords(x_scr, y_scr);
		updateColumnWindow(); //

		break;
	case KEY_CODE_ESC:
		pressedEnter = false;
		break;
	default:
		finding_str += tmp;
		break;
	}
	goFindView('/');
}

void Model::findTextBefore(int symbol)
{
	if (pressedEnter == true)
	{
		switch (symbol)
		{
		case 'n':
			nextSearch();
			break;
		case 'N':
			previousSearch();
			break;
		case KEY_CODE_ESC:
			pressedEnter = false;
			break;
		default:
			break;
		}
		goFindView('?');
		updateMainWin();
		view->setCoords(x_scr, y_scr);
		updateColumnWindow(); //
		return;
	}
	MyString tmp(1, symbol);
	int number;
	if (symbol == KEY_CODE_BACKSPACE && finding_str.size() > 0)
	{
		finding_str.erase(finding_str.size() - 1, 1);
		goFindView('?');
		return;
	}
	if (symbol == KEY_CODE_BACKSPACE && finding_str.size() == 0)
	{
		return;
	}
	switch (symbol)
	{
	case KEY_CODE_ENTER:
		pressedEnter = true;
		previousSearch();
		goFindView('?');
		updateMainWin();
		view->setCoords(x_scr, y_scr);
		updateColumnWindow();//
		break;
	case KEY_CODE_ESC:
		pressedEnter = false;
		break;
	default:
		finding_str += tmp;
		break;
	}
	goFindView('?');
}

void Model::nextSearch()
{
	int cur_str, cur_sym;
	for (cur_str = current_y, cur_sym = current_x + 1;;)
	{
		cur_sym = text[cur_str].find(finding_str.data(), cur_sym);
		if (cur_sym == -1)
		{
			cur_str++;
			cur_sym = 0;
			if (cur_str == text.size())
				break;
		}
		else
		{
			while (1)
			{
				getTrueColsRowsRight();
				if (current_y == cur_str && current_x == cur_sym)
				{
					break;
				}
			}
			break;
		}
	}
}

void Model::previousSearch()
{
	int cur_str, cur_sym;
	cur_str = current_y, cur_sym = current_x - 1;
	while (cur_sym == -1)
	{
		cur_str--;
		if (cur_str < 0)
			return;
		else
		{
			cur_sym = text[cur_str].size() - 1;
		}
	}
	while (1)
	{
		if (cur_str < 0) break;

		if (text[cur_str].data() == nullptr)
		{
			cur_str--;
			cur_sym = text[cur_str].size() - 1;
			continue;
		}

		if (*(text[cur_str].data() + cur_sym) == finding_str[0])
		{
			if (cur_sym == text[cur_str].find(finding_str.data(), cur_sym))
			{
				while (1)
				{
					getTrueColsRowsLeft();
					if (current_y == cur_str && current_x == cur_sym)
					{
						break;
					}
				}
				break;
			}
			else
			{
				cur_sym--;
				if (cur_sym == -1)
				{
					cur_str--;
					if (cur_str < 0)
						break;
					else
					{
						cur_sym = text[cur_str].size() - 1;
					}
				}
			}
		}
		else
		{
			cur_sym--;
			while (cur_sym == -1)
			{
				cur_str--;
				if (cur_str < 0)
					break;
				else
				{
					cur_sym = text[cur_str].size() - 1;
				}
			}
		}
	}
}