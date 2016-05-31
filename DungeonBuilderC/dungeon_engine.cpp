#include "dungeon_engine.h"
#include "printutils.h"
#include "utils.h"
#include "command_window.h"
#include "string_constants.h"

using namespace std;

string DungeonEngine::exit(string args)
{
	return STR_EXIT;
}

string DungeonEngine::pageUp(string args)
{
	renderOffset = min(renderOffset+pageSize,textBuffer.size()-pageSize);
	return "";
}

string DungeonEngine::pageDown(string args)
{
	renderOffset = max(0,renderOffset - pageSize);
	return "";
}

string DungeonEngine::inventory(string args)
{
	if(player->objects.size() == 0)
	{
		return "You are empty handed.";
	}

	textBuffer.push_back("You are carrying");
	vector<DungeonObject*> *objects = &player->objects;
	showContents(objects);

	return "";

}

string DungeonEngine::drop(string args)
{
	DungeonObject *thing = (DungeonObject*)extractEntity(&player->objects,&args);
	if(thing != nullptr) {
		removeObject(&player->objects,thing);
		room->objects.push_back(thing);
		return "You drop the " + thing->getPrimaryName() +".";
	}
	else {
		return "You don't have that.";
	}
}

string DungeonEngine::examine(string args)
{
	DungeonObject* thing =extractObject(&room->objects,&args);
	if(thing == nullptr)
	{
		thing = extractObject(&player->objects,&args);
	}

	if(thing != nullptr && thing->description.size() == 0)
	{
		return "You see no further detail.";

	}
	else if(thing != nullptr)
	{
		addToBuffer(&thing->description);
		return "";
	}
	else {
		DungeonCreature *gal = (DungeonCreature*)extractEntity(&room->creatures,&args);
		if(gal != nullptr)
		{
			if(gal->description.size() == 0)
			{
				return "There is nothing more to see.";
			}
			else {
				addToBuffer(&gal->description);
				return "";
			}
		}
		else
		{
			return "You don't see that here.";
		}
	}

	return "investigate examine function more";
}

string DungeonEngine::put(string args)
{
	//get the string before and after the word "in" to clarify what is being put where
	unsigned long inLocation = args.find(" in ");
	if(inLocation == string::npos)
	{
		return "Your fumble about, but it doesn't work.";
	}

	string firstHalf = args.substr(0,inLocation);
	string secondHalf = args.substr(inLocation+4,args.size()-(inLocation+4));


	DungeonObject* containerObject =  extractObject(&player->objects,&secondHalf);
	if(containerObject == nullptr)
	{
		containerObject = extractObject(&room->objects,&secondHalf);
	}

	if(containerObject != nullptr && !containerObject->isOpen)
	{
		return "It isn't open.";
	}

	if(containerObject != nullptr && containerObject->isOpen)
	{
		DungeonObject* putObject = extractAndRemoveObject(&player->objects,&firstHalf);
		if(putObject == nullptr)
		{
			putObject = extractAndRemoveObject(&room->objects,&firstHalf);
		}

		if(putObject != nullptr)
		{
			containerObject->contents.push_back(putObject);
			return "You put the "+ putObject->getPrimaryName() + " in the " + containerObject->getPrimaryName() + ".";
		}
		else
		{
			return "You don't have that.";
		}
	}

	return "You flounder about, with no success.";
}

