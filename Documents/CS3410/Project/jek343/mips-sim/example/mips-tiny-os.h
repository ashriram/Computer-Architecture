#ifndef _MIPS_TINY_OS_H_
#define _MIPS_TINY_OS_H_

/* 
 * The MIPS simulator comes built-in with a tiny, barely functional operating system
 * that can do very simple input and output. This header file defines the programming
 * interface that MIPS programs can use to access these operating system services.
 *
 * Do not modify this file -- it must match the internals of the MIPS simulator.
 *
 * To compile a program to run on the simulator, include this file in your C program:
 *    #include "mips-tiny-os.h"
 * Do not use standard C headers, such as <stdio.h>. They won't work in the simulator.
 * Compile programs using mipsel-linux-gcc with a command line like this:
 * >  mipsel-linux-gcc -nostartfiles -nodefaultlibs -static -mno-xgot -mno-embedded-pic \
 *     -mno-abicalls -G 0 -o foo foo.c
 * Or, use a Makefile like the one provided.
 *
 */

// Null pointer definition; normally in system headers, but we don't use those.
#define NULL ((void *)0)

// End-of-file marker; can be returned by some of the input functions below.
#define EOF (-1)


// print an integer
void printi(int i);

// print a string
void prints(char *s);

// print a single character
void putc(char c);

// read and return a single character of text from the user;
// returns the character, or EOF if there is no more text to read
int getc(void);

// read a line of text from the user and stores it in the given buffer;
// the line of text ends at the first '\r' or '\n' character (which are
// discarded and not stored in buf) or the end-of-file, whichever comes first;
// returns buf on success, or NULL if there is no more text to read
char *gets(char *buf);

// return a pseudo-random number
int rand(void);

// get the current username from the $NETID or $USER environment variables
// truncated to n chars including the null terminator
void getuser(char *buf, int n);

// read at most len bytes from file and stores them in the given buffer;
// on success, return the number of bytes actually read; returns a negative for errors
int readfile(char *filename, char *buf, int len);

// write len bytes to file and truncates the file to exactly len bytes; 
// on success, return the number of bytes actually written; returns a negative for errors
int writefile(char *filename, char *buf, int len);

// exit the program with the given status code;
// this call never returns
void exit(int status);

// stops the program using a BREAK instruction;
// useful when debugging, otherwise fatal
void breakpoint(void);



/* the rest of this file contains the implementations of the above functions */

void printi(int i) {
  /* invoke system call number 1 */
  asm("addiu $2, $0, 1");
  asm("syscall");
}

void prints(char *s) {
  /* invoke system call number 2 */
  asm("addiu $2, $0, 2");
  asm("syscall");
}

void putc(char c) {
  /* invoke system call number 3 */
  asm("addiu $2, $0, 3");
  asm("syscall");
}

int getc() {
  /* invoke system call number 4 */
  asm("addiu $2, $0, 4");
  asm("syscall");
}

char *gets(char *buf) {
  int i = 0;
  for (;;) {
    int c = getc();
    if (c == EOF && i == 0) {
      return NULL;
    } else if (c == EOF || c == '\n' || c == '\r') {
      buf[i] = '\0';
      return buf;
    } else {
      buf[i++] = c;
    }
  }
}

int rand(void) {
  /* invoke system call number 6 */
  asm("addiu $2, $0, 6");
  asm("syscall");
}

void getuser(char *buf, int n) {
  /* invoke system call number 7 */
  asm("addiu $2, $0, 7");
  asm("syscall");
}

int readfile(char *filename, char *buf, int len) {
  /* invoke system call number 8 */
  asm("addiu $2, $0, 8");
  asm("syscall");
}

int writefile(char *filename, char *buf, int len) {
  /* invoke system call number 9 */
  asm("addiu $2, $0, 9");
  asm("syscall");
}

void exit(int status) {
  /* invoke system call number 10 */
  asm("addiu $2, $0, 10");
  asm("syscall");
  for (;;); // exit isn't supposed to return, so keep the compiler happy
}

void breakpoint(void) {
  /* insert a BREAK instruction */
  asm("break");
}

void __start(int ac, char **av)
{
  int status = main(ac, av);
  exit(status);
}

#endif // _MIPS_TINY_OS_H_
