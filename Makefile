CC := gcc
INCLUDEDIR := include
OBJSRCDIR := include/src
CFLAGS := -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm
SRC := huygens.c
EXE := Huygens.exe

all: $(EXE) clean run

$(INCLUDEDIR)/utils.ghc:
	@echo "Compiling..."
	@$(CC) $(@:.ghc=.h)

utils.o: $(INCLUDEDIR)/utils.ghc
	@$(CC) -c $(OBJSRCDIR)/$(@:.o=.c) -I$(INCLUDEDIR)

$(INCLUDEDIR)/rect_utils.ghc:
	@$(CC) $(@:.ghc=.h)

rect_utils.o: $(INCLUDEDIR)/rect_utils.ghc
	@$(CC) -c $(OBJSRCDIR)/$(@:.o=.c) -I$(INCLUDEDIR)

$(INCLUDEDIR)/menu_utils.ghc:
	@$(CC) $(@:.ghc=.h)

menu_utils.o: $(INCLUDEDIR)/menu_utils.ghc
	@$(CC) -c $(OBJSRCDIR)/$(@:.o=.c) -I$(INCLUDEDIR)

$(EXE): utils.o rect_utils.o menu_utils.o
	@$(CC) $(SRC) -o $@ $^ -I$(INCLUDEDIR) $(CFLAGS)

.PHONY: clean
clean:
	@rm $(INCLUDEDIR)/*.gch
	@rm *.o

.PHONY: run
run:
	@echo "Running..."
	@./$(EXE)
