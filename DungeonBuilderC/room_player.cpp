#include "headers/room_player.h"

using namespace std;

string RoomPlayer::exit(vector<string> args)
{
	return STR_EXIT;
}

string RoomPlayer::take(vector<string> args)
{
	if(args.size() < 2) {
		return "What do you want to take?";
	}
	string takeNoun = args[1];
	toLower(&takeNoun);
	
	auto i = 0u;
	for (i = 0u; i < room->objects.size(); i++)
	{
		DungeonObject *o = room->objects[i];
		if(o->name == takeNoun && o->takeable)
		{			
			player->objects.push_back(o);
			break;
		}
	}
	room->objects.erase(room->objects.begin()+i);
	clearWindows();
	resetWindows();
	return "You take the " + args[1];
	
}
string RoomPlayer::use(vector<string> args)
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


void RoomPlayer::clearWindows()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
	delwin(headerWindow);
	clear();
}

void RoomPlayer::resetWindows()
{
	headerWindow = newwin(1,COLS,0,0);
	commandWindow = newwin(1,COLS,LINES-1,0);
	responseWindow = newwin(1,COLS,LINES-2,0);
	mainWindow = newwin(LINES-3,COLS-8,1,4);
	scrollok(mainWindow,TRUE);
	getmaxyx(stdscr,h,w); // this doesn't work in windows

	string header = "Dungeon!  Room:"+room->name;
	mvwprintwCenterBold(headerWindow,0,header.c_str());

	int lineCount = 1;

	string desc = room->description;
	mvwprintw(mainWindow,lineCount,0,desc.c_str());

	lineCount++;
	for(auto i = 0u; i < room->objects.size(); i++)
	{
		lineCount++;
		mvwprintw(mainWindow,lineCount,0,room->objects[i]->description.c_str());
	}

	lineCount++;
	lineCount++;
	for(auto i =0u; i < room->exits.size();i++)
	{
		lineCount++;
		mvwprintw(mainWindow,lineCount,0,room->exits[i]->description.c_str());
	}



	refresh();
	wrefresh(headerWindow);
	wrefresh(commandWindow);
	wrefresh(responseWindow);
	wrefresh(mainWindow);

}

void RoomPlayer::load(DungeonRoom *_room,DungeonPlayer *_player)
{
	player = _player;
	room = _room;
	cmdMap[STR_EXIT] = &RoomPlayer::exit;
	cmdMap[STR_USE] = &RoomPlayer::use;
	cmdMap[STR_TAKE] = &RoomPlayer::take;

	//iterate over players inventory and add all
	//aliases for the verb 'use' to the cmdMap
	for(auto i = 0u; i < player->objects.size();i++)
	{
		DungeonObject *o = player->objects[i];
		for(auto j = 0u; j < o->useAliases.size(); j++)
		{
			cmdMap[o->useAliases[i]] = &RoomPlayer::use;
		}
	}

	//create a map of exit names to move to
	for(auto i = 0u; i < room->exits.size();i++)
	{
		string name = room->exits[i]->name;
		toLower(&name);
		moveMap[name] = room->exits[i]->room;
	}

	resetWindows();

	CommandWindow cmdW;
	bool cmdFound = false;
	vector<string> cmd;
	while(true) {
		cmd = cmdW.command(commandWindow,STR_PROMPT);
		if(cmd.size() > 0) {
			toLower(&cmd[0]);
			cmdFound = cmdMap.count(cmd[0]) > 0;

			if(!cmdFound) {
				bool moveFound = moveMap.count(cmd[0]) > 0;
				if(!moveFound) {
					cmd.clear();
					mvwprintw(responseWindow,0,0,"What are you doing, dave?");
					wclrtoeol(responseWindow);
					wrefresh(responseWindow);
				}
				else
				{
					DungeonRoom *newRoom = moveMap[cmd[0]];
					room = newRoom;
					clearWindows();
					resetWindows();
				}
			}
			else
			{
				if(cmd[0] == STR_USE) break;
				commandFunction cmdFunc = cmdMap[cmd[0]];
				string response = (this->*cmdFunc)(cmd);
				if(response.length() > 0) {
					cmd.clear();
					mvwprintw(responseWindow,0,0,response.c_str());
					wclrtoeol(responseWindow);
					wrefresh(responseWindow);
				}
			}
		}

	}


	clearWindows();
}
