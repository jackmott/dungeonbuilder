#pragma once

#include <map>
#include "lib.h"

struct DungeonObject;


struct ObjectEditor
{
	typedef std::string  (ObjectEditor::*commandFunction) (std::vector<std::string>);			
	std::map<std::string,commandFunction> cmdMap;

	DungeonObject *object;

	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;
	WINDOW *headerWindow;

	int w,h;

	
	void clearWindows();
	void resetWindows();
	void load(DungeonObject *object);

	std::string edit(std::vector<std::string> args);
	std::string exit(std::vector<std::string> args);
	std::string add(std::vector<std::string> args);
	std::string set(std::vector<std::string> args);
	std::string del(std::vector<std::string> args);
};

