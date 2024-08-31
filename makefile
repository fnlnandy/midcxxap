ifeq ($(OS),Windows_NT)
    EXE := .exe
# Need testing, idk if this is true.
    RM  := del
else
    EXE :=
    RM  := rm -rf
endif

PROG := testmidcxxap$(EXE)

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Werror -Wextra -O2 -I.

INCDIR   := midcxxap
CXXFILES := main.cpp $(wildcard $(INCDIR)/*.cpp)
OFILES   := $(CXXFILES:%.cpp=%.o)

.PHONY: all clean

all: $(PROG)

clean:
	$(RM) $(PROG)
	$(RM) $(OFILES)

$(PROG): $(OFILES)
	$(CXX) $(OFILES) -o $@

%.o: %.cpp
	$(CXX) -c $< -o $@
