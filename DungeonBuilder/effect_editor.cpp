#include "entity_list.h"
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
extern GlobalState globalState;

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
		DungeonObject *o = (DungeonObject*)extractEntity(&effect->removeTransforms,&objStr);
		if(o != nullptr)
		{
			removePointer(&effect->removeTransforms,o);
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
	else if (editNoun == STR_DESC || editNoun == STR_DESCRIPTION)
	{
		if (args.size() < 3) {
			return "Please supply value directly in the command.";
		}		
		effect->modificationString = join(2,args,CHR_SPACE);
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
		EntityList el;
		

		DungeonEntity* room = effect->parent;
		while (room->entityType != ENTITY_TYPE::Room)
		{
			room = room->parent;
		}

		DungeonEntity* e = el.load((DungeonRoom*)room);
		if(e != nullptr)
		{
			effect->removeTransforms.push_back(e);
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
		DungeonObject *o = (DungeonObject*)extractEntity(&effect->removeTransforms,&objStr);
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
	else if(editNoun == STR_DESCRIPTION || editNoun == STR_DESC)
	{
		TextEditor ed;
		effect->modificationString = ed.edit("Replacement Description For:"+effect->parent->getPrimaryName(),effect->modificationString);
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
		effect->removeTransforms.push_back(o);
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

	printHeader(headerWindow,"Effect",effect->parent->parent->getPrimaryName(),effect->parent->getPrimaryName(),effect->getPrimaryName(),3);

	setcolor(mainWindow,COLOR_WHITE);
	string typesRow = "| ";
	for(auto s : EFFECT_STRS)
	{
		typesRow += s + " | ";
	}
	textBuffer.push_back(typesRow);
		
	string typeRow = STR_MENU_TYPE + effect->getPrimaryName();
	textBuffer.push_back(typeRow);

	if(effect->type == EFFECT_TYPE::Heal || effect->type == EFFECT_TYPE::Damage)
	{		
		string magnitudeRow = STR_MENU_MAGNITUDE + to_string(effect->magnitude);
		textBuffer.push_back(magnitudeRow);
	}

	if(effect->type == EFFECT_TYPE::Transform)
	{
		textBuffer.push_back(STR_MENU_SET_ADD_OBJECT);		
		for(auto e : effect->removeTransforms)
		{
			textBuffer.push_back("  "+e->getPrimaryName());			
		}
	}

	if (effect->type == EFFECT_TYPE::ReplaceRoomDesc)
	{
		textBuffer.push_back(STR_MENU_REPLACE_ROOM_DESC + effect->modificationString);		
	}
	renderTextBuffer();
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





