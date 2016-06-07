#ifndef DUNGEON_ENTITY_H
#define DUNGEON_ENTITY_H

#include <vector>
#include <string>


struct DungeonEntity
{



	std::vector <std::string> names;
	DungeonEntity* parent = nullptr;
	unsigned long uid = NULL;	
	int age=0;
	virtual void addName(std::string name);
	virtual void setPrimaryName(std::string name);
	virtual std::string  getPrimaryName() const;
	virtual std::vector<std::string> getNames() const;
	virtual std::vector<std::string> getLcaseNames() const;
	virtual std::string  toJSON();
	bool removeName(std::string name);
};

#endif
