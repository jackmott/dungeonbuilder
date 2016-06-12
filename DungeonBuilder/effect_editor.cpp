#include "object_list.h"
#include "dungeon_action.h"
#include "dungeon_effect.h"
#include "dungeon_object.h"
#include "effect_editor.h"
#include "object_editor.h"
#include "command_window.h"
#include "text_editor.h"
#include "printutils.h"
#include "utils.h"
#include "string_constants.h"

using namespace std;
extern vector<DungeonObject*> g_objectList;

string EffectEditor::exit(vector<string> args)
{
	return STR_EXIT;
}

string EffectEditor::del(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to delete?";
	}
	string delNoun = args[1];
	toLower(&delNoun);
	if(delNoun == STR_OBJECT)
	{
		if(args.size() <3)
		{
			return "Which object do you want to delete?";
		}
		string objStr = join(2,args,CHR_SPACE);
		DungeonObject *o = (DungeonObject*)extractEntity(&effect->transforms,&objStr);
		if(o != nullptr)
		{
			removePointer(&effect->transforms,o);
			delete o;
			resetWindows();
			return "";
		}
		else
		{
			return "I don't see that here.";
		}
	}
	resetWindows();
	return "";
}

string EffectEditor::set(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to set?";
	}

	string editNoun = args[1];
	toLower(&editNoun);

	if(editNoun == STR_MAGNITUDE)
	{
		if(args.size() < 3) {
			return "Please supply the value directly in the command.";
		}
		string magStr = args[2];
		int mag = stoi(magStr,nullptr,10);
		effect->magnitude = mag;
	}
	else if(editNoun == STR_TYPE)
	{
		if(args.size() < 3) {
			return "Please supply the value directly in the command.";
		}
		string typeStr = toLower(join(2,args,CHR_SPACE));
		for(int i =0 ; i <= ARRAYSIZE(EFFECT_STRS) ;i++){
			string lcase = toLower(EFFECT_STRS[i]);
			if(lcase == typeStr)
			{
				effect->type = (EFFECT_TYPE)i;
				break;
			}
		}
	}
	else if(editNoun == STR_OBJECT)
	{
		DungeonObjectList ol;
		DungeonObject* o = ol.load(g_objectList);
		if(o != nullptr)
		{
			effect->transforms.push_back(o);
		}
		else {
			return "ERROR GOT BAD INFO BACK FROM THE OBJECT LIST";
		}
	}

	resetWindows();
	return "";
}


string EffectEditor::edit(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to edit?";
	}
	string editNoun = args[1];
	toLower(&editNoun);

	if(editNoun == STR_OBJECT)
	{
		if(args.size() <3)
		{
			return "Which object do you want to edit?";
		}
		string objStr = join(2,args,CHR_SPACE);
		DungeonObject *o = (DungeonObject*)extractEntity(&effect->transforms,&objStr);
		if(o != nullptr)
		{
			ObjectEditor ed;
			ed.load(o);
		}
		else
		{
			return "I don't see that here.";
		}
	}
	clearWindows();
	resetWindows();
	return "";
}

string EffectEditor::add(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to add?";
	}
	string addNoun = args[1];
	toLower(&addNoun);

	if(addNoun == STR_OBJECT)
	{
		ObjectEditor oe;
		DungeonObject* o = new DungeonObject();
		o->parent = effect->parent;
		o->addName(join(2,args,CHR_SPACE));
		clearWindows();
		oe.load(o);
		effect->transforms.push_back(o);
		resetWindows();

		return "";
	}

	clearWindows();
	resetWindows();
	return "";
}


void EffectEditor::clearWindows()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
	delwin(headerWindow);
}


void EffectEditor::resetWindows()
{
	commandWindow = newwin(1,getCols(),LINES-1,0);
	responseWindow = newwin(1,getCols(),LINES-2,0);
	mainWindow = newwin(LINES-3,getCols(),1,0);
	headerWindow = newwin(1,getCols(),0,0);

	getmaxyx(stdscr,h,w); // this doesn't work in windows
	refresh();

	wrefresh(commandWindow);
	wrefresh(responseWindow);
	wrefresh(mainWindow);
	wrefresh(headerWindow);

	string command;


	printHeader(headerWindow,effect->parent->parent->getPrimaryName(),effect->parent->getPrimaryName(),"EFFECT:"+effect->getPrimaryName(),3);

	int lineCount = 2;

	setcolor(mainWindow,COLOR_WHITE);
	string typesRow = "| ";
	for(auto s : EFFECT_STRS)
	{
		typesRow += s + " | ";
	}
	mvwprintwCenter(mainWindow,lineCount,typesRow.c_str());

	lineCount++;
	string typeRow = STR_MENU_TYPE + effect->getPrimaryName();
	mvwprintw(mainWindow,lineCount,0,typeRow.c_str());

	if(effect->type == EFFECT_TYPE::Heal || effect->type == EFFECT_TYPE::Damage)
	{
		lineCount++;
		string magnitudeRow = STR_MENU_MAGNITUDE + to_string(effect->magnitude);
		mvwprintw(mainWindow,lineCount,0,magnitudeRow.c_str());
	}

	if(effect->type == EFFECT_TYPE::Transform)
	{
		lineCount++;
		mvwprintw(mainWindow,lineCount,0,STR_MENU_SET_ADD_OBJECT);
		for(auto e : effect->transforms)
		{
			lineCount++;
			mvwprintw(mainWindow,lineCount,2,e->getPrimaryName().c_str());
		}
	}
	wrefresh(mainWindow);

}

void EffectEditor::load(DungeonEffect* _effect)
{
	effect = _effect;
	cmdMap[STR_EDIT] = &EffectEditor::edit;
	cmdMap[STR_EXIT] = &EffectEditor::exit;
	cmdMap[STR_ADD] = &EffectEditor::add;
	cmdMap[STR_SET] = &EffectEditor::set;
	cmdMap[STR_DELETE] = &EffectEditor::del;

	resetWindows();

	CommandWindow cmdW;
	bool cmdFound = false;
	vector<string> cmd;
	while(true) {
		cmd = cmdW.getCommand(commandWindow,STR_PROMPT);
		if(cmd.size() > 0) {
			toLower(&cmd[0]);
			cmdFound = cmdMap.count(cmd[0]) > 0;
		}
		if(!cmdFound) {
			cmd.clear();
			mvwprintw(responseWindow,0,0,"What are you doing, dave?");
			wclrtoeol(responseWindow);
			wrefresh(responseWindow);
		}
		else
		{
			if(cmd[0] == STR_EXIT) break;
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


	clearWindows();
}





