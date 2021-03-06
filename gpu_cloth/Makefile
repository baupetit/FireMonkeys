###########################################################################
#   Copyright (C) 2005 by Maurizio Monge                                  #
#   monge@sns.it                                                          #
#                                                                         #
#   This program is free software; you can redistribute it and/or modify  #
#   it under the terms of the GNU General Public License as published by  #
#   the Free Software Foundation; either version 2 of the License, or     #
#   (at your option) any later version.                                   #
#                                                                         #
#   This program is distributed in the hope that it will be useful,       #
#   but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
#   GNU General Public License for more details.                          #
#                                                                         #
#   You should have received a copy of the GNU General Public License     #
#   along with this program; if not, write to the                         #
#   Free Software Foundation, Inc.,                                       #
#   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             #
###########################################################################

CC=gcc
CXX=g++

INC=
LIB=-lm -lGL -lGLU -lpng -ljpeg

#INC=$(shell pkg-config gtk+-2.0 --cflags) -DHAVE_GTK
#LIB=$(shell pkg-config gtk+-2.0 --libs) -lm -lGL -lGLU -lpng -ljpeg

XINC=-I/usr/X11R6/include -DXTHREADS -DXUSE_MTSAFE_API
XLIB=-L/usr/X11R6/lib -lX11 -lXext -lXxf86vm

CFLAGS=-O2 -fomit-frame-pointer -Wall -D_GNU_SOURCE -DGL_GLEXT_PROTOTYPES $(INC) $(XINC)
#CFLAGS=-g3 -Wall -D_GNU_SOURCE -DGL_GLEXT_PROTOTYPES $(INC) $(XINC)
CXXFLAGS=$(CFLAGS)
LDFLAGS=$(LIB) $(XLIB)


SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o)
DEPS=$(SRC:%.cpp=.%.dep)
HEADERS=$(wildcard *.h)
MOCS=
TARGET=gpu_cloth


all: $(TARGET)

clean:
	-rm -f *.o *.so

distclean:
	-rm -f *.moc *.o *.so .*.dep *~ *.s *.i *.ii $(TARGET) depend

$(TARGET): $(MOCS) $(OBJ)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS)

$(PLUGIN_DIR)/lib%.so: $(PLUGIN_DIR)/%.o
	$(CXX) -o $@ $< $(LDFLAGS) -shared -Wl,-soname,$(TARGET_SO)

%.moc: %.h
	moc -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.%.dep: %.cpp
	$(CXX) -MM $(CXXFLAGS) $< >$@

.%.dep: %.c
	$(CC) -MM $(CFLAGS) $< >$@

-include $(DEPS)
