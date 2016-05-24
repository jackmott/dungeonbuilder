#include "dungeon_engine.h"

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
		if(o->name == takeNoun && o->takeable)
		{
			match = o;
			player->objects.push_back(o);
			break;
		}
	}
	if(match != nullptr) {
		room->objects.erase(room->objects.begin()+i);
		clearWindows();
		resetWindows();
		return "You take the " + args[1];
	}
	else
	{
		return "I don't see that here";
	}

}
string DungeonEngine::use(vector<string> args)
{
	if(args.size() < 2) {
		return "What do you want to use?";
	}

	if(args.size() < 3)
	{
		return "What do you want to use " + args[1] + " on?";
	}
	string useNoun = args[1];
	toLower(&useNoun);

	string subject = args[2];
	toLower(&subject);

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


	addToBuffer(&room->description);

	for(auto o : room->objects)
	{
		addToBuffer(&o->description);
	}

	for(auto exit : room->exits)
	{
		addToBuffer(&exit->description);
	}


	refresh();
	wrefresh(headerWindow);
	wrefresh(commandWindow);	
	wrefresh(mainWindow);

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

void DungeonEngine::load(DungeonRoom *_room,DungeonPlayer *_player)
{
	player = _player;
	room = _room;
	renderPos = 0;
	cmdMap[STR_EXIT] = &DungeonEngine::exit;
	cmdMap[STR_USE] = &DungeonEngine::use;
	cmdMap[STR_TAKE] = &DungeonEngine::take;

	//iterate over players inventory and add all
	//aliases for the verb 'use' to the cmdMap
	for(auto o : player->objects)
	{
		for(auto alias : o->useAliases)
		{
			cmdMap[alias] = &DungeonEngine::use;
		}
	}

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
				if(response.length() > 0) {
					cmd.clear();
					textBuffer.push_back(response);					
				}
			}
		}

	}


	clearWindows();
}
