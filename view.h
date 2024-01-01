#ifndef VIEW_H
#define VIEW_H

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include "wrapper.h"
#include "MyString.h"

#pragma comment(lib,"LibMyString.lib")

#define KEY_CODE_ESC 27
#define KEY_CODE_ENTER 10
#define KEY_CODE_BACKSPACE 8

#define KEY_PGD 457
#define KEY_PGU 451

#define WIDTH_WINDOW 110
#define HEIGHT_WINDOW_MAIN 27
#define HEIGHT_WINDOW_BAR 3
#define BEGIN_WINDOW_X 0
#define BEGIN_WINDOW_Y 0

#define HEIGHT_WINDOW_COLUMN 27
#define WIDTH_WINDOW_COLUMN 3
#define BEGIN_WINDOW_COLUMN_X 114
#define BEGIN_WINDOW_COLUMN_Y 0

using namespace std;

enum Modes
{
	NAVIGATION = 1,
	WRITE,
	FIND,
	COMMAND
};

class View
{
public:
	View();

	void showText(vector<MyString>, int, int);
	void showStatusBar(int, int, int, char*);
	void showHelpInfo();

	int  countLinesInString(int, vector <MyString>);
	void setCoords(int, int);
	void clearMainBar();
	void clearStatusBar();
	void GoToString(char*);
	void find(char, char*);

	void clearColumnBar();
	void showColumns(int*);

private:
	Wrapper wrapper;
};

#endif