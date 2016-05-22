#include "headers/dungeon_data.h"

#define TAB "\t"

using namespace std;

string DungeonRoom::toJSON()
{
	ostringstream sout;
	sout << TAB << TAB << TAB << "\"ID\":\"" << uid << "\"," << endl;
	sout << TAB << TAB << TAB << "\"Name\":\"" << name << "\"," << endl;
	sout << TAB << TAB << TAB << "\"Description\":\"" << description << "\"," << endl;

	sout << TAB << TAB << TAB << "\"Objects\":[" << endl;
	for (auto i = 0u; i < objects.size(); i++) {
		sout << TAB << TAB << TAB << TAB << "{" << objects[i]->toJSON() << "}," << endl;
	}
	sout << TAB << TAB << TAB << "]," << endl;

	sout << TAB << TAB << TAB << "\"Creatures\":[" << endl;
	for (auto i = 0u; i < creatures.size(); i++) {
		sout << TAB << TAB << TAB << TAB << "{" << creatures[i]->toJSON() << "}," << endl;
	}
	sout << TAB << TAB << TAB << "]," << endl;

	sout << TAB << TAB << TAB << "\"Exits\":[" << endl;
	for (auto i = 0u; i < exits.size(); i++) {
		sout << TAB << TAB << TAB << TAB << "{" << exits[i]->toJSON() << "}," << endl;
	}
	sout << TAB << TAB << TAB << "]" << endl;
	return sout.str();
}
string DungeonExit::toJSON()
{
	ostringstream sout;
	sout << "\"Name\":\"" << name << "\", \"Description\":\"" << description 
		 << "\", \"links\":" << room->uid;
	return sout.str();
}
string DungeonObject::toJSON()
{
	ostringstream sout;
	sout << "\"Name\":\"" << name << "\", \"Description\":\"" << description << "\"";
	return sout.str();
}
string DungeonCreature::toJSON()
{
	ostringstream sout;
	sout << "\"Name\":\"" << name << "\", \"Description\":\"" << description
		 << "\", \"hitpoints\":" << hitpoints << ", \"Alignment\":" << alignment;
	return sout.str();
}