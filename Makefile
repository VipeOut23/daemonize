##
# Daemonize
#

CC= gcc
CFLAGS = -Wall -std=gnu17
ALLCFLAGS = $(CFLAGS) $(shell echo | gcc -xc -E -v - 2>&1 | grep -E '^\s' | sed '1d;s/^\s/ -I/' | tr '\n' ' ') # Explictly include system libraries for cdb
OBJ= daemonize.o
PREFIX=~/.local

daemonize: $(OBJ)
	$(CC) $(ALLCFLAGS) $(OBJ) -o daemonize

install: daemonize
	install -vD -m755 daemonize $(PREFIX)/bin/daemonize

uninstall:
	rm $(prefix)/bin/daemonize

run: daemonize
	./daemonize $(ARGS)

clean:
	rm $(OBJ) daemonize

%.o: %.c
	$(CC) $(ALLCFLAGS) -c $< -o $@

help:
	@echo "Targets: "
	@echo "  - daemonize (default)"
	@echo "  - install"
	@echo "  - uninstall"
	@echo "  - clean"
	@echo "Environment:"
	@echo "  - PREFIX (un-/install prefix | def: ~/.local)"

.PHONY: daemonize run clean install uninstall help

# end
