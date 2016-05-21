#pragma once
#include <string>
#include <vector>

using namespace std;


struct DungeonExit;
struct DungeonObject;
struct DungeonCreature;

struct DungeonRoom
{
	string name;
	string description;
	vector<DungeonExit> exits;
	vector<DungeonObject> objects;
	vector<DungeonCreature> creatures;

};
struct DungeonExit
{
	string name;
	string description;
	DungeonRoom room;
};

struct DungeonObject
{
	string name;
	string description;
};

struct DungeonCreature
{
	string name;
	string description;
	int hitpoints;
	int alignment;
};
