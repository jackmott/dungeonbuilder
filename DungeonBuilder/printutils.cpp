#include "printutils.h"
#include "utils.h"
#include "string_constants.h"
#include <map>
#include <Windows.h>

using namespace std;


void mvwprintwCenter (WINDOW * window,int row,string text)
{	
	mvwprintw(window,row,(COLS-text.length())/2,text.c_str());
}


void mvwprintwBold (WINDOW *window,int row,int col,string text)
{
	wattron(window,A_BOLD);
	mvwprintw(window,row,col,text.c_str());
	wattroff(window,A_BOLD);
}

void mvwprintwCenterBold (WINDOW *window,int row,string text)
{
	wattron(window,A_BOLD);
	mvwprintw(window,row,(COLS-text.length())/2,text.c_str());
	wattroff(window,A_BOLD);
}


#define DUNGEON_HEADER_BG COLOR_RED
#define DUNGEON_HEADER_FG COLOR_BLACK
#define DUNGEON_HEADER_FG_BOLD COLOR_WHITE

void printHeader(WINDOW *window,string leftText,string centerText,string rightText,int boldIndex)
{

	setbackground(window,DUNGEON_HEADER_FG,DUNGEON_HEADER_BG);
	wclrtoeol(window);

	centerText = STR_RIGHT_ARROW + centerText + STR_RIGHT_ARROW;


	int startX = (COLS - (leftText.size()+rightText.size()+centerText.size()))/2;

	if(boldIndex == 1)
	{
		setcolors(window,DUNGEON_HEADER_FG_BOLD,DUNGEON_HEADER_BG);
		mvwprintwBold(window,0,startX,leftText.c_str());
	}
	else
	{
		setcolors(window,DUNGEON_HEADER_FG,DUNGEON_HEADER_BG);
		mvwprintw(window,0,startX,leftText.c_str());
	}
	startX += leftText.size();

	if(boldIndex == 2)
	{
		setcolors(window,DUNGEON_HEADER_FG_BOLD,DUNGEON_HEADER_BG);
		mvwprintwBold(window,0,startX,centerText.c_str());
	}
	else
	{
		setcolors(window,DUNGEON_HEADER_FG,DUNGEON_HEADER_BG);
		mvwprintw(window,0,startX,centerText.c_str());
	}
	startX += centerText.size();
	setcolors(window,DUNGEON_HEADER_FG,DUNGEON_HEADER_BG);
	if(boldIndex == 3)
	{
		setcolors(window,DUNGEON_HEADER_FG_BOLD,DUNGEON_HEADER_BG);
		mvwprintwBold(window,0,startX,rightText.c_str());
	}
	else
	{
		setcolors(window,DUNGEON_HEADER_FG,DUNGEON_HEADER_BG);
		mvwprintw(window,0,startX,rightText.c_str());
	}
	wrefresh(window);
}

void printHeader(WINDOW *window,string leftText,string rightText){

	setbackground(window,DUNGEON_HEADER_FG,DUNGEON_HEADER_BG);
	wclrtoeol(window);

	rightText = STR_RIGHT_ARROW + rightText;

	int startX = (COLS - (leftText.size()+rightText.size()))/2;
	setcolors(window,DUNGEON_HEADER_FG,DUNGEON_HEADER_BG);
	mvwprintw(window,0,startX,leftText.c_str());
	startX += leftText.size();
	setcolors(window,DUNGEON_HEADER_FG_BOLD,DUNGEON_HEADER_BG);
	mvwprintwBold(window,0,startX,rightText.c_str());
	wrefresh(window);
}

void printHeader(WINDOW* window,string text)
{
	setbackground(window,DUNGEON_HEADER_FG,DUNGEON_HEADER_BG);
	wclrtoeol(window);

	mvwprintwCenterBold(window,0,text.c_str());
	wrefresh(window);
}

//to keep track of color pairs and group numbers because
//ncurses is silly
map<int,int> colorMap;
int uGroup = 1;

int getColorGroup(int fore,int back)
{
	//25 bigger than any foreground color making hash unique for all color combos
	int colorHash = fore + back*25;
	auto findResult = colorMap.find(colorHash);
	int group = NULL;
	if(findResult == colorMap.end())
	{
		init_pair(uGroup,fore,back);
		colorMap.insert(make_pair(colorHash,uGroup));
		group = uGroup;
		uGroup++;
	}
	else
	{
		group = findResult->second;
	}
	return group;
}

void setcolor (WINDOW* window,int fore)
{
	setcolors(window,fore,COLOR_BLACK);
}
void setcolors(WINDOW *window,int fore,int back)
{
	int group = getColorGroup(fore,back);
	wattron(window,COLOR_PAIR(group));
}

void setbackground(WINDOW *window,int fore,int back)
{
	int group = getColorGroup(fore,back);
	wbkgd(window,COLOR_PAIR(group));
}


