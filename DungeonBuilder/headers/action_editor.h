#ifndef ACTION_EDITOR_H
#define ACTION_EDITOR_H

#include <map>
#include "dungeon_action.h"
#include "lib.h"


struct ActionEditor
{
	typedef std::string (ActionEditor::*commandFunction) (std::vector<std::string>);			
	std::map<std::string,commandFunction> cmdMap;

	DungeonAction* action;

	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;
	WINDOW *headerWindow;

	int w,h;

	
	void clearWindows();
	void resetWindows();
	void load(DungeonAction *_action);

	std::string edit(std::vector<std::string> args);
	std::string exit(std::vector<std::string> args);
	std::string add(std::vector<std::string> args);
	std::string set(std::vector<std::string> args);
	std::string del(std::vector<std::string> args);
};

#endif