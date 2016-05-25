#include "dungeon_engine.h"
#include <Windows.h>
#include "printutils.h"
#include "utils.h"
#include "command_window.h"
#include "string_constants.h"

using namespace std;

string DungeonEngine::exit(string args)
{
	return STR_EXIT;
}

string DungeonEngine::put(string args)
{
	DungeonObject* putObject = extractAndRemoveObject(&player->objects,&args);
	if(putObject == nullptr)
	{
		putObject = extractAndRemoveObject (&room->objects,&args);
	}
	if(putObject != nullptr)
	{
		DungeonObject* containerObject =  extractObject(&player->objects,&args); 
		if (containerObject == nullptr )		
		{
			containerObject = extractObject(&room->objects,&args);			
		}
		if (containerObject != nullptr && containerObject->isOpen)
		{
			containerObject->contents.push_back(putObject);
			return "You put the "+ putObject->name + " in the " + containerObject->name + ".";			
		} else
		{
			return "You try to but you can't put that there.";
		}
	}

	return "You flounder about, with no success.";
}

string DungeonEngine::take(string args)
{
	DungeonObject* takenObject = extractAndRemoveObject(&room->objects,&args);

	if(takenObject == nullptr)
	{
		for(auto o : room->objects)
		{
			if(o->isOpen) {
				takenObject = extractAndRemoveObject(&o->contents,&args);
				if(takenObject != nullptr)
				{
					break;
				}
			}
		}

		if(takenObject == nullptr)
		{
			for(auto o : player->objects)
			{
				if(o->isOpen) {
					takenObject = extractAndRemoveObject(&o->contents,&args);
					if(takenObject != nullptr)
					{
						break;
					}
				}
			}
		}
	}

	if(takenObject != nullptr)
	{
		player->objects.push_back(takenObject);
		return takenObject->name + " taken.";
	}
	else
	{
		return "You try to take it, but it seems futile";
	}

}
string DungeonEngine::use(string args)
{
	DungeonObject* roomObject = extractObject(&room->objects,&args);
	DungeonObject* playerObject = extractObject(&player->objects,&args);
	DungeonCreature* creature = extractCreature(&room->creatures,&args);

	if(playerObject != nullptr && creature != nullptr) {
		string response = creature->attack(playerObject,player);
		if(creature->hitpoints <= 0)
		{
			for(vector<DungeonCreature*>::iterator it = room->creatures.begin(); it != room->creatures.end(); ++it)
			{
				if(*it == creature)
				{
					room->creatures.erase(it);
					break;
				}
			}
		}
		return response;
	}
	else
	{
		return "Your attempt amounts to nothing.";
	}

}

string DungeonEngine::open(string args)
{
	DungeonObject* thingToOpen = extractObject(&room->objects,&args);
	if(thingToOpen == nullptr)
	{
		thingToOpen = extractObject(&player->objects,&args);
	}


	if(thingToOpen->canOpen == true && thingToOpen->isOpen == false)
	{
		thingToOpen->isOpen = true;
		textBuffer.push_back("You open the "+thingToOpen->name+", inside you see");
		showContents(thingToOpen);
		return "";
	}


	return "You can't open that";
}


string DungeonEngine::lookCmd(string args)
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
	
	init_pair(1,COLOR_BLACK,COLOR_RED);
	wbkgd(headerWindow,COLOR_PAIR(1));
	//setcolors(headerWindow,1,COLOR_BLACK,COLOR_RED);
	//wclrtoeol(headerWindow);
	mvwprintw(headerWindow,0,0,"Dungeon Builder");
	mvwprintwCenter(headerWindow,0,room->name.c_str());

	look();

	refresh();
	wrefresh(headerWindow);
	wrefresh(commandWindow);
	wrefresh(mainWindow);

}


void DungeonEngine::look()
{
	addToBuffer(&room->description);

	for(auto creature : room->creatures)
	{
		textBuffer.push_back(thereIsA(creature->name));
	}

	for(auto o : room->objects)
	{
		textBuffer.push_back(thereIsA(o->name));
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

void DungeonEngine::showContents(DungeonObject* o)
{

	for(auto content: o->contents)
	{
		textBuffer.push_back("  "+a_an(content->name));
	}

}

void DungeonEngine::render(unsigned int start,unsigned int end)
{
	for(auto i = start; i < end; i++)
	{
		wprintw(mainWindow,(textBuffer[i]+"\n").c_str());
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
	cmdMap[STR_PUT] = &DungeonEngine::put;
	cmdMap[STR_PLACE] = &DungeonEngine::put;

	//iterate over players inventory and add all
	//aliases for the verb 'use' to the cmdMap
	for(auto o : player->objects)
	{
		for(auto alias : o->useAliases)
		{
			cmdMap[alias] = &DungeonEngine::use;
		}
	}

	for(auto o : room->objects)
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

	while(true) {
		updateCmdMap();
		render(renderPos,textBuffer.size());
		string userInput = cmdW.getCommandAsString(commandWindow,STR_PROMPT);
		textBuffer.push_back(STR_PROMPT+userInput);

		if(userInput.length() > 0) {


			vector<string> verbs;
			for(map<string,commandFunction>::iterator it = cmdMap.begin(); it != cmdMap.end(); ++it) {
				verbs.push_back(it->first);
			}

			string verb = extractPhrase(&verbs,&userInput);

			if(verb == "") {
				vector<string> directions;
				for(map<string,DungeonRoom*>::iterator it = moveMap.begin(); it != moveMap.end(); ++it) {
					directions.push_back(it->first);
				}

				string move = extractPhrase(&directions,&userInput);

				if(move == "") {
					textBuffer.push_back("What are you doing, dave?");
				}
				else
				{
					DungeonRoom *newRoom = moveMap[move];
					room = newRoom;
					look();
				}
			}
			else
			{
				if(verb == STR_EXIT) break;
				commandFunction cmdFunc = cmdMap[verb];
				string response = (this->*cmdFunc)(userInput);
				if(response.length() > 1) {
					textBuffer.push_back(response);
				}
			}
		}

	}


	clearWindows();
}
