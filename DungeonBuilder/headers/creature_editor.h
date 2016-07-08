#ifndef CREATURE_EDITOR
#define CREATURE_EDITOR

#include "abstract_editor.h"


struct DungeonCreature;


struct CreatureEditor : AbstractEditor
{
	typedef std::string  (CreatureEditor::*commandFunction) (const std::vector<std::string> &);			
	std::map<std::string,commandFunction> cmdMap;

	DungeonCreature *creature;
	
	void clearWindows();
	void resetWindows();
	void load(DungeonCreature *creature);

	std::string add(const std::vector<std::string> &args);
	std::string del(const std::vector<std::string> &args);
	std::string set(const std::vector<std::string> &args);
	std::string edit(const std::vector<std::string> &args);
	std::string exit(const std::vector<std::string> &args);	
};

#endif
