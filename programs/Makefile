# A SIMPY MAKEFILE FOR C++ PROGRAMS

# paths
INCLUDE = ../include
MODULES = ../modules

#compiler
CC = g++

# Compile options. Το -I<dir> λέει στον compiler να αναζητήσει εκεί include files
CPPFLAGS = -Wall  -g -I$(INCLUDE) -std=c++11

# Αρχεία .o
OBJS =  play.o $(MODULES)/items.o $(MODULES)/spells.o $(MODULES)/living.o $(MODULES)/grid.o $(MODULES)/general.o $(MODULES)/useables.o

# Το εκτελέσιμο πρόγραμμα
EXEC = prog 

# Command line arguments
ARGS =
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

valgrind:
	valgrind --leak-check=full ./$(EXEC) $(ARGS)