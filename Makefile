CC = g++
CFLAGS := -std=c++11 -Wall -Wfatal-errors -c
LDFLAGS := -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
BIN = SuperBomberPeggyDeluxe
OBJ = bomberman.o map.o player.o strong.o fast.o bomber.o bomb.o timer.o mapEditor.o main.o

all: .depend $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

.depend: *.hpp *.cpp
	@echo -n > .depend
	@$(foreach obj,$(OBJ), g++ $(CFLGAS) -MM $(subst .o,.cpp,$(obj)) >> .depend;)

-include .depend

clean::
	rm -f $(OBJ) $(BIN) .depend *.*~ Makefile~
