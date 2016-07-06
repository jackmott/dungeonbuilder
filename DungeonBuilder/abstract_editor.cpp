#include "abstract_editor.h"
#include "string_constants.h"

using namespace std;

bool AbstractEditor::checkCommonInput(string s)
{
	if (s == STR_PAGE_UP)
	{
		if(scrollpos > 0) scrollpos --;
		renderTextBuffer();
		return true;
	} else if (s == STR_PAGE_DOWN)
	{
		scrollpos++;
		renderTextBuffer();
		return true;
	}
	return false;
}


void AbstractEditor::renderTextBuffer()
{
	wclear(mainWindow);
	for(size_t y = 0; y < LINES-3;y++)
	{
		if(y+scrollpos >= textBuffer.size()) break;
		mvwprintw(mainWindow,y,0,textBuffer[y+scrollpos].c_str());
	}
	wrefresh(mainWindow);

}