// #red#
vector<DungeonChunk> parseDungeonText(vector<string> &textBuffer)
{
	vector<DungeonChunk> chunks;
	for(auto text : textBuffer) {
		bool bold = false;
		int backColor = COLOR_BLACK;
		int foreColor = COLOR_WHITE;
		DUNGEON_ALIGN align = DUNGEON_ALIGN::LEFT;
		DungeonChunk chunk;

		DungeonToken token;
		for(size_t i = 0; i < text.size(); i++)
		{
			unsigned char c = (unsigned char)text[i];
			unsigned char nextC = NULL;
			if(i < text.size()-1)
			{
				nextC = text[i+1];
			}

			//Center markup
			if(c=='`' && nextC != '`')
			{
				if(align == DUNGEON_ALIGN::LEFT) align = DUNGEON_ALIGN::CENTER;
				else align = DUNGEON_ALIGN::LEFT;
			}
			//Bold markup
			else if(c == '*' && nextC != '*')
			{
				bold = !bold;
			}
			//Fore Color Markup
			else if(c == '#' && nextC != '#')
			{
				char token = text[i+1];
				token = tolower(token);
				bool isColor = true;
				switch(token)
				{
				case 'e':
					foreColor = COLOR_BLACK;
					break;
				case 'r':
					foreColor = COLOR_RED;
					break;
				case 'g':
					foreColor = COLOR_GREEN;
					break;
				case 'y':
					foreColor = COLOR_YELLOW;
					break;
				case 'b':
					foreColor = COLOR_BLUE;
					break;
				case 'm':
					foreColor = COLOR_MAGENTA;
					break;
				case 'c':
					foreColor = COLOR_CYAN;
					break;
				case 'w':
					foreColor = COLOR_WHITE;
					break;
				default:
					isColor = false;
					break;
				}
				if(isColor) {
					i = i+1; //go past # and letter
				}

			} //BG COLOR
			else if(c == '~' && nextC != '~')
			{
				char token = text[i+1];
				token = tolower(token);
				bool isColor = true;
				switch(token)
				{
				case 'e':
					backColor = COLOR_BLACK;
					break;
				case 'r':
					backColor = COLOR_RED;
					break;
				case 'g':
					backColor = COLOR_GREEN;
					break;
				case 'y':
					backColor = COLOR_YELLOW;
					break;
				case 'b':
					backColor = COLOR_BLUE;
					break;
				case 'm':
					backColor = COLOR_MAGENTA;
					break;
				case 'c':
					backColor = COLOR_CYAN;
					break;
				case 'w':
					backColor = COLOR_WHITE;
					break;
				default:
					isColor = false;
					break;
				}
				if(isColor) {
					i = i+1; //go past # and letter
				}

			}
			else {
				if(c == '#' || c == '*' || c == '~' || c == '`') i++; //handle escaped #
				c = text[i];
				DungeonChar dc;
				dc.c = c;
				dc.bold = bold;
				dc.backColor = backColor;
				dc.foreColor = foreColor;
				dc.alignment = align;
				if(dc.c == CHR_SPACE)
				{
					chunk.push_back(token);
					token.clear();
					while(i < text.size() && (unsigned char)text[i] == CHR_SPACE)
					{
						token.push_back(dc);
						i++;
					}
					i--;
					chunk.push_back(token);
					token.clear();
				}
				else if(dc.c == CHR_NEWLINE)
				{
					chunk.push_back(token);
					token.clear();
					while(i < text.size() && (unsigned char)text[i] == CHR_NEWLINE)
					{
						token.push_back(dc);
						i++;
					}
					i--;
					chunk.push_back(token);
					token.clear();
				}
				else
				{
					token.push_back(dc);
				}
			}
		}
		chunk.push_back(token);
		chunks.push_back(chunk);
	}
	return chunks;
}

void renderDungeonText(WINDOW * window,vector<DungeonChunk> chunks)
{
	size_t y = 0;

	//For handling centered text
	bool centering = false;
	vector<DungeonChar> centerBuffer;

	for(auto chunk : chunks)
	{
		//Each chunk starts at x = 0 and default colors
		//and attributes
		size_t x =0;
		int currentColor = COLOR_WHITE;
		int currentBgColor = COLOR_BLACK;
		setcolors(window,currentColor,currentBgColor);

		for(auto token : chunk) {
			size_t remainingWidth = COLS-x;

			if(token.size() > remainingWidth)
			{
				y++;
				x = 0;
			}
			for(size_t i = 0; i < token.size(); i++)
			{

				DungeonChar dc = token[i];
				chtype attributes;
				if(dc.bold) attributes = A_BOLD;
				else attributes = A_NORMAL;

				if(dc.foreColor != currentColor || dc.backColor != currentBgColor)
				{
					setcolors(window,dc.foreColor,dc.backColor);
					currentColor = dc.foreColor;
					currentBgColor = dc.backColor;
				}
				
				if(dc.alignment == DUNGEON_ALIGN::CENTER)
				{
					centering = true;
					if(x!=0)
					{
						y++;
						x=0;
					}
					centerBuffer.push_back(dc);

				}
				else if(centering && dc.alignment == DUNGEON_ALIGN::LEFT)
				{
					x = (COLS - centerBuffer.size())/2;
					for(size_t j = 0; j < centerBuffer.size();j++)
					{
						DungeonChar centerChar = centerBuffer[j];
						currentColor = centerChar.foreColor;
						currentBgColor = centerChar.backColor;
						setcolors(window,currentColor,currentBgColor);
						if (centerChar.bold) attributes = A_BOLD;
						else attributes = A_NORMAL;
						mvwaddch(window,y,x,centerChar.c | attributes);
						x++;						
					}
					y++;
					x=0;
					centerBuffer.clear();
					centering = false;
				}				
				else {
					if(dc.c == CHR_NEWLINE) {
						mvwaddch(window,y,x,CHR_SPACE | attributes);
						x = 0;
						y++;
					}
					else if(x > COLS) {
						y++;
						x = 0;
						mvwaddch(window,y,x,dc.c | attributes);
					}
					else
					{
						mvwaddch(window,y,x,dc.c | attributes);
						x++;
					}
				}
			}
		} // end for token
		y++;
	} // end for chunk
}
