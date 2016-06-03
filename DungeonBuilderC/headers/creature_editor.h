#ifndef CREATURE_EDITOR
#define CREATURE_EDITOR

#include <vector>
#include <string>
#include <map>
#include "lib.h"

struct DungeonCreature;


struct CreatureEditor
{
	typedef std::string  (CreatureEditor::*commandFunction) (std::vector<std::string>);			
	std::map<std::string,commandFunction> cmdMap;

	DungeonCreature *creature;

	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;
	WINDOW *headerWindow;

	int w,h;

	
	void clearWindows();
	void resetWindows();
	void load(DungeonCreature *creature);

	std::string add(std::vector<std::string> args);
	std::string del(std::vector<std::string> args);
	std::string set(std::vector<std::string> args);
	std::string edit(std::vector<std::string> args);
	std::string exit(std::vector<std::string> args);	
};

#endif
