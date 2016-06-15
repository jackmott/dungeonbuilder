#ifndef DUNGEON_SERIALIZER_H
#define DUNGEON_SERIALIZER_H

#include <string>
#include <fstream>
#include <stack>
#include <vector>

struct DungeonObject;


//reflection, the C way
// #x expands to a string literal of the variable name
#define writeInt(x) _writeInt(#x,x)
#define writeString(x) _writeString(#x,x)
#define writeBool(x) _writeBool(#x,x)
#define writeEntity(x) _writeInt(#x,x->uid)
#define writeVectorString(x) _writeVectorString(#x,x)
//only the ids!
#define writeVectorEntity(x) _writeVectorEntity(#x,&x)

#define loadInt(x,json) x = _loadInt(#x,json)
#define loadString(x,json) x = _loadString(#x,json)
#define loadBool(x,json) x = _loadBool(#x,json)
#define loadVectorString(x,json) x = _loadVectorString(#x,json)

#define loadVectorEntity(x,json) _loadVectorEntity(#x,&x,json)


//save
std::string _writeInt(std::string name, int value);
std::string _writeString(std::string name,std::string value);
std::string _writeBool(std::string name,bool value);
std::string _writeVectorString(std::string name,std::vector<std::string> const &value);
//only the ids!
std::string _writeVectorEntity(std::string name, void *value);

//load
int _loadInt(std::string name, void* json);
std::string _loadString(std::string name, void* json);
bool _loadBool(std::string name, void* json);
std::vector<std::string> _loadVectorString(std::string name, void* json);
void _loadVectorEntity(std::string name,void* _v,void *_json);

void loadJson(std::string filename);

#endif