# Declaration of variables
CC = gcc
CXXFLAGS = 
LDFLAGS = -lncurses

# File names
EXEC = snek
#SOURCES = $(wildcard **/*.cpp)
SOURCES = $(shell find . -name "*.c")
OBJECTS = $(SOURCES:.c=.o)
#OBJECTS = $(lastword $(subst /, , $(SOURCES:.cpp=o)))
OBJDIR = obj

all: createdir $(EXEC) tidy

createdir:
	mkdir -p obj && mkdir -p bin

tidy: createdir $(EXEC)
	find . -name "*.o" -not -path "./obj/*" -exec mv -v {} obj \;

#print variables
print-%: ; @echo $*=$($*)

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS)  -o $(EXEC)

# To obtain object files
%.o: %.c
	$(CC) -c $(CXXFLAGS) $< -o $@

# To remove generated files
clean:
	rm -f $(EXEC)
	rm -f $(OBJECTS)
	rm -rf obj

#non-file targets
.phony: clean tidy createdir all
