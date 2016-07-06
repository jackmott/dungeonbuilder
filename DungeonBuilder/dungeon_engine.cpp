#include "dungeon_creature.h"
#include "dungeon_trigger.h"
#include "dungeon_effect.h"
#include "dungeon_action.h"
#include "dungeon_exit.h"
#include "dungeon_object.h"
#include "dungeon_room.h"
#include "dungeon_player.h"
#include "dungeon_engine.h"
#include "printutils.h"
#include "utils.h"
#include "command_window.h"
#include "string_constants.h"

using namespace std;

int DungeonEngine::resize(string args)
{
	resize_term(0,0);
	resetWindows();
	return 0;
}

int DungeonEngine::pageUp(string args)
{
	renderOffset++;
	return 0;
}

int DungeonEngine::pageDown(string args)
{
	if(renderOffset > 0) renderOffset--;
	return 0;
}

int DungeonEngine::inventory(string args)
{
	if(player->objects.size() == 0)
	{
		textBuffer.push_back("You are empty handed");

	}
	else
	{

		textBuffer.push_back("You are carrying");
		vector<DungeonObject*> *objects = &player->objects;
		showContents(objects);
	}
	return 0;

}

int DungeonEngine::drop(string args)
{
	DungeonObject *thing = (DungeonObject*)extractEntity(&player->objects,&args);
	if(thing != nullptr) {
		removePointer(&player->objects,thing);
		room->objects.push_back(thing);
		textBuffer.push_back("You drop the " + thing->getPrimaryName() +".");
		return 1;
	}
	else {
		textBuffer.push_back("You don't have that.");
		return 0;
	}
}

int DungeonEngine::examine(string args)
{
	DungeonObject* thing =extractObject(&room->objects,&args);
	if(thing == nullptr)
	{
		thing = extractObject(&player->objects,&args);
	}

	if(thing != nullptr && thing->description.size() == 0)
	{
		if(thing->canOpen && thing->isOpen)
		{
			textBuffer.push_back("It is open.");
		}
		else if(thing->canOpen && !thing->isOpen)
		{
			textBuffer.push_back("It is closed.");
		}
		textBuffer.push_back("You see no further detail.");

	}
	else if(thing != nullptr)
	{
		textBuffer.push_back(thing->description);
		if(thing->canOpen && thing->isOpen)
		{
			textBuffer.push_back("It is open.");
		}
		else if(thing->canOpen && !thing->isOpen)
		{
			textBuffer.push_back("It is closed.");
		}
	}
	else {
		DungeonCreature *gal = (DungeonCreature*)extractEntity(&room->creatures,&args);
		if(gal != nullptr)
		{
			if(gal->description.size() == 0)
			{
				textBuffer.push_back("There is nothing more to see.");
			}
			else {
				textBuffer.push_back(gal->description);
			}
		}
		else
		{
			textBuffer.push_back("You don't see that here.");
		}
	}
	return 0;
}

int DungeonEngine::put(string args)
{
	//get the string before and after the word "in" to clarify what is being put where
	unsigned long inLocation = args.find(" in ");
	if(inLocation == string::npos)
	{
		textBuffer.push_back("Your fumble about, but it doesn't work.");
		return 0;
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
		textBuffer.push_back("It isn't open.");
		return 0;
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
			textBuffer.push_back("You put the "+ putObject->getPrimaryName() + " in the " + containerObject->getPrimaryName() + ".");
			return 1;
		}
		else
		{
			textBuffer.push_back("You don't have that.");
			return 0;
		}
	}

	textBuffer.push_back("You flounder about, with no success.");
	return 0;
}

