#ifndef WRAPPER_H
#define WRAPPER_H

#pragma once
#include "curses.h"

class Wrapper
{
public:
	void cursInit();
	void cursBox(WINDOW*, int, int);
	void cursWRefresh(WINDOW*);
	void cursRefresh();
	void cursClear(WINDOW*);
	void cursMove(int, int);

	void cursPrintLine(WINDOW*, int, char*);
	void cursPrintInfoBar(WINDOW*, int, char*);

	WINDOW* cursNewWin(int, int, int, int);

	WINDOW* info_bar_win;
	WINDOW* main_win;

	WINDOW* column_win;
	void cursPrintColumnNumber(WINDOW*, int, char*);
};

#endif