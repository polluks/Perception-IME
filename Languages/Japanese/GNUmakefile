# Japanese.Language for Amiga OS 4.x
#
LNAME	= japanese.language

MAKE	= make
CC		= gcc -N -q
STRIP	= strip
COPY	= Copy
RM		= Delete

CWFLAGS	= -Wall -Wundef -Wshadow -Wpointer-arith -Wbad-function-cast -Wconversion -Waggregate-return
INCLUDE	= -I/SDK/includes/include_h/ -I../../DevKit/Include/
CFLAGS	= -O2 $(CWFLAGS) -gstabs $(INCLUDE) -fno-builtin -fPIC -shared

LDFLAGS	= -nostartfiles -nostdlib -mcrt=newlib
LDLIBS	= -lc

OBJECTS	= main.o apicalls.o hook.o

$(LNAME): devkit $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $(LNAME).debug
	$(STRIP) $(LNAME).debug -o $(LNAME)

clean:
	$(RM) $(OBJECTS) $(LNAME).debug $(LNAME) ALL QUIET

install: $(LNAME)
	$(COPY)	$(LNAME).debug LOCALE:Languages/$(LNAME)

devkit:
	$(MAKE) -C ../../DevKit -f GNUmakefile

%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<
