OBJS = Main.cpp

OBJ_NAME = program.exe

all : $(OBJS)
	g++ $(OBJS) -IC:\Development\i686-w64-mingw32\include -LC:\Development\i686-w64-mingw32\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -o $(OBJ_NAME)