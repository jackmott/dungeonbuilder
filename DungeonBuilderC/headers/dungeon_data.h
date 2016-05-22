#pragma once
#include <string>
#include <vector>
#include <sstream>

using namespace std;


struct DungeonExit;
struct DungeonObject;
struct DungeonCreature;

struct DungeonRoom
{
	int uid;
	string name;
	string description;
	vector<DungeonExit*> exits;
	vector<DungeonObject*> objects;
	vector<DungeonCreature*> creatures;

	string toString();

};
struct DungeonExit
{
	string name;
	string description;
	DungeonRoom* room;

	string toString();
};

struct DungeonObject
{
	string name;
	string description;

	string toString();
};

struct DungeonCreature
{
	string name;
	string description;
	int hitpoints;
	int alignment;

	string toString();
};

struct DungeonPlayer
{
	string name;
	string description;
	int hitpoints;
	int score;
	vector<DungeonObject*> objects;
	DungeonRoom* location;
};