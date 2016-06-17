#ifndef TRIGGER_EDITOR_H
#define TRIGGER_EDITOR_H

#include "abstract_editor.h"

struct TriggerEditor : AbstractEditor
{
	typedef std::string (TriggerEditor::*commandFunction) (std::vector<std::string>);			
	std::map<std::string,commandFunction> cmdMap;

	DungeonTrigger* trigger;
		
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