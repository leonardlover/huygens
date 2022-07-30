CC := gcc
INCLUDEDIR := include
OBJSRCDIR := include/src
CFLAGS := -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm
SRC := huygens.c
EXE := Huygens.exe

all: $(EXE) clean run

utils.o:
	@echo "Compiling..."
	@$(CC) -c $(OBJSRCDIR)/$(@:.o=.c) -I$(INCLUDEDIR)

rect_utils.o:
	@$(CC) -c $(OBJSRCDIR)/$(@:.o=.c) -I$(INCLUDEDIR)

$(EXE): utils.o rect_utils.o
	@$(CC) $(SRC) -o $@ $^ -I$(INCLUDEDIR) $(CFLAGS)

.PHONY: clean
clean:
	@rm *.o

.PHONY: run
run:
	@echo "Running..."
	@./$(EXE)
