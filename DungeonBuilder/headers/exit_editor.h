#ifndef EXIT_EDITOR_H
#define EXIT_EDITOR_H

#include "abstract_editor.h"

struct GlobalState;
struct DungeonRoom;
struct DungeonExit;

extern GlobalState globalState;

struct ExitEditor : AbstractEditor
{
	typedef std::string  (ExitEditor::*commandFunction) (std::vector<std::string>);			
	std::map<std::string,commandFunction> cmdMap;

	DungeonExit *dungeonExit;
	
		
	void clearWindows();
	void resetWindows();
	DungeonRoom* load(DungeonExit *_dungeonExit);

	std::string set(std::vector<std::string> args);
	std::string add(std::vector<std::string> args);
	std::string del(std::vector<std::string> args);
	std::string edit(std::vector<std::string> args);
	std::string exit(std::vector<std::string> args);	
};

#endif