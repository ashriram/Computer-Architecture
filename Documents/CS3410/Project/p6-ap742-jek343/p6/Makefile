#####################################################################
# CS 3410: Dynamic Memory Allocator (i.e., malloc)                  #
#                                                                   #
# Execute `make help` to enumerate all of the targets you can       #
# build.                                                            #
#                                                                   #
# Do not, under any circumstances, edit this Makefile. For example, #
# if you change the CFLAGS to include -O3 instead of -O0, what you  #
# perceive your performance to be will _not_ be what we test it     #
# against when it is graded.                                        #
#                                                                   #
# You do not need to read anything below, but we have included      #
# excessive comments to hopefully de-mistify how make works.        #
#                                                                   #
#####################################################################

# Initial  compilation and  linking flag  definitions.  Make  works by
# recursively defining variables and even targets depending on what it
# determines needs  to evaluated.   This is one  of the  most powerful
# features of make,  however you sometimes need to  bypass it.  Use :=
# to define  a variable on that  line.  You can  also use ?= to  set a
# variable  only if  it is  not already  defined (e.g.  if a  user has
# already defined it  as a shell environment variable,  you don't want
# to overwrite it).

CFLAGS := -O0 -Wall -Werror
LD_FLAGS =

# By default we will compile your custom heaplib.c since you will
# spend the majority of your time working on this file.
HEAPLIB_SOURCE = heaplib.c

# Make was designed to have targets (thing before the colon) have
# dependencies (things listed after the colon) that are files.  The
# reason is that when a given file in the list of dependencies is
# changed, make detects this (it uses the timestamp) and knows that
# any targets related to that file need to be changed.  PHONY is used
# to specify that you have a target, but it does not directly depend
# on any files.  Often times you can get away with not having PHONY,
# but it is hard to detect when this is the problem and you are better
# off using it.
.PHONY: all clean tests help
.PHONY: heaplib.o debug print-debug lame lame-debug lame-print-debug ll ll-debug ll-print-debug

# The all target is what gets executed by default when you enter
# `make` without any target names.  Targets are all lines with a colon
# after them.  See the heaplib.o target below for why we are cleaning
# before we compile (this is non-standard, and somewhat defeats the
# purpose of make).
all: clean test_heaplib

#####################################################################
# Your custom heaplib additional compilation targets.               #
#####################################################################
# When you use #ifdef PRINT_DEBUG, this is what defines that.  In the
# rest of the file you will see many targets listed twice.  There is
# only one actual target, but the preceding lines are executing
# target-level definitions / modifications of variables.
print-debug: CFLAGS += -DPRINT_DEBUG
print-debug: debug

# There are four levels of debugging available in gcc and most
# compilers: 0,1,2,3.  -g0 disables debugging symbols, and the default
# is -g2.  We choose -g3 for maximum information.  Both clang and icc
# support these levels.  Refer to
# https://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html for more
# information than you'll ever want.
debug: CFLAGS += -g3
debug: clean tests

#####################################################################
# Lame targets: same as before except we compile heaplib_lame.c   #
#####################################################################
# This is a target-level definition of HEAPLIB_SOURCE.  Since make
# evaluates variables lazily (as they are needed), the correct
# HEAPLIB_SOURCE will be used when compiling stages listed in the next
# section.  It still works if we used := at the top, but in this
# scenario it was only defined once, rather than overwritten.
lame: HEAPLIB_SOURCE = heaplame.c
lame: clean tests
# Compile with extra detailed debugging information.
lame-debug: CFLAGS += -g3
lame-debug: lame
# Compile with debug information, and define PRINT_DEBUG for the c file.
lame-print-debug: CFLAGS += -DPRINT_DEBUG
lame-print-debug: lame-debug

#####################################################################
# Less Lame targets: same as before except we compile heaplib_lesslame.c   #
#####################################################################
# This is a target-level definition of HEAPLIB_SOURCE.  Since make
# evaluates variables lazily (as they are needed), the correct
# HEAPLIB_SOURCE will be used when compiling stages listed in the next
# section.  It still works if we used := at the top, but in this
# scenario it was only defined once, rather than overwritten.
ll: HEAPLIB_SOURCE = heaplesslame.c
ll: clean tests
# Compile with extra detailed debugging information.
ll-debug: CFLAGS += -g3
ll-debug: ll
# Compile with debug information, and define PRINT_DEBUG for the c file.
ll-print-debug: CFLAGS += -DPRINT_DEBUG
ll-print-debug: ll-debug

