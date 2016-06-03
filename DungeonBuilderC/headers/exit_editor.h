#ifndef EXIT_EDITOR_H
#define EXIT_EDITOR_H

#include <vector>
#include <string>
#include <map>
#include "lib.h"

struct DungeonRoom;
struct DungeonExit;

extern DungeonRoom *g_startRoom;
extern std::vector<DungeonRoom*> g_roomList;
struct ExitEditor
{
	typedef std::string  (ExitEditor::*commandFunction) (std::vector<std::string>);			
	std::map<std::string,commandFunction> cmdMap;

	DungeonExit *dungeonExit;
	

	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;
	WINDOW *headerWindow;

		
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