################################################################################
#              Makefile for the CS 3410 MIPS Interpreter Project               #
################################################################################

# Additional flags for the compiler
CFLAGS=-std=c99 -D_BSD_SOURCE -Wall

# Default target to run, which creates a `mips_interpreter` executable
all: mips_interpreter

# Compiles the student linkedlist.c into an object file
# Then, combines the object file into a single `linkedlist` executable
linkedlist: linkedlist.o linkedlist_main.o
	gcc $(CFLAGS) -o $@ $^

# Compiles the student linkedlist.c, hashtable.c into object files
# Then, combines the object files into a single `hashtable` executable
hashtable: linkedlist.o hashtable.o hashtable_main.o
	gcc $(CFLAGS) -o $@ $^

# Compiles the student linkedlist.c, hashtable.c, and mips.c into object files
# Then, combines the object files into a single `mips_interpreter` executable
mips_interpreter: linkedlist.o hashtable.o mips.o mips_interpreter.o
	gcc $(CFLAGS) -Werror -o $@ $^

# Wildcard rule that allows for the compilation of a *.c file to a *.o file
%.o : %.c
	gcc -c $(CFLAGS) $< -o $@

# Removes any executables and compiled object files
clean:
	rm -f linkedlist hashtable mips_interpreter *.o
