# Perception.Library for Amiga OS 4.x
#
LNAME	= perception.library
AUTODOC	= perception.doc

CC		= gcc -N -q
STRIP	= strip
RM		= Delete
AD		= Autodoc
MAKE	= make

CWFLAGS	= -Wall -Wundef -Wshadow -Wpointer-arith -Wbad-function-cast -Wconversion -Waggregate-return
INCLUDE	= -I/SDK/includes/include_h/ -I../DevKit/Include/
CFLAGS	= -O2 $(CWFLAGS) -gstabs $(INCLUDE) -fno-builtin -fPIC -shared

LDFLAGS	= -nostartfiles -nostdlib -mcrt=newlib
LDLIBS	= -lc

OBJECTS	= main.o apicalls.o daemon.o objects.o

$(LNAME): devkit $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $(LNAME).debug
	$(STRIP) $(LNAME).debug -o $(LNAME)

clean:
	$(RM) $(OBJECTS) $(LNAME).debug $(LNAME) ALL QUIET

devkit:
	$(MAKE) -C ../DevKit -f GNUmakefile

autodoc:
	$(AD) apicalls.c >$(AUTODOC)

install: $(LNAME)
	C:Copy	$(LNAME).debug	LIBS:$(LNAME)
	C:Delete LIBS:$(LNAME).debug

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


