#include "controller.h"

Controller::Controller(Model* model_)
{
	model = model_;
}

void Controller::start()
{
	while (1)
	{
		int symbol = getch();
		processSymbol(symbol);
	}
}

void Controller::processSymbol(int symbol)
{
	int mode_now = model->getMode();
	switch (mode_now)
	{
	case NAVIGATION:
		navigationMode(symbol);
		break;
	case WRITE:
		if (symbol == KEY_CODE_ESC)
		{
			model->setMode(NAVIGATION);
			model->updateStatusBar();
		}
		else
		{
			writeMode(symbol);
		}
		break;

	case FIND:
		if (symbol == KEY_CODE_ESC)
		{
			model->setMode(NAVIGATION);
			model->updateStatusBar();
		}
		else
		{
			findMode(symbol);
		}
		break;

	case COMMAND:
		if (symbol == KEY_CODE_ESC)
		{
			model->setMode(NAVIGATION);
			model->updateStatusBar();
		}
		break;
	}
}


void Controller::navigationMode(int symbol)
{
	int ch;
	switch (symbol)
	{
	case 'i':
		model->setMode(WRITE);
		model->updateAll();
		break;

	case 'I':
		model->goToStartCurrentString();
		model->setMode(WRITE);
		model->updateAll();
		break;

	case 'S':
		model->goToEndCurrentString();
		model->setMode(WRITE);
		model->updateAll();
		break;

	case 'A':
		model->cleanCurrentString();
		model->setMode(WRITE);
		model->updateAll();
		break;

	case 'r':
		ch = getch();
		model->replaceCurrentSymbol(ch);
		model->setMode(WRITE);
		model->updateAll();
		break;

	case ':':
		model->setMode(COMMAND);
		commandMode();
		model->updateAll();
		break;

	case KEY_UP:
		model->KeyUp();
		break;

	case KEY_DOWN:
		model->KeyDown();
		break;

	case KEY_LEFT:
		model->KeyLeft();
		break;

	case KEY_RIGHT:
		model->KeyRight();
		break;

	case KEY_PGU:
		model->keyPageUp();
		break;

	case KEY_PGD:
		model->keyPageDown();
		break;

	case 'g':
		ch = getch();
		if (ch == (int)'g') model->goToTheFirstPage();
		break;

	case 'G':
		model->goToTheLastPage();
		break;

	case '^':
		model->goToStartCurrentString();
		break;

	case '$':
		model->goToEndCurrentString();
		break;

	case 'w':
		model->goToEndWord();
		model->updateAll();
		break;

	case 'b':
		model->goToStartWord();
		model->updateAll();
		break;

	case 'x':
		model->deleteCurrentSymbol();
		model->updateAll();
		break;

	case 'y':
		ch = getch();
		if (ch == KEY_CODE_ENTER)
		{
			model->copyString();
			model->updateAll();
		}
		else
		{
			if (ch == 'w')
			{
				model->copyWord();
				model->updateAll();
			}
		}
		break;

	case 'd':
		ch = getch();
		if (ch == 'd')
		{
			model->cutCurrentStr();
		}
		if (ch == 'i')
		{
			ch = getch();
			if (ch == 'w')
			{
				model->deleteCurrentWord();
			}
		}
		model->updateAll();
		break;

	case 'p':
		model->insertString();
		model->updateAll();
		break;

	case '?':
		findMode(symbol);
		break;

	case '/':
		findMode(symbol);
		break;

	default:
		if (symbol - 48 >= 0 && symbol - 48 <= 9) // if digit
		{
			model->goToStringView();
			model->goToStringNum(symbol);
			while (1)
			{
				ch = getch();
				model->goToStringNum(ch);
				if (ch == 'G')
				{
					break;
				}
			}
			model->updateAll();
		}
		break;
	}
}


void Controller::writeMode(int symbol)
{
	switch (symbol)
	{
	case KEY_UP:
		model->KeyUp();
		break;
	case KEY_DOWN:
		model->KeyDown();
		break;
	case KEY_LEFT:
		model->KeyLeft();
		break;
	case KEY_RIGHT:
		model->KeyRight();
		break;
	case KEY_PGU:
		model->keyPageUp();
		break;
	case KEY_PGD:
		model->keyPageDown();
		break;
	case KEY_CODE_ENTER:
		model->Enter();
		break;
	case KEY_CODE_BACKSPACE:
		model->Backspace();
		break;
	case KEY_DC:
		break;
	default:
		model->insertSymbol(symbol);
	}
}

void Controller::findMode(int symbol)
{
	int ch;
	model->goFindView(symbol);
	model->setMode(FIND);
	while (true)
	{
		// new
		if (symbol == 's')
		{
			MyString set_num = "et num";
			for (int i = 0; ; ++i)
			{
				ch = getch();
				if (set_num[i] != ch) break;
				if (set_num[i] == 'm' && ch == 'm')
				{
					if (model->getCommand().comd->get()) {
						model->cmd.comd->Undo();
					}
					else {
						model->cmd.comd->Execute();
					}
					break;
				}
			}
			model->updateAll();
			break;
		}
		// end
		ch = getch();

		if (symbol == '?')
		{
			model->findTextBefore(ch);
		}
		else if (symbol == '/')
		{
			model->findTextAfter(ch);
		}

		if (ch == KEY_CODE_ESC)
		{
			model->setMode(NAVIGATION);
			break;
		}
	}
	model->updateAll();
}

void Controller::commandMode()
{
	int ch;
	model->commandView();
	while (1)
	{
		if (model->clean_all == true)
			break;
		ch = getch();
		if (ch == KEY_CODE_ESC)
			break;
		model->readCommand(ch);
	}
	model->clean_all = false;
	model->setMode(NAVIGATION);
}