#ifndef ACTION_EDITOR_H
#define ACTION_EDITOR_H

#include "abstract_editor.h"

struct DungeonAction;


struct ActionEditor : AbstractEditor
{
	typedef std::string (ActionEditor::*commandFunction) (std::vector<std::string>);			
	std::map<std::string,commandFunction> cmdMap;

	DungeonAction* action;
				
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