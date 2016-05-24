#include "dungeon_engine.h"
#include <Windows.h>
#include "printutils.h"
#include "utils.h"
#include "command_window.h"
#include "string_constants.h"

using namespace std;

string DungeonEngine::exit(vector<string> args)
{
	return STR_EXIT;
}

string DungeonEngine::take(vector<string> args)
{
	if(args.size() < 2) {
		return "What do you want to take?";
	}
	string takeNoun = args[1];
	toLower(&takeNoun);

	auto i = 0u;
	DungeonObject *match = nullptr;
	for(i = 0u; i < room->objects.size(); i++)
	{
		DungeonObject *o = room->objects[i];
		if(o->name == takeNoun && o->canTake)
		{
			match = o;
			player->objects.push_back(o);
			break;
		}
	}
	if(match != nullptr) {
		room->objects.erase(room->objects.begin()+i);
		return "You take the " + args[1];
	}
	else
	{
		return "I don't see that here";
	}

}
string DungeonEngine::use(vector<string> args)
{
	args = removeArticles(args);
	if(args.size() < 2) {
		return "What do you want to use?";
	}

	string thingToUse = "";
	string subject = "";
	if(containsWith(args))
	{
		if(args.size() < 3)
		{
			return "What do you want to do that with?";
		}
		thingToUse = args[3];
		subject = args[1];
	}
	else
	{
		if(args.size() < 3)
		{
			return "What do you want to use that on?";
		}
		thingToUse = args[1];
		subject = args[2];
	}

	toLower(&thingToUse);
	toLower(&subject);

	DungeonObject* objectToUse = nullptr;
	for(auto o : player->objects)
	{
		if(o->name == thingToUse)
		{
			objectToUse = o;
		}
	}

	DungeonCreature* creatureToAffect = nullptr;
	int creatureIndex = 0;
	for(auto creature : room->creatures)
	{
		if(creature->name == subject)
		{
			creatureToAffect = creature;
		}
		creatureIndex++;
	}

	if(objectToUse != nullptr && creatureToAffect != nullptr)
	{
		string response = creatureToAffect->attack(objectToUse,player);
		if(creatureToAffect->hitpoints <= 0)
		{
			room->creatures.erase(room->creatures.begin()+creatureIndex-1);
		}
		return response;
	}
	else
	{
		return "I don't understand what you are asking me to do.";
	}

	return "";

}

string DungeonEngine::lookCmd(vector<string> args)
{
	look();
	return "";
}


void DungeonEngine::clearWindows()
{
	delwin(commandWindow);
	delwin(mainWindow);
	delwin(headerWindow);
	clear();
}

void DungeonEngine::addToBuffer(vector<string> *v)
{
	textBuffer.insert(textBuffer.end(),v->begin(),v->end());
}



void DungeonEngine::resetWindows()
{
	headerWindow = newwin(1,COLS,0,0);
	commandWindow = newwin(1,COLS,LINES-1,0);
	mainWindow = newwin(LINES-2,COLS,1,0);
	scrollok(mainWindow,TRUE);
	getmaxyx(stdscr,h,w); // this doesn't work in windows

	string header = "Dungeon!  Room:"+room->name;
	mvwprintwCenterBold(headerWindow,0,header.c_str());

	look();

	refresh();
	wrefresh(headerWindow);
	wrefresh(commandWindow);
	wrefresh(mainWindow);

}

string DungeonEngine::open(vector<string> args)
{
	args = removeArticles(args);
	if(args.size() < 2) {
		textBuffer.push_back("What do you want to open?");
	}

	string thingToOpen = args[1];
	toLower(&thingToOpen);


	vector<DungeonObject*> allObjects;
	allObjects.insert(allObjects.end(),player->objects.begin(),player->objects.end());
	allObjects.insert(allObjects.end(),room->objects.begin(),room->objects.end());
	for(auto o : allObjects)
	{
		if(o->name == thingToOpen)
		{
			if(o->canOpen == true && o->isOpen == false)
			{
				o->isOpen = true;
				textBuffer.push_back("You open the "+o->name+", inside you see");
				showContents(o);
				return "";
			}
			
		}
	}

	return "You can't open that";
}

void DungeonEngine::showContents(DungeonObject* o)
{

	for(auto content: o->contents)
	{
		textBuffer.push_back("  "+a_an(content->name));
	}

}


void DungeonEngine::look()
{
	addToBuffer(&room->description);

	for(auto creature : room->creatures)
	{
		addToBuffer(&creature->description);
	}

	for(auto o : room->objects)
	{
		addToBuffer(&o->description);
		if(o->isOpen && o->contents.size() > 0) {
			textBuffer.push_back("Inside the "+ o->name + " you see");
			showContents(o);
		}
	}

	for(auto exit : room->exits)
	{
		addToBuffer(&exit->description);
	}

}

void DungeonEngine::render(unsigned int start,unsigned int end)
{
	for(auto i = start; i < end; i++)
	{
		string line = textBuffer[i];
		line = line + "\n";
		wprintw(mainWindow,line.c_str());
		wrefresh(mainWindow);
		renderPos++;
		Sleep(200);
	}

}

void DungeonEngine::updateCmdMap()
{
	cmdMap[STR_EXIT] = &DungeonEngine::exit;
	cmdMap[STR_USE] = &DungeonEngine::use;
	cmdMap[STR_TAKE] = &DungeonEngine::take;
	cmdMap[STR_LOOK] = &DungeonEngine::lookCmd;
	cmdMap[STR_OPEN] = &DungeonEngine::open;
	//iterate over players inventory and add all
	//aliases for the verb 'use' to the cmdMap
	for(auto o : player->objects)
	{
		for(auto alias : o->useAliases)
		{
			cmdMap[alias] = &DungeonEngine::use;
		}
	}

}

void DungeonEngine::load(DungeonRoom *_room,DungeonPlayer *_player)
{
	player = _player;
	room = _room;
	renderPos = 0;

	//create a map of exit names to move to
	for(auto e : room->exits)
	{
		string name = e->name;
		toLower(&name);
		moveMap[name] = e->room;
	}

	resetWindows();

	CommandWindow cmdW;
	bool cmdFound = false;
	vector<string> cmd;
	while(true) {
		updateCmdMap();
		render(renderPos,textBuffer.size());
		cmd = cmdW.command(commandWindow,STR_PROMPT);
		string cmdString = STR_PROMPT + join(0,cmd," ");
		textBuffer.push_back(cmdString);
		if(cmd.size() > 0) {
			toLower(&cmd[0]);
			cmdFound = cmdMap.count(cmd[0]) > 0;

			if(!cmdFound) {
				bool moveFound = moveMap.count(cmd[0]) > 0;
				if(!moveFound) {
					cmd.clear();
					textBuffer.push_back("What are you doing, dave?");
				}
				else
				{
					DungeonRoom *newRoom = moveMap[cmd[0]];
					room = newRoom;
				}
			}
			else
			{
				if(cmd[0] == STR_EXIT) break;
				commandFunction cmdFunc = cmdMap[cmd[0]];
				string response = (this->*cmdFunc)(cmd);
				if(response.length() > 1) {
					cmd.clear();
					textBuffer.push_back(response);
				}
			}
		}

	}


	clearWindows();
}
