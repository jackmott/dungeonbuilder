#include "headers/dungeon_data.h"

#define TAB "\t"

using namespace std;

string DungeonRoom::toString()
{
	ostringstream sout;
	sout << "{" << endl;
	sout << TAB << TAB << TAB << "\"ID\":\"" << uid << "\"," << endl;
	sout << TAB << TAB << TAB << "\"Name\":\"" << name << "\"," << endl;
	sout << TAB << TAB << TAB << "\"Description\":\"" << description << "\"," << endl;

	sout << TAB << TAB << TAB << "\"Objects\":[" << endl;
	for (int i = 0; i < objects.size(); i++) {
		sout << TAB << TAB << TAB << TAB << "{" << objects[i]->toString() << "}," << endl;
	}
	sout << TAB << TAB << TAB << "]," << endl;

	sout << TAB << TAB << TAB << "\"Creatures\":[" << endl;
	for (int i = 0; i < creatures.size(); i++) {
		sout << TAB << TAB << TAB << TAB << "{" << creatures[i]->toString() << "}," << endl;
	}
	sout << TAB << TAB << TAB << "]," << endl;

	sout << TAB << TAB << TAB << "\"Exits\":[" << endl;
	for (int i = 0; i < exits.size(); i++) {
		sout << TAB << TAB << TAB << TAB << "{" << exits[i]->toString() << "}," << endl;
	}
	sout << TAB << TAB << TAB << "]" << endl;
	sout << TAB << TAB << "}" << endl;
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