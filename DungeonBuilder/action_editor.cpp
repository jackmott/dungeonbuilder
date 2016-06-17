#include "dungeon_effect.h"
#include "action_editor.h"
#include "dungeon_action.h"
#include "command_window.h"
#include "text_editor.h"
#include "printutils.h"
#include "utils.h"
#include "string_constants.h"
#include "effect_editor.h"
using namespace std;


string ActionEditor::exit(vector<string> args)
{
	return STR_EXIT;
}

string ActionEditor::del(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to delete?";
	}
	string delNoun = args[1];
	toLower(&delNoun);
	if(delNoun == STR_NAME)
	{
		if(args.size() < 3)
		{
			return "Provide a name to delete.";
		}
		string name = join(2,args,CHR_SPACE);
		if(!action->removeName(name)) {
			return "You can't.";
		}
	}
	resetWindows();
	return "";
}

string ActionEditor::set(vector<string> args)
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
	if(editNoun == STR_NAME)
	{
		string newname = join(2,args,CHR_SPACE);
		action->setPrimaryName(newname);
	}
	else if(editNoun == STR_TEXT_OUTPUT)
	{
		string output = join(2,args,CHR_SPACE);
		action->output = output;
	}
	else if(editNoun == STR_NEED_HELD)
	{
		action->needToHold = isAffirmative(args[2]);
	}
	resetWindows();
	return "";
}


string ActionEditor::edit(vector<string> args)
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

string ActionEditor::add(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to add?";
	}
	string addNoun = args[1];
	toLower(&addNoun);
	if(addNoun == STR_NAME)
	{
		if(args.size() < 3)
		{
			return "Provide a name to add please.";
		}
		string name = join(2,args,CHR_SPACE);
		action->addName(name);
	}
	else if(addNoun == STR_EFFECT)
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

		action->effects.push_back(e);
		e->parent = action;
		ed.load(e);
	}

	clearWindows();
	resetWindows();
	return "";
}


void ActionEditor::clearWindows()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
	delwin(headerWindow);
}


void ActionEditor::resetWindows()
{
	commandWindow = newwin(1,COLS,LINES-1,0);
	responseWindow = newwin(1,COLS,LINES-2,0);
	mainWindow = newwin(LINES-3,COLS,1,0);
	headerWindow = newwin(1,COLS,0,0);
	textBuffer.clear();
	refresh();

	wrefresh(commandWindow);
	wrefresh(responseWindow);
	wrefresh(mainWindow);
	wrefresh(headerWindow);

	string command;


	printHeader(headerWindow,action->parent->parent->getPrimaryName(),action->parent->getPrimaryName(),"ACTION:"+action->getPrimaryName(),3);



	setcolor(mainWindow,COLOR_WHITE);

	string nameRow = STR_MENU_NAME + join(0,action->getNames(),',');
	textBuffer.push_back(nameRow);
	

	
	string outputRow = STR_MENU_TEXT_OUTPUT + action->output;
	textBuffer.push_back(outputRow);

	
	string torf = action->needToHold ? STR_TRUE : STR_FALSE;
	string holdRow = STR_MENU_NEED_HOLD + torf;
	textBuffer.push_back(holdRow);
		
	textBuffer.push_back(STR_MENU_EFFECT);
	for(auto e : action->effects)
	{
		textBuffer.push_back("  "+e->getPrimaryName());		
	}

	renderTextBuffer();
	wrefresh(mainWindow);

}

void ActionEditor::load(DungeonAction* _action)
{
	action = _action;
	cmdMap[STR_EDIT] = &ActionEditor::edit;
	cmdMap[STR_EXIT] = &ActionEditor::exit;
	cmdMap[STR_ADD] = &ActionEditor::add;
	cmdMap[STR_SET] = &ActionEditor::set;
	cmdMap[STR_DELETE] = &ActionEditor::del;
	resetWindows();

	CommandWindow cmdW;
	bool cmdFound = false;
	vector<string> cmd;
	while(true) {
		cmd = cmdW.getCommand(commandWindow,STR_PROMPT);
		if(cmd.size() > 0) {
			toLower(&cmd[0]);
			if (checkCommonInput(cmd[0])) continue;
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
			if(response.length() > 0 && response != STR_PAGE_UP && response != STR_PAGE_DOWN) {
				cmd.clear();
				mvwprintw(responseWindow,0,0,response.c_str());
				wclrtoeol(responseWindow);
				wrefresh(responseWindow);
			}
		}
	}


	clearWindows();
}





