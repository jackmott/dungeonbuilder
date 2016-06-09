#ifndef EFFECT_EDITOR
#define EFFECT_EDITOR

#include <map>
#include <vector>
#include <string>
#include "lib.h"

struct DungeonEffect;


struct EffectEditor
{
	typedef std::string  (EffectEditor::*commandFunction) (std::vector<std::string>);			
	std::map<std::string,commandFunction> cmdMap;

	DungeonEffect* effect;

	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;
	WINDOW *headerWindow;

	int w,h;

	
	void clearWindows();
	void resetWindows();
	void load(DungeonEffect *_effect);

	std::string edit(std::vector<std::string> args);
	std::string exit(std::vector<std::string> args);
	std::string add(std::vector<std::string> args);
	std::string set(std::vector<std::string> args);
	std::string del(std::vector<std::string> args);

};

#endif