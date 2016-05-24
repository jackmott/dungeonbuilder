#include "utils.h"
#include <sstream>
#include <algorithm>

vector<string> &split(const string &s,char delim,vector<string> &elems) {
	stringstream ss(s);
	string item;
	while(getline(ss,item,delim)) {
		elems.push_back(item);
	}
	return elems;
}

vector<string> removeArticles(vector<string> words)
{
	vector<string> result;
	for(auto word: words)
	{
		string lowerWord = word;
		toLower(&lowerWord);

		if(lowerWord != "a" && lowerWord != "the")
		{
			result.push_back(word);
		}
	}
	return result;
}

bool containsWith(vector<string> words)
{

	for(auto word: words)
	{
		string lowerWord = word;
		toLower(&lowerWord);
		if(lowerWord == "with") return true;
	}
	return false;
}


vector<string> split(const string &s,char delim) {
	vector<string> elems;
	split(s,delim,elems);
	return elems;
}

bool isVowel(char c)
{
	switch(c)
	{
	case 'a':
	case 'e':
	case 'i':
	case 'o':
	case 'u':
		return true;
	default:
		return false;
	}

}

string a_an (string s)
{
	string lowerS = s;
	toLower(&lowerS);
	if(isVowel(lowerS[0]))
	{
		return "an "+s;
	}
	else {
		return "a "+s;
	}
}

string join(unsigned int offset,vector<string> &v,string delim)
{
	string result;
	for(auto i = offset; i < v.size();i++)
	{
		result = result + v[i];
		if(i < v.size()-1)
		{
			result = result + delim;
		}
	}
	return result;
}

void toLower(string *s)
{
	transform(s->begin(),s->end(),s->begin(),::tolower);
}

bool isAffirmative(string s)
{
	string l = s;
	toLower(&l);

	if (l == "t" || l=="y" || l=="yes" || l == "true") return true;
	return false;
}