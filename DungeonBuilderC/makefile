CC=gcc
CXX=g++
RM=rm -f
INCLUDE = headers
CPPFLAGS=-g -std=c++11  -iquote $(INCLUDE)
LDFLAGS=-g
LDLIBS= -lncurses

SRCS=$(wildcard ./*.cpp)
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
