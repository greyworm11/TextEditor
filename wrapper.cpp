#include "wrapper.h"

void Wrapper::cursInit()
{
	initscr();
	keypad(stdscr, TRUE);
	noecho();
}

WINDOW* Wrapper::cursNewWin(int a, int b, int c, int d)
{
	return newwin(a, b, c, d);
}

void Wrapper::cursBox(WINDOW* win, int a, int b)
{
	box(win, a, b);
}

void Wrapper::cursWRefresh(WINDOW* win)
{
	wrefresh(win);
}

void Wrapper::cursRefresh()
{
	refresh();
}

void Wrapper::cursClear(WINDOW* win)
{
	wclear(win);
}

void Wrapper::cursMove(int x, int y)
{
	move(x, y);
}

void Wrapper::cursPrintLine(WINDOW* win, int number_line, char* line)
{
	mvwprintw(win, number_line, 0, "%s", line);
}

void Wrapper::cursPrintInfoBar(WINDOW* win, int number_line, char* line)
{
	mvwprintw(win, number_line, 1, "%s", line);
}

void Wrapper::cursPrintColumnNumber(WINDOW* win, int number_line, char* line)
{
	mvwprintw(win, number_line, 3, "%s", line);
}