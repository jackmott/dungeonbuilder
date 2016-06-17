#ifndef DUNGEON_ENTITY_H
#define DUNGEON_ENTITY_H

#include <vector>
#include <string>


enum class ENTITY_TYPE 
{ 
	Object,
	Creature,
	Room,
	Exit,
	Action,
	Trigger,
	Effect
};

struct DungeonEntity
{
	ENTITY_TYPE entityType;
	std::vector <std::string> names;
	std::string article; //a vs an
	DungeonEntity* parent = nullptr;
	unsigned long uid = NULL;	
	int age=0;	
	virtual void fixUpPointers();
	virtual void addName(std::string name);
	virtual void setPrimaryName(std::string name);
	virtual std::string  getPrimaryName() const;
	virtual std::vector<std::string> getNames() const;
	virtual std::vector<std::string> getLcaseNames() const;
	virtual std::string  toJSON();
	void guessArticle();
	bool removeName(std::string name);
};

#endif
