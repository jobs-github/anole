# Example:
# INCLUDE		= -I ./include
# LIB			= -lpthread
# LIB_3RD	    = -L /usr/lib ./lib/libxxx.a
# INCS			= $(INCLUDE)
# LIBS			= $(LIB) -ldl -lidn
CC = gcc -std=gnu99
CXX = g++
BIN = ./build/
INCLUDE = -I /usr/local/include \
	-I /usr/local/boost1.66/include \
	-I slothjson \
	-I slothjson/rapidjson

LIB = -lboost_system \
	-lpthread \
	-lcrypto \
	-lssl \
	-lzlog

FLAGS = -std=c++11 -fPIC -O0 -g3 -Wall -fmessage-length=0 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILE -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE
LIB_3RD = -L /usr/local/lib

OBJECTS		= $(BIN)main.o \
	$(BIN)anole.o \
	$(BIN)server_session.o \
	$(BIN)client_session.o \
	$(BIN)session.o \
	$(BIN)anole_proto.o \
	$(BIN)config.o \
	$(BIN)utils.o \
	$(BIN)slothjson.o

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

$(BIN)server_session.o:	$(CWD)server_session.cpp
	$(CXX) $(FLAGS) $(INCS) -c -o $@ $<

$(BIN)client_session.o:	$(CWD)client_session.cpp
	$(CXX) $(FLAGS) $(INCS) -c -o $@ $<

$(BIN)session.o:	$(CWD)session.cpp
	$(CXX) $(FLAGS) $(INCS) -c -o $@ $<

$(BIN)anole_proto.o:	$(CWD)anole_proto.cpp
	$(CXX) $(FLAGS) $(INCS) -c -o $@ $<

$(BIN)config.o:	$(CWD)config.cpp
	$(CXX) $(FLAGS) $(INCS) -c -o $@ $<

$(BIN)utils.o:	$(CWD)utils.cpp
	$(CXX) $(FLAGS) $(INCS) -c -o $@ $<

DIR_JSON = ./slothjson/

$(BIN)slothjson.o:	$(DIR_JSON)slothjson.cpp
	$(CXX) $(FLAGS) $(INCS) -c -o $@ $<

clean:
	rm -rf $(TARGET) $(OBJECTS)

# install: $(TARGET)
# 	test -d $(DESTDIR)$(prefix) || mkdir -p		$(DESTDIR)$(prefix)
# 	install -v -m 0755 $(TARGET) $(DESTDIR)$(prefix)