#####################################################################
# The actual compilation.                                           #
#####################################################################
# The following compiles the previously defined HEAPLIB_SOURCE into an
# object file using the -c flag of gcc.  We expand the CFLAGS that
# have been modified by various targets to achieve the compilation
# goal we desire.  Finally, we must include -o and specify heaplib.o
# ($@ expands to the name of the target) because we will either be
# compiling heaplib.c (which would normally be heaplib.o) _or_
# heaplame.c (which by default would compile into
# heaplame.o).  Our later targets expect heaplib.o.
heaplib.o:
	gcc -c $(CFLAGS) $(HEAPLIB_SOURCE) -o $@


# Depend on all of the test files we seek to compile so that all of
# them will be built.
tests: test_heaplib

# When you compile executables, you always include the linking
# (LD_FLAGS) last.  Each target below uses $+, which expands to all of
# the dependencies.  For example, in the test_heaplib target $+ expands
# to: heaplib.o test_heaplib.c
test_heaplib: heaplib.o test_heaplib.c
	gcc $(CFLAGS) $+ -o $@ $(LD_FLAGS)

# It is common practice to define a clean target that performs cleanup
# for the user.  What is cleaned and how many clean targets you define
# varies, but it is good practice to define at least one.
clean:
	rm -f *.o test_heaplib

# Defining help but is much less common.  You may have noticed that
# all the commands that actually get executed by make (gcc in this
# file) print to the screen.  This is the default behavior to allow
# transparency.  If you want to suppress the printing, you put an @ in
# front of the line you want to suppress so that it is just executed.
# Since this is printing a help message, the command echoing behavior
# of make is less than helpful...
help:
	@printf "CS 3410 Malloc Build System\n"
	@printf "\nYour custom heaplib.c implementation targets:\n"
	@printf "  make                   : heaplib.c\n"
	@printf "  make debug             : heaplib.c with debugging symbols\n"
	@printf "  make print-debug       : heaplib.c with debugging symbols and PRINT_DEBUG\n"
	@printf "\nThe lame implementation targets:\n"
	@printf "  make lame             : heaplame.c\n"
	@printf "  make lame-debug       : heaplame.c with debugging symbols\n"
	@printf "  make lame-print-debug : heaplame.c with debugging symbols and PRINT_DEBUG\n"
	@printf "\nThe less lame implementation targets:\n"
	@printf "  make ll             : heaplesslame.c\n"
	@printf "  make ll-debug       : heaplesslame.c with debugging symbols\n"
	@printf "  make ll-print-debug : heaplesslame.c with debugging symbols and PRINT_DEBUG\n"
	@printf "\nAll targets compile the test_heaplib executable. You\n"
	@printf "can use tab completion for make targets on the course VM.  It will also list\n"
	@printf "files in the current directory, but will work as long as you complete to a\n"
	@printf "valid target.\n"
	@printf "\n  - For non-default targets, type make and don't hit enter.\n"
	@printf "  - Hit tab twice to list all potential completions.\n"
	@printf "  - When you type enough for only one to be possible it will be completed.\n"
	@printf "  - [hit enter]\n"
	@printf "\nNOTE: There are other targets that will show up with tab completion.\n"
	@printf "      These targets should not be compiled (e.g. test_heaplib). Use the\n"
	@printf "      targets we have listed above.\n"

# So in the end, this is a rather non-standard Makefile since we are
# varying what is compiled.  If your ultimate goal was to create just
# one executable (say test_heaplib), then you would just do something like
# below.  The only other gotchas with Make are that you have to use
# TAB characters in targets (not spaces), as well as a common C/C++
# mistake is to not include the header files in the target
# dependencies.  We do not use the header file (heaplib.h) above
# because you should NOT be changing it.  But if you are writing your
# own project, then it is likely you will be adding new code in the
# header as you develop, and if it is not explicitly listed in one of
# the targets then the new changes may not be brought in.
#
# .PHONY: all test
#
# all: test_heaplib
#
# test_heaplib: heaplib.o heaplib.h test_heaplib.c
#     gcc $(CFLAGS) $+ -o $@ $(LD_FLAGS)
#
# # $< is the first dependency listed
# heaplib.o: heaplib.c
#     gcc -c $(CFLAGS) $< -o $@
#
# clean:
#     rm -f *.o test_heaplib
#
# The following is an extremely useful though somewhat hard to search
# for resource on what the different automatic variables are:
#
# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
#
# Happy making, go write your own recipe!