string DungeonEngine::take(string args)
{
	DungeonObject* takenObject = extractAndRemoveObject(&room->objects,&args);

	if(takenObject == nullptr)
	{
		takenObject = (DungeonObject*)extractEntity(&player->objects,&args);
		if(takenObject != nullptr) {
			return "You already have that.";
		}
		else {
			for(auto o : player->objects)
			{
				if(o->isOpen && o->contents.size() > 0) {
					takenObject = extractAndRemoveObject(&o->contents,&args);
					break;
				}
			}
		}
	}

	if(takenObject != nullptr)
	{
		player->objects.push_back(takenObject);
		return takenObject->getPrimaryName() + " taken.";
	}
	else
	{
		return "You try to take it, but it seems futile";
	}

}
string DungeonEngine::use(string args)
{
	//DungeonObject* roomObject = (DungeonObject*)extractEntity(&room->objects,&args);
	DungeonObject* playerObject = extractObject(&player->objects,&args);
	DungeonCreature* creature = (DungeonCreature*)extractEntity(&room->creatures,&args);

	if(playerObject != nullptr && creature != nullptr) {
		string response = creature->attack(playerObject,player);
		if(creature->hitpoints <= 0)
		{
			removeCreature(&room->creatures,creature);
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

	//chceck for objects to open in the room
	DungeonObject* thingToOpen = extractObject(&room->objects,&args);
	//failing that, check for them on the player
	if(thingToOpen == nullptr)
	{
		thingToOpen = extractObject(&player->objects,&args);
	}
	//If it is an openable thing, that is not already open, open it!
	if(thingToOpen != nullptr && thingToOpen->canOpen == true && thingToOpen->isOpen == false)
	{
		thingToOpen->isOpen = true;
		if(thingToOpen->contents.size() == 0) {
			textBuffer.push_back("You open the "+thingToOpen->getPrimaryName()+", it is empty.");		
		} else {
			textBuffer.push_back("You open the "+thingToOpen->getPrimaryName()+", inside you see");		
			showContents(&thingToOpen->contents);
		}
		return "";
	}
	else if(thingToOpen != nullptr)
	{
		return "You can't open that";
	}

	//Now check for 'doors' to open
	DungeonExit * exitToOpen = (DungeonExit*)extractEntity(&room->exits,&args);
	if(exitToOpen != nullptr && exitToOpen->isDoor && !exitToOpen->isOpen)
	{
		exitToOpen->isOpen = true;
		textBuffer.push_back(exitToOpen->openingText);
	}
	else if(exitToOpen != nullptr)
	{
		return "You try but fail.";
	}

	return "You don't see that here.";
}

void DungeonEngine::move(DungeonExit *dungeonExit)
{
	//if this is not a logical door, of it is open, go!
	if(! dungeonExit->isDoor || dungeonExit->isOpen)
	{
		room = dungeonExit->room;
		updateCmdMap();
		look();
	}
	else if(dungeonExit->isDoor && !dungeonExit->isOpen)
	{
		textBuffer.push_back(dungeonExit->closedText);
	}

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
	headerWindow = newwin(1,getCols(),0,0);
	commandWindow = newwin(1,getCols(),LINES-1,0);
	keypad(commandWindow,true);

	mainWindow = newwin(LINES-2,getCols(),1,0);
	scrollok(mainWindow,TRUE);
	getmaxyx(stdscr,h,w);

	init_pair(1,COLOR_BLACK,COLOR_RED);
	wbkgd(headerWindow,COLOR_PAIR(1));

	look();

	refresh();
	wrefresh(headerWindow);
	wrefresh(commandWindow);
	wrefresh(mainWindow);

}


void DungeonEngine::showContents(vector<DungeonObject*> *objects,int depth)
{
	string indent = "  ";
	for(int i = 0; i < depth; i++)
	{
		indent += "  ";
	}
	for(auto o: *objects){
		if(o->isOpen && o->contents.size() > 0)
		{
			textBuffer.push_back(indent+a_an(o->getPrimaryName())+ " which contains ");
			showContents(&o->contents,depth+1);
		}
		else {
			textBuffer.push_back(indent + a_an(o->getPrimaryName()));
		}

	}


}
void DungeonEngine::look()
{
	addToBuffer(&room->description);

	for(auto creature : room->creatures)
	{
		textBuffer.push_back(thereIsA(creature->getPrimaryName()));
	}

	for(auto o : room->objects)
	{
		string objString = thereIsA(o->getPrimaryName())+".";
		if(o->isOpen && o->contents.size() > 0) {
			objString += " Inside it you see ";
			textBuffer.push_back(objString);
			showContents(&o->contents);
		}
		else {
			textBuffer.push_back(objString);
		}

	}

	for(auto exit : room->exits)
	{

		if(exit->isDoor)
		{
			if(exit->isOpen)
			{
				textBuffer.push_back(exit->openText);
			}
			else
			{
				textBuffer.push_back(exit->closedText);
			}
		}
		else {
			textBuffer.push_back(exit->openText);
		}

	}

}



void DungeonEngine::render(unsigned long offset)
{
	wclear(mainWindow);


	int bufferSize = (int)textBuffer.size();
	int end = max(0,bufferSize - offset);
	int start = max(0,end-LINES);


	for(auto i = start; i < end; i++)
	{
		string entry = textBuffer[i];
		vector<string> tokens = split(entry,' ');

		int x = 0;
		for(auto s : tokens)
		{
			s = s + " ";
			if(s.length() + x < COLS)
			{
				wprintw(mainWindow,s.c_str());
				x += s.length();
			}
			else {
				wprintw(mainWindow,"\n");

				wprintw(mainWindow,s.c_str());
				x = s.length();
			}
		}
		wprintw(mainWindow,"\n");


		//dbsleep(200);
	}
	wrefresh(mainWindow);
}

void DungeonEngine::updateCmdMap()
{
	cmdMap.clear();

	cmdMap[STR_EXIT] = &DungeonEngine::exit;
	cmdMap[STR_PAGE_UP] = &DungeonEngine::pageUp;
	cmdMap[STR_PAGE_DOWN] = &DungeonEngine::pageDown;
	cmdMap[STR_USE] = &DungeonEngine::use;
	cmdMap[STR_LOOK_AT] = &DungeonEngine::examine;
	cmdMap[STR_TAKE] = &DungeonEngine::take;
	cmdMap[STR_LOOK] = &DungeonEngine::lookCmd;
	cmdMap[STR_OPEN] = &DungeonEngine::open;
	cmdMap[STR_PUT] = &DungeonEngine::put;
	cmdMap[STR_PLACE] = &DungeonEngine::put;
	cmdMap[STR_EXAMINE] = &DungeonEngine::examine;
	cmdMap[STR_DROP] = &DungeonEngine::drop;
	cmdMap[STR_INVENTORY] = &DungeonEngine::inventory;
	cmdMap[STR_I] = &DungeonEngine::inventory;

	//update movemap
	moveMap.clear();
	for(auto e : room->exits)
	{
		for(auto s : e->getLcaseNames())
		{
			moveMap[s] = e;
		}
	}



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
	renderOffset = 0;
	pageSize = LINES - 5;

	//create a map of exit names to move to
	
	resetWindows();

	CommandWindow cmdW;

	while(true) {
		updateCmdMap();
		wmove(headerWindow,0,0);
		wclrtoeol(headerWindow);
		mvwprintw(headerWindow,0,0,"Dungeon Builder");
		mvwprintwCenter(headerWindow,0,room->getPrimaryName().c_str());
		wrefresh(headerWindow);
		render(renderOffset);
		string userInput = cmdW.getCommandAsString(commandWindow,STR_PROMPT);

		if(userInput != STR_PAGE_DOWN && userInput != STR_PAGE_UP)
		{
			textBuffer.push_back(STR_PROMPT+userInput);
		}

		if(userInput.length() > 0) {


			vector<string> verbs;
			for(map<string,commandFunction>::iterator it = cmdMap.begin(); it != cmdMap.end(); ++it) {
				verbs.push_back(it->first);
			}

			string verb = extractPhrase(verbs,&userInput);

			if(verb == "") {
				vector<string> directions;
				for(map<string,DungeonExit*>::iterator it = moveMap.begin(); it != moveMap.end(); ++it) {
					directions.push_back(it->first);
				}

				string moveStr = extractPhrase(directions,&userInput);

				if(moveStr == "") {
					textBuffer.push_back("What are you doing, dave?");
				}
				else
				{
					move(moveMap[moveStr]);
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
