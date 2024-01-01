#ifndef MODEL_H
#define MODEL_H

#pragma once
#include "view.h"
#include <fstream>
#include "Command.h"

class Model
{
private:
	vector <MyString> text;

	View* view;
	int current_x = 0, current_y = 0;
	int x_scr = 0, y_scr = 0;
	int current_mode = NAVIGATION;
	int part_current_line = 0;
	int part_up_line = 0, up_line = 0;
	int part_down_line = 0, down_line = 0;
	int x_status = 0, y_status = 28;

	MyString filename = "example.txt";
	MyString command = "";
	MyString string_num = "";
	MyString finding_str = "";
	MyString copiedStr = "";

public:
	Model(View*);

	int  countLinesInString(int);
	void setDownLine();
	void setUpLine();
	void commandView();
	void goToStringView();
	void updateStatusBar();
	void updateMainWin();
	void updateAll();
	void goFindView(int symbol);
	void showHelp();
	void setMode(int new_mode);
	int  getMode();
	void openFile(MyString);
	void KeyUp();
	void KeyLeft();
	void KeyDown();
	void KeyRight();
	void getTrueColsRowsUP();
	void getTrueColsRowsDOWN();
	void getTrueColsRowsLeft();
	void getTrueColsRowsRight();
	void nextDownLine();
	void nextUpLine();
	void previousDownLine();
	void previousUpLine();
	void keyPageUp();
	void keyPageDown();
	void goToTheFirstPage();
	void goToTheLastPage();
	void replaceCurrentSymbol(int);
	void cleanCurrentString();
	void goToStartCurrentString();
	void goToEndCurrentString();
	int  checkWordRightSide(int, int);
	int  checkWordLeftSide(int, int);
	void goToEndWord();
	void goToStartWord();
	void goToStringNum(int);
	void deleteCurrentSymbol();
	void copyString();
	void cutCurrentStr();
	void insertString();
	void copyWord();
	void deleteCurrentWord();
	void findTextAfter(int);
	void findTextBefore(int);
	void Enter();
	void Backspace();
	void insertSymbol(int);
	void readCommand(int);
	void parseCommand();
	void writeToFile(MyString);
	void cleanAll();
	void nextSearch();
	void previousSearch();

	bool clean_all = false;
	bool pressedEnter = false;
	bool column_mode = false; 

	void updateColumnWindow();

	Command cmd;
	ShowLine ff;

	void setCommand(Command cmd)
	{
		this->cmd = cmd;
	}

	Command getCommand()
	{
		return cmd;
	}
};

#endif