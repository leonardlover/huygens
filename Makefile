CC := gcc
INCLUDEDIR := include
OBJSRCDIR := include/src
CFLAGS := -lSDL2 -lSDL2_image -lSDL2_mixer -lm -g
SRC := huygens.c
EXE := Huygens.exe

all: $(EXE) clean run

utils.o:
	@echo "Compiling..."
	@$(CC) -c $(OBJSRCDIR)/$(@:.o=.c) -I$(INCLUDEDIR)

asset_utils.o:
	@$(CC) -c $(OBJSRCDIR)/$(@:.o=.c) -I$(INCLUDEDIR)

$(EXE): utils.o asset_utils.o
	@$(CC) $(SRC) -o $@ $^ -I$(INCLUDEDIR) $(CFLAGS)

.PHONY: clean
clean:
	@rm *.o

.PHONY: run
run:
	@echo "Running..."
	@./$(EXE)
	@echo "Exiting..."
