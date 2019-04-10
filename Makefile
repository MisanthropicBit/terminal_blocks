BIN := terminal_blocks
CC := clang
CCFLAGS := $(shell ncurses5.4-config --cflags) -Iinclude -Wall -Wshadow -Wpedantic -Wextra -g
LDFLAGS := $(shell ncurses5.4-config --libs)
OBJDIR := objs
SRCDIR := src

.PHONY: clean debug

%.o: $(SRCDIR)/%.c
	$(CC) $(CCFLAGS) -c $< -o $@

$(BIN): block.o enter_highscore.o game.o highscores.o main.o menu.o random.o terminal.o timer.o
	$(CC) $(CCFLAGS) $(LDFLAGS) $^ -o $(BIN)

debug: CCFLAGS += -DTB_DEBUG
debug: $(BIN)

clean:
	rm -v $(BIN) *.o
