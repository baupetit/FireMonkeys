CC = g++
CFLAGS = -Wall -O3 -g
EXEC_NAME = FireMonkeys
INCLUDES = -I/usr/include/GL  
LIBS =  -lglut -lGLEW -lGLU -lGL -lrt
INSTALL_DIR = 

OBJDIR=../obj/


SRC = $(wildcard *.cpp) 
OBJ = $(SRC:%.cpp=$(OBJDIR)%.o) 

all : $(EXEC_NAME)

clean :
	rm -rf $(EXEC_NAME) $(OBJDIR)*.o

$(EXEC_NAME) : $(OBJ)
	$(CC) -o $(EXEC_NAME) $(OBJ) $(LIBS)

$(OBJDIR)%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(OBJDIR)%.o:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<


