# Example:
# INCLUDE		= -I ./include
# LIB			= -lpthread
# LIB_3RD	    = -L /usr/lib ./lib/libxxx.a
# INCS			= $(INCLUDE)
# LIBS			= $(LIB) -ldl -lidn
CC = gcc -std=gnu99
CXX = g++
BIN = ./
INCLUDE = 
LIB = 
FLAGS = -fPIC -O0 -g3 -Wall -fmessage-length=0 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILE -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE
LIB_3RD = 

OBJECTS		= $(BIN)main.o \
	$(BIN)anole.o \

TARGET		= ./anole
INCS		= $(INCLUDE)
LIBS		= $(LIB)

all: $(OBJECTS)

	test -d $(BIN) || mkdir -p $(BIN)
	$(CXX) -rdynamic -o $(TARGET) $(INCS) $(OBJECTS) $(LIB_3RD) $(LIBS)

CWD			= ./

$(BIN)main.o:	$(CWD)main.cpp
	$(CXX) $(FLAGS) $(INCS) -c -o $@ $<

$(BIN)anole.o:	$(CWD)anole.cpp
	$(CXX) $(FLAGS) $(INCS) -c -o $@ $<

clean:
	rm -rf $(TARGET) $(OBJECTS)

# install: $(TARGET)
# 	test -d $(DESTDIR)$(prefix) || mkdir -p		$(DESTDIR)$(prefix)
# 	install -v -m 0755 $(TARGET) $(DESTDIR)$(prefix)