int DungeonEngine::take(string args)
{
	DungeonObject* takenObject = extractAndRemoveObject(&room->objects,&args);

	if(takenObject == nullptr)
	{
		takenObject = (DungeonObject*)extractEntity(&player->objects,&args);
		if(takenObject != nullptr) {
			textBuffer.push_back("You already have that.");
			return 0;
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
		textBuffer.push_back(takenObject->getPrimaryName() + " taken.");
		return 1;
	}
	else
	{
		textBuffer.push_back("You try to take it, but it seems futile");
		return 0;
	}

}
int DungeonEngine::action(string actionStr,string args)
{
	vector<string> vArgs = split(args,' ');
	vArgs = removeArticles(vArgs);

	string objectStr;
	string targetStr;

	if(vArgs.size() < 1)
	{
		textBuffer.push_back("I don't understand.");
		return 0;
	}

	ObjectTarget ot = extractObjectTarget(vArgs);
	//no preposition found?
	if(ot.object == "" && ot.target == "")
	{
		ot.object = join(0,vArgs,CHR_SPACE);
	}

	DungeonEntity * targetEntity = nullptr;
	if(ot.target != "")
	{
		targetEntity = extractObject(&player->objects,&ot.target);
		if(targetEntity == nullptr)
		{
			targetEntity = extractObject(&room->objects,&ot.target);
		}
		if(targetEntity == nullptr)
		{
			targetEntity = extractEntity(&room->creatures,&ot.target);
		}
	}

	//Check if any player objects have the action
	DungeonObject* playerObject = extractObject(&player->objects,&ot.object);
	if(playerObject != nullptr) {

		DungeonAction* action = (DungeonAction*)extractEntity(&playerObject->actions,&actionStr);
		if(action != nullptr)
		{
			string output = action->apply(&textBuffer,targetEntity,player,room,true);		
			textBuffer.push_back(output);
			return 1; //Todo maybe some actions take more than 1 turn?
		}

	}
	
	//Check if the room has the action
	DungeonAction * action = (DungeonAction*)extractEntity(&room->actions,&actionStr);
	if (action != nullptr)
	{
		string output = action->apply(&textBuffer,targetEntity,player,room,false);
		textBuffer.push_back(output);
		return 1; //Todo maybe some actions take more than 1 turn?
	}

	//Check if objects in the room ahve the action
	DungeonObject *roomObject = extractObject(&room->objects,&ot.object);
	if(roomObject != nullptr)
	{
		DungeonAction* action = (DungeonAction*)extractEntity(&roomObject->actions,&actionStr);
		if(action != nullptr && !action->needToHold)
		{
			
			string output = action->apply(&textBuffer,targetEntity,player,room,false);			
			textBuffer.push_back(output);
			return 1; //Todo maybe some actions take more than 1 turn?
		}
	}
	textBuffer.push_back("Your attempt amounts to nothing.");
	return 0;


}

int DungeonEngine::close(string args)
{

	//chceck for objects to open in the room
	DungeonObject* thingToClose = extractObject(&room->objects,&args);
	//failing that, check for them on the player
	if(thingToClose == nullptr)
	{
		thingToClose = extractObject(&player->objects,&args);
	}
	//If it is an openable thing, that is not already open, open it!
	if(thingToClose != nullptr && thingToClose->canOpen == true && thingToClose->isOpen == true)
	{
		thingToClose->isOpen = false;
		textBuffer.push_back("You close the "+thingToClose->getPrimaryName()+".");
		return 1;
	}
	else if(thingToClose != nullptr)
	{
		textBuffer.push_back("You can't close that.");
		return 0;
	}

	//Now check for 'doors' to open
	DungeonExit * exitToClose = (DungeonExit*)extractEntity(&room->exits,&args);
	if(exitToClose != nullptr && exitToClose->isDoor && exitToClose->isOpen)
	{
		exitToClose->isOpen = false;
		textBuffer.push_back(exitToClose->closingText);
		return 1;
	}
	else if(exitToClose != nullptr)
	{
		textBuffer.push_back("You try but fail.");
		return 0;
	}

	textBuffer.push_back("You don't see that here.");
	return 0;
}


int DungeonEngine::open(string args)
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
		}
		else {
			textBuffer.push_back("You open the "+thingToOpen->getPrimaryName()+", inside you see");
			showContents(&thingToOpen->contents);
		}
		return 1;
	}
	else if(thingToOpen != nullptr)
	{
		textBuffer.push_back("You can't open that");
		return 0;
	}

	//Now check for 'doors' to open
	DungeonExit * exitToOpen = (DungeonExit*)extractEntity(&room->exits,&args);
	if(exitToOpen != nullptr && exitToOpen->isDoor && !exitToOpen->isOpen)
	{
		exitToOpen->isOpen = true;
		textBuffer.push_back(exitToOpen->openingText);
		return 1;
	}
	else if(exitToOpen != nullptr)
	{
		textBuffer.push_back("You try but fail.");
		return 0;
	}

	textBuffer.push_back("You don't see that here.");
	return 0;
}

