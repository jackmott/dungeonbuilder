#ifndef CREATURE_EDITOR
#define CREATURE_EDITOR

#include "abstract_editor.h"


struct DungeonCreature;


struct CreatureEditor : AbstractEditor
{
	typedef std::string  (CreatureEditor::*commandFunction) (std::vector<std::string>);			
	std::map<std::string,commandFunction> cmdMap;

	DungeonCreature *creature;
	
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
