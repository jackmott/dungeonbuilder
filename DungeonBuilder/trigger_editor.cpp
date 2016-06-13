#include "dungeon_effect.h"
#include "dungeon_entity.h"
#include "trigger_editor.h"
#include "command_window.h"
#include "text_editor.h"
#include "printutils.h"
#include "utils.h"
#include "string_constants.h"
#include "effect_editor.h"
using namespace std;

string TriggerEditor::exit(vector<string> args)
{
	return STR_EXIT;
}

string TriggerEditor::del(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to delete?";
	}
	string delNoun = args[1];
	toLower(&delNoun);

	resetWindows();
	return "";
}

string TriggerEditor::set(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to set?";
	}
	if(args.size() < 3) {
		return "Please supply the value directly in the command.";
	}
	string editNoun = args[1];
	toLower(&editNoun);

	if(editNoun == STR_TEXT_OUTPUT)
	{
		string output = join(2,args,CHR_SPACE);
		trigger->output = output;
	}
	else if(editNoun == STR_TYPE)
	{
		string typeStr = toLower(join(2,args,CHR_SPACE));
		for(int i =0 ; i <= ARRAYSIZE(TRIGGER_STRS) ;i++){
			string lcase = toLower(TRIGGER_STRS[i]);
			if(lcase == typeStr)
			{
				trigger->type = (TRIGGER_TYPE)i;
			}
		}
	}
	else if(editNoun == STR_MAGNITUDE)
	{
		string magStr = args[2];
		int mag = stoi(magStr,nullptr,10);
		trigger->magnitude = mag;
	}
	else if(editNoun == STR_NEED_HELD)
	{
		trigger->needToHold = isAffirmative(args[2]);
	}
	resetWindows();
	return "";
}


string TriggerEditor::edit(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to edit?";
	}
	string editNoun = args[1];
	toLower(&editNoun);


	clearWindows();
	resetWindows();
	return "";
}

string TriggerEditor::add(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to add?";
	}
	string addNoun = args[1];
	toLower(&addNoun);

	if(addNoun == STR_EFFECT)
	{
		EffectEditor ed;
		DungeonEffect *e = new DungeonEffect();

		if(args.size() > 2) {

			string typeStr = toLower(join(2,args,CHR_SPACE));
			for(int i =0 ; i <= ARRAYSIZE(EFFECT_STRS) ;i++){
				string lcase = toLower(EFFECT_STRS[i]);
				if(lcase == typeStr)
				{
					e->type = (EFFECT_TYPE)i;
					break;
				}
			}

		}
		trigger->effects.push_back(e);
		e->parent = trigger;
		ed.load(e);
	}

	clearWindows();
	resetWindows();
	return "";
}


void TriggerEditor::clearWindows()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
	delwin(headerWindow);
}


void TriggerEditor::resetWindows()
{
	commandWindow = newwin(1,COLS,LINES-1,0);
	responseWindow = newwin(1,COLS,LINES-2,0);
	mainWindow = newwin(LINES-3,COLS,1,0);
	headerWindow = newwin(1,COLS,0,0);

	getmaxyx(stdscr,h,w); // this doesn't work in windows
	refresh();

	wrefresh(commandWindow);
	wrefresh(responseWindow);
	wrefresh(mainWindow);
	wrefresh(headerWindow);

	string command;


	printHeader(headerWindow,trigger->parent->parent->getPrimaryName(),trigger->parent->getPrimaryName(),"Trigger:"+trigger->getPrimaryName(),3);

	int lineCount = 2;

	setcolor(mainWindow,COLOR_WHITE);


	string typeRow = STR_MENU_TYPE + trigger->getPrimaryName();
	mvwprintw(mainWindow,lineCount,0,typeRow.c_str());

	lineCount++;
	string outputRow = STR_MENU_TEXT_OUTPUT + trigger->output;
	mvwprintw(mainWindow,lineCount,0,outputRow.c_str());

	lineCount++;
	string torf = trigger->needToHold ? STR_TRUE : STR_FALSE;
	string holdRow = STR_MENU_NEED_HOLD + torf;
	mvwprintw(mainWindow,lineCount,0,holdRow.c_str());

	lineCount++;
	string magnitudeRow = STR_MENU_MAGNITUDE + to_string(trigger->magnitude);
	mvwprintw(mainWindow,lineCount,0,magnitudeRow.c_str());


	lineCount++;
	mvwprintw(mainWindow,lineCount,0,STR_MENU_EFFECT);
	for(auto e : trigger->effects)
	{
		lineCount++;
		mvwprintw(mainWindow,lineCount,2,e->getPrimaryName().c_str());
	}

	wrefresh(mainWindow);

}

void TriggerEditor::load(DungeonTrigger* _trigger)
{
	trigger = _trigger;
	cmdMap[STR_EDIT] = &TriggerEditor::edit;
	cmdMap[STR_EXIT] = &TriggerEditor::exit;
	cmdMap[STR_ADD] = &TriggerEditor::add;
	cmdMap[STR_SET] = &TriggerEditor::set;
	cmdMap[STR_DELETE] = &TriggerEditor::del;
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





