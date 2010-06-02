CXX = g++
CXXFLAGS = -Wall -O3 -g 
EXEC_NAME = FireMonkeys
INCLUDES = -I/usr/include/GL  
LIBS =  -lglut -lGLEW -lGLU -lGL -lrt
INSTALL_DIR = 

OBJDIR=../obj/


SRC = $(wildcard *.cpp) 
OBJ = $(SRC:%.cpp=$(OBJDIR)%.o) 


boundaries:CXXFLAGS:=$(CXXFLAGS) -D_BOUNDARIES_SOLVE_
boundaries: all

all : $(EXEC_NAME)

clean :
	rm -f $(EXEC_NAME) $(OBJDIR)*.o

$(EXEC_NAME) : $(OBJ)
	$(CXX) -o $(EXEC_NAME) $(OBJ) $(LIBS)

$(OBJDIR)%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

$(OBJDIR)%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<


