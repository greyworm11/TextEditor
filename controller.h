#ifndef CONTROLLER_H
#define CONTROLLER_H

#pragma once
#include "model.h"

class Controller
{
public:
	Model* model;

	Controller(Model*); // constructor
	void start(); // start Controller
	void processSymbol(int);

	// modes
	void navigationMode(int);
	void writeMode(int);
	void commandMode();
	void findMode(int);
};

#endif
