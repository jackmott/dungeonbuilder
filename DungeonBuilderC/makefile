CC=gcc
CXX=g++
RM=rm -f
INCLUDE = headers
CPPFLAGS=-g -std=c++11  -iquote $(INCLUDE)
LDFLAGS=-g
LDLIBS= -lncurses

SRCS=command_window.cpp creature_editor.cpp dungeon_data.cpp dungeon_display.cpp exit_editor.cpp main_menu.cpp main.cpp object_editor.cpp printutils.cpp room_editor.cpp dungeon_engine.cpp text_editor.cpp utils.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: dungeonbuilder

dungeonbuilder: $(OBJS)
	$(CXX) $(LDFLAGS) -o dungeon $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .depend

include .depend
