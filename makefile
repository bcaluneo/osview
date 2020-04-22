CC = g++
EXE = osview

FLAGS = -std=c++17
FLAGS += -Wall
FLAGS += -ID:\SDL\include\SDL2 -LD:\SDL\lib
FLAGS += -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

# FLAGS += -Wl,--subsystem,windows

OBJS = main.o \
			 graph.o

all : $(OBJS)
	$(CC) $^ $(FLAGS) -o $(EXE)

$(OBJS):
	$(CC) $(FLAGS) -c src/$(@:.o=.cc) -o $@

zip:
	tar -cvf "$(EXE)".tar *.dll *.exe tex/*.png

clean:
	rm -f *.exe
	rm -f *.tar
