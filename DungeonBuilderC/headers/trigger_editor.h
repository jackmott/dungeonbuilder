#ifndef TRIGGER_EDITOR_H
#define TRIGGER_EDITOR_H

#include <map>
#include "dungeon_trigger.h"
#include "lib.h"


struct TriggerEditor
{
	typedef std::string (TriggerEditor::*commandFunction) (std::vector<std::string>);			
	std::map<std::string,commandFunction> cmdMap;

	DungeonTrigger* trigger;

	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;
	WINDOW *headerWindow;

	int w,h;

	
	void clearWindows();
	void resetWindows();
	void load(DungeonTrigger *_trigger);

	std::string edit(std::vector<std::string> args);
	std::string exit(std::vector<std::string> args);
	std::string add(std::vector<std::string> args);
	std::string set(std::vector<std::string> args);
	std::string del(std::vector<std::string> args);
};

#endif