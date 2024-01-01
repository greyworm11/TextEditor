#include "view.h"

View::View()
{
	wrapper.cursInit();

	wrapper.main_win = wrapper.cursNewWin(HEIGHT_WINDOW_MAIN, WIDTH_WINDOW, BEGIN_WINDOW_Y, BEGIN_WINDOW_X);
	wrapper.info_bar_win = wrapper.cursNewWin(HEIGHT_WINDOW_BAR, WIDTH_WINDOW, HEIGHT_WINDOW_MAIN, BEGIN_WINDOW_Y);
	wrapper.column_win = wrapper.cursNewWin(HEIGHT_WINDOW_COLUMN, WIDTH_WINDOW_COLUMN + 5, BEGIN_WINDOW_Y, BEGIN_WINDOW_COLUMN_X - 3);


	wrapper.cursRefresh();
	wrapper.cursBox(wrapper.info_bar_win, 0, 0);
	//wrapper.cursBox(wrapper.column_win, 0, 0); // enable string numbering box
	wrapper.cursWRefresh(wrapper.info_bar_win);
	wrapper.cursWRefresh(wrapper.main_win);
	wrapper.cursWRefresh(wrapper.column_win); //
}

int View::countLinesInString(int number_line, vector <MyString> text)
{
	if (text[number_line].size() % WIDTH_WINDOW == 0 && text[number_line].size() != 0)
		return text[number_line].size() / WIDTH_WINDOW - 1;
	else
		return text[number_line].size() / WIDTH_WINDOW;
}

void View::showText(vector<MyString> text, int start_line, int part_up_line)
{
	int i = start_line, number_str = 0;
	if (text[i].data() != nullptr)
	{
		MyString first(text[i].data());
		MyString tmp;
		if (part_up_line != 0)
		{
			for (int j = part_up_line; j <= countLinesInString(i, text); ++j)
			{
				tmp = first.substr(j * WIDTH_WINDOW);
				if (tmp.size() > WIDTH_WINDOW)
				{
					tmp[WIDTH_WINDOW] = '\0';
				}
				else
				{
					tmp[tmp.size()] = '\0';
				}
				wrapper.cursPrintLine(wrapper.main_win, number_str, tmp.data());
				number_str++;
				if (number_str == HEIGHT_WINDOW_MAIN)
				{
					wrapper.cursWRefresh(wrapper.main_win);
					return;
				}
			}
			i++;
		}
	}
	else
	{
		number_str++;
		i++;
	}

	MyString next;
	MyString tmp1;

	for (; i < text.size(); i++)
	{
		if (text[i].data() == nullptr)
		{
			number_str++;
			if (number_str == HEIGHT_WINDOW_MAIN)
			{
				wrapper.cursWRefresh(wrapper.main_win);
				return;
			}
			continue;
		}
		next = text[i];
		for (int j = 0; j <= countLinesInString(i, text); ++j)
		{
			tmp1 = next.substr(j * WIDTH_WINDOW);
			if (tmp1.size() > WIDTH_WINDOW)
			{
				tmp1[WIDTH_WINDOW] = '\0';
			}
			else
			{
				tmp1[tmp1.size()] = '\0';
			}
			wrapper.cursPrintLine(wrapper.main_win, number_str, tmp1.data());
			number_str++;
			if (number_str == HEIGHT_WINDOW_MAIN)
			{
				wrapper.cursWRefresh(wrapper.main_win);
				return;
			}

		}
	}
}

void View::GoToString(char* str)
{
	if (str == nullptr)
	{
		MyString line("Move to string num: ");
		wrapper.cursPrintInfoBar(wrapper.info_bar_win, 1, line.data());
		wrapper.cursWRefresh(wrapper.info_bar_win);
		return;
	}
	MyString line("Move to string num: ");
	line += str;
	wrapper.cursPrintInfoBar(wrapper.info_bar_win, 1, line.data());
	wrapper.cursWRefresh(wrapper.info_bar_win);
}

void View::find(char symbol, char* str)
{
	MyString line("Find mode ");
	switch (symbol)
	{
	case '/':
		line += "after:  ";
		break;
	case '?':
		line += "before: ";
		break;
	}
	if (str == nullptr)
	{
		wrapper.cursPrintInfoBar(wrapper.info_bar_win, 1, line.data());
		wrapper.cursWRefresh(wrapper.info_bar_win);
		return;
	}
	line += str;
	wrapper.cursPrintInfoBar(wrapper.info_bar_win, 1, line.data());
	wrapper.cursWRefresh(wrapper.info_bar_win);
}

void View::showStatusBar(int mode, int x, int y, char* str)
{
	MyString line;
	MyString coordinate_x(x + 1), coordinate_y(y + 1);

	line += "MOD: ";

	switch (mode)
	{
	case NAVIGATION:
		line += "NAVIGATION, ";
		break;
	case WRITE:
		line += "WRITE, ";
		break;
	case FIND:
		line += "FIND, ";
		break;
	case COMMAND:
		line += "COMMAND, ";
		break;
	}

	line += "LINE: ";
	line.append(coordinate_y.data());
	line += ", POS: ";
	line.append(coordinate_x.data());
	line += "        ";

	if (mode == COMMAND)
	{
		line += "Command: ";
		if (str != NULL)
			line += str;
	}

	wrapper.cursPrintInfoBar(wrapper.info_bar_win, 1, line.data());
	wrapper.cursWRefresh(wrapper.info_bar_win);
}

void View::setCoords(int x, int y)
{
	wrapper.cursMove(y, x);
	wrapper.cursRefresh();
	wrapper.cursWRefresh(wrapper.info_bar_win);
	wrapper.cursWRefresh(wrapper.main_win);
	wrapper.cursWRefresh(wrapper.column_win);
}

void View::clearMainBar()
{
	wrapper.cursClear(wrapper.main_win);
}

void View::clearStatusBar()
{
	wrapper.cursClear(wrapper.info_bar_win);
	wrapper.cursBox(wrapper.info_bar_win, 0, 0);
}

void View::showHelpInfo()
{
	wrapper.cursPrintInfoBar(wrapper.info_bar_win, 1, (char*)"Write mode i, I, S, A, r. Find mode /,?. Command mode : ........Press any key to back");
	wrapper.cursWRefresh(wrapper.info_bar_win);
}

void View::clearColumnBar()
{
	wrapper.cursClear(wrapper.column_win);
	//wrapper.cursBox(wrapper.column_win, 0, 0);
	wrapper.cursWRefresh(wrapper.column_win);
}

void View::showColumns(int* current_numbers_column)
{
	for (int i = 0; i < HEIGHT_WINDOW_COLUMN; ++i)
	{
		MyString coordinate_y(current_numbers_column[i]);
		wrapper.cursPrintColumnNumber(wrapper.column_win, i, coordinate_y.data());
		wrapper.cursWRefresh(wrapper.column_win);
	}
}