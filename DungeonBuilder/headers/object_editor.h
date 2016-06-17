#ifndef OBJECT_EDITOR_H
#define OBJECT_EDITOR_H

#include "abstract_editor.h"

struct DungeonObject;


struct ObjectEditor : AbstractEditor
{
	typedef std::string  (ObjectEditor::*commandFunction) (std::vector<std::string>);			
	std::map<std::string,commandFunction> cmdMap;

	DungeonObject *object;

		
	void clearWindows();
	void resetWindows();
	void load(DungeonObject *object);

	std::string edit(std::vector<std::string> args);
	std::string exit(std::vector<std::string> args);
	std::string add(std::vector<std::string> args);
	std::string set(std::vector<std::string> args);
	std::string del(std::vector<std::string> args);
};

#endif