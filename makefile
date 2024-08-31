ifeq ($(OS),Windows_NT)
    EXE := .exe
# Need testing, idk if this is true.
    RM  := del
    PYTHON := python
else
    EXE :=
    RM  := rm -rf
    PYTHON := python3
endif

PROG := testmidcxxap$(EXE)

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Werror -Wextra -O2 -I.
RELEASESCRIPT := release.py

INCDIR   := midcxxap
CXXFILES := main.cpp $(wildcard $(INCDIR)/*.cpp)
OFILES   := $(CXXFILES:%.cpp=%.o)

.PHONY: all clean release

all: $(PROG)

clean:
	$(RM) $(PROG)
	$(RM) $(OFILES)

release:
	$(PYTHON) $(RELEASESCRIPT) -s=$(INCDIR)/midcxxap.cpp -i=$(INCDIR)/midcxxap.hpp -o=midcxxap.hpp

$(PROG): $(OFILES)
	$(CXX) $(OFILES) -o $@

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)
