CC = g++
EXE = osview

FLAGS = -std=c++11
FLAGS += -Wall
FLAGS += -ID:\SDL\include\SDL2 -LD:\SDL\lib
FLAGS += -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

OBJS = main.cc

default:
	$(CC) src/$(OBJS) $(FLAGS) -o $(EXE)

rel:
	$(CC) src/$(OBJS) $(FLAGS) -Wl,--subsystem,windows -o $(EXE)

zip:
	tar -cvf "$(EXE)".tar *.dll *.exe tex/*.png

clean:
	rm -f *.exe
	rm -f *.tar
