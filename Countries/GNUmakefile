# Perception.Library for Amiga OS 4.x
#

CC		= gcc -N -q
STRIP	= strip
RM		= Delete
AD		= Autodoc
MAKE	= make

CWFLAGS	= -Wall -Wundef -Wshadow -Wpointer-arith -Wbad-function-cast -Wconversion -Waggregate-return
INCLUDE	= -I/SDK/includes/include_h/ -I/SDK/Workspace/Perception-IME/DevKit/Include/
CFLAGS	= -O2 $(CWFLAGS) -gstabs $(INCLUDE) -fno-builtin -fPIC -shared

LDFLAGS	= -nostartfiles -nostdlib -mcrt=newlib
LDLIBS	= -lc

TARGETS	=	a	b \
			c	d

countries: $(TARGETS)
	@Echo "Completed Building Countries DataBase"

clean:
	$(RM) $(TARGETS) ALL QUIET

%.o: %.c
	Echo $(CC) $(CFLAGS) -c -o $@ $<
