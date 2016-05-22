#include "headers/dungeon_data.h"

#define TAB "\t"

using namespace std;

string DungeonRoom::toString()
{
	ostringstream sout;
	sout << TAB << "ID: " << uid << endl;
	sout << TAB << "Name: " << name << endl;
	sout << TAB << "Description: " << description << endl;

	sout << TAB << "--ROOM_EXITS--" << endl;
	for (int i = 0; i < exits.size(); i++) {
		sout << TAB << TAB << "-EXIT-" << endl;
		sout << TAB << TAB << exits[i]->toString();
		sout << TAB << "-END_EXIT-";
	}

	sout << TAB << "--ROOM_OBJECTS--" << endl;
	for (int i = 0; i < objects.size(); i++) {
		sout << TAB << TAB << "-OBJECT-" << endl;
		sout << TAB << TAB << objects[i]->toString();
		sout << TAB << "-END_OBJECT-";
	}

	sout << TAB << "--ROOM_CREATURES--" << endl;
	for (int i = 0; i < creatures.size(); i++) {
		sout << TAB << TAB << "-CREATURE-" << endl;
		sout << TAB << TAB << creatures[i]->toString();
		sout << TAB << "-END_CREATURE-";
	}

	return sout.str();
}
string DungeonExit::toString()
{
	ostringstream sout;
	return sout.str();
}
string DungeonObject::toString()
{
	ostringstream sout;
	return sout.str();
}
string DungeonCreature::toString()
{
	ostringstream sout;
	return sout.str();
}