#ifndef ABSTRACT_EDITOR_H
#define ABSTRACT_EDITOR_H

#include <vector>
#include <map>
#include <string>
#include "lib.h"
#include "utils.h"

extern GlobalState globalState;

struct AbstractEditor {
	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;
	WINDOW *headerWindow;

	int scrollpos=0;

	std::vector<std::string> textBuffer;
	
	bool checkCommonInput(std::string s);
	void renderTextBuffer();
	
};
#endif