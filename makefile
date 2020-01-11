CXX_OBJS = \
		source/main.o\
		source/player.o\

CXX = g++
INCLUDE += include/
CXXFLAGS += -I$(INCLUDE)

BINARY = kiuta

all: $(BINARY)

$(BINARY) : $(CXX_OBJS)
	@echo "LINKING $@"; $(CXX) -o $(CXX_OBJS) -lsfml-graphics -lsfml-window -lsfml-system

.cpp.o:
	@echo "CXX $@"; $(CXX) $(CXXFLAGS) -o $@ -c $<