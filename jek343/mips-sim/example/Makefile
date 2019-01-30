CC=mipsel-linux-gcc 
COMMONFLAGS=-static -mno-xgot -mno-abicalls -G 0
LINKFLAGS=-nostartfiles -nodefaultlibs $(COMMONFLAGS)
COMPILEANDASSEMBLE=-c $(COMMONFLAGS)
COMPILEONLY=-S $(COMMONFLAGS)

TARGETS=hello

all: $(TARGETS)

$(TARGETS): %: %.o Makefile
	$(CC) $(LINKFLAGS) -o $@ $@.o

.c.o:
	$(CC) $(COMPILEANDASSEMBLE) $<

.c.s:
	$(CC) $(COMPILEONLY) $<

clean:
	rm -f $(TARGETS) *.o