void DungeonEngine::move(DungeonExit *dungeonExit)
{
	//if this is not a logical door, of it is open, go!
	if(! dungeonExit->isDoor || dungeonExit->isOpen)
	{
		textBuffer.push_back(dungeonExit->travelText);
		room = dungeonExit->room;
		turns++;
		updateCmdMap();
		look();
	}
	else if(dungeonExit->isDoor && !dungeonExit->isOpen)
	{
		textBuffer.push_back(dungeonExit->closedText);
	}

}


int DungeonEngine::lookCmd(string args)
{
	look();
	return 0;
}


void DungeonEngine::clearWindows()
{
	delwin(commandWindow);
	delwin(mainWindow);
	delwin(headerWindow);
	clear();
}




void DungeonEngine::resetWindows()
{
	headerWindow = newwin(1,COLS,0,0);
	commandWindow = newwin(1,COLS,LINES-1,0);
	mainWindow = newwin(LINES-2,COLS,1,0);

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
		if(o->ontops.size() > 0)
		{
			textBuffer.push_back(indent+a_an("on top of the "+o->getPrimaryName()+" you see " ));
			showContents(&o->ontops,depth+1);
		}
		else if (o->ontops.size() == 0 && (!o->isOpen || o->contents.size() ==0)) {
			textBuffer.push_back(indent + a_an(o->getPrimaryName()));
		}

	}


}
void DungeonEngine::look()
{
	bool isThereAnyLight = false;
	if(room->hasLight) isThereAnyLight = true;
	if(!isThereAnyLight) {
		for(auto o : player->objects)
		{
			if(o->isLight) {
				isThereAnyLight = true;
				break;
			}
		}
	}
	if(!isThereAnyLight)
	{
		for(auto o : room->objects)
		{
			if(o->isLight) {
				isThereAnyLight = true;
				break;
			}
		}
	}

	if(isThereAnyLight)
	{
		textBuffer.push_back(room->description);
		for(auto creature : room->creatures)
		{
			textBuffer.push_back(thereIsA(creature->getPrimaryName()));
		}

		for(auto o : room->objects)
		{
			string objString = thereIsA(o->getPrimaryName())+".";
			bool thereIsA = false;
			if(o->isOpen && o->contents.size() > 0) {
				thereIsA = true;
				objString += " Inside it you see ";
				textBuffer.push_back(objString);
				showContents(&o->contents);
			}
			if(o->ontops.size() > 0)
			{
				if (thereIsA)
					objString = " On the "+o->getPrimaryName()+" you see";
				else 
					objString += "On it you see";
				textBuffer.push_back(objString);
				showContents(&o->ontops);
			}
			
			if (o->ontops.size() == 0 && (!o->isOpen || o->contents.size() == 0)){
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
		}
	}
	else {
		textBuffer.push_back("All you see is darkness....");
	}



}



void DungeonEngine::render(unsigned long offset)
{
	wclear(mainWindow);

	renderDungeonText(mainWindow,parseDungeonText(textBuffer),offset);

	wrefresh(mainWindow);
}

void DungeonEngine::updateCmdMap()
{
	cmdMap.clear();

	cmdMap[STR_PAGE_UP] = &DungeonEngine::pageUp;
	cmdMap[STR_PAGE_DOWN] = &DungeonEngine::pageDown;
	cmdMap[STR_LOOK_AT] = &DungeonEngine::examine;
	cmdMap[STR_TAKE] = &DungeonEngine::take;
	cmdMap[STR_LOOK] = &DungeonEngine::lookCmd;
	cmdMap[STR_OPEN] = &DungeonEngine::open;
	cmdMap[STR_CLOSE] = &DungeonEngine::close;
	cmdMap[STR_PUT] = &DungeonEngine::put;
	cmdMap[STR_PLACE] = &DungeonEngine::put;
	cmdMap[STR_EXAMINE] = &DungeonEngine::examine;
	cmdMap[STR_DROP] = &DungeonEngine::drop;
	cmdMap[STR_INVENTORY] = &DungeonEngine::inventory;
	cmdMap[STR_I] = &DungeonEngine::inventory;
	cmdMap[STR_KEY_RESIZE] = &DungeonEngine::resize;

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
	actionMap.clear();
	for(auto o : player->objects)
	{
		for(auto action : o->actions)
		{
			for(auto name : action->getLcaseNames())
			{
				actionMap[name] = &DungeonEngine::action;
			}
		}
	}
	for (auto action : room->actions)
	{
		for(auto name : action->getLcaseNames())
		{
			actionMap[name] = &DungeonEngine::action;
		}
	}
	for(auto o : room->objects)
	{
		for(auto action : o->actions)
		{
			for(auto name : action->getLcaseNames())
			{
				actionMap[name] = &DungeonEngine::action;
			}
		}
	}
}


void DungeonEngine::gameLogic(int turnsUsed)
{
	if(turnsUsed > 0)
	{
		turns += turnsUsed;
		updatePhysicalObjects();
	}

}

// Iterate over all physically present objects in the game world
// Do all the things
// ie. On the player, or in a room, 
void DungeonEngine::updatePhysicalObjects()
{
	//For now, we adjust age, and check triggers

	// All player objects
	vector<DungeonObject*> objects = getAllPlayerObjects(player);
	for(auto o: objects) {
		o->age++;
		checkTriggers(o);
	}

	// All objects in room
	for(auto r : globalState.roomList)
	{
		vector<DungeonObject*> roomObjects = getAllRoomObjects(r);
		for(auto o : roomObjects)
		{
			o->age++;
			checkTriggers(o);
		}

	}

	// Todo: expand to creatures as well?
}




// Check all object's triggers
void DungeonEngine::checkTriggers(DungeonObject* o)
{


	for(auto t : o->triggers)
	{
		bool isTriggered = false;
		switch(t->type)
		{
		case TRIGGER_TYPE::Evil:
			isTriggered = t->checkForEvil(room,t->magnitude);
			break;
		case TRIGGER_TYPE::Age:
			isTriggered = t->checkAge();
			break;
		default:
			textBuffer.push_back("unhandled trigger type");
			break;
		}
		//If the trigger was triggered, push the output and apply the effects
		if(isTriggered)
		{
			textBuffer.push_back(t->output);
			for(auto e : t->effects)
			{
				//TODO deal with Target
				e->apply(&textBuffer,nullptr,player,room,true);
			}

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
	look();
	CommandWindow cmdW;

	while(true) {
		updateCmdMap();
		wmove(headerWindow,0,0);
		wclrtoeol(headerWindow);
		printEngineHeader(headerWindow,"Dungeon Builder",room->getPrimaryName(),to_string(turns));
		wrefresh(headerWindow);


		render(renderOffset);
		string userInput = cmdW.getCommandAsString(commandWindow,STR_PROMPT);
		string originalUserInput = userInput;
		if(userInput == STR_EXIT) break;
		if(userInput != STR_PAGE_DOWN && userInput != STR_PAGE_UP && userInput != STR_KEY_RESIZE)
		{
			textBuffer.push_back(STR_PROMPT+userInput);
		}

		bool didInputCauseAnyAction = false;
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

				if(moveStr != "")
				{
					move(moveMap[moveStr]);
					gameLogic(1);
					didInputCauseAnyAction = true;
				}
			}
			else
			{
				commandFunction cmdFunc = cmdMap[verb];
				int turnsUsed = (this->*cmdFunc)(userInput);
				gameLogic(turnsUsed);
				didInputCauseAnyAction = true;
			}

			//Apply actions, even if other things have been applied
			vector<string> actions;
			for(map<string,ActionFunction>::iterator it = actionMap.begin(); it != actionMap.end(); ++it) {
				actions.push_back(it->first);
			}
			string actionStr = extractPhrase(actions,&originalUserInput);			
			

			if (actionStr != "") {
				ActionFunction actFunc = actionMap[actionStr];
				int turnsUsed = (this->*actFunc)(actionStr,userInput);
				gameLogic(turnsUsed);
				didInputCauseAnyAction = true;
			}

			if(!didInputCauseAnyAction)
			{
				textBuffer.push_back("I don't understand...");
			}

		}

	}


	clearWindows();
}
