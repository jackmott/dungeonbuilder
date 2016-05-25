#include "dungeon_data.h"

#define TAB "\t"

using namespace std;

string DungeonRoom::toJSON()
{
	ostringstream sout;
	sout << TAB << TAB << TAB << "\"uid\":" << uid << "," << endl;
	sout << TAB << TAB << TAB << "\"name\":\"" << name << "\"," << endl;
	sout << TAB << TAB << TAB << "\"description\":\"" << description << "\"," << endl;

	sout << TAB << TAB << TAB << "\"objects\":[" << endl;
	for (auto i = 0u; i < objects.size(); i++) {
		sout << TAB << TAB << TAB << TAB << "{" << objects[i]->toJSON() << "}," << endl;
	}
	sout << TAB << TAB << TAB << "]," << endl;

	sout << TAB << TAB << TAB << "\"creatures\":[" << endl;
	for (auto i = 0u; i < creatures.size(); i++) {
		sout << TAB << TAB << TAB << TAB << "{" << creatures[i]->toJSON() << "}," << endl;
	}
	sout << TAB << TAB << TAB << "]," << endl;

	sout << TAB << TAB << TAB << "\"exits\":[" << endl;
	for (auto i = 0u; i < exits.size(); i++) {
		sout << TAB << TAB << TAB << TAB << "{ " << exits[i]->toJSON() << "}," << endl;
	}
	sout << TAB << TAB << TAB << "]" << endl;
	return sout.str();
}
string DungeonExit::toJSON()
{
	ostringstream sout;
	sout << "\"name\":\"" << name << "\", \"description\":\"" << description 
		 << "\", \"links\":" << room->uid;
	return sout.str();
}
string DungeonObject::toJSON()
{
	ostringstream sout;
	sout << "\"name\":\"" << name << "\", \"description\":\"" << description << "\"";
	return sout.str();
}
string DungeonCreature::toJSON()
{
	ostringstream sout;
	sout << "\"name\":\"" << name << "\", \"description\":\"" << description
		 << "\", \"hitpoints\":" << hitpoints << ", \"alignment\":" << alignment;
	return sout.str();
}