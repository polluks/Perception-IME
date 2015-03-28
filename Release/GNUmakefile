#
#	'Perception.Library' Build Scripting
#
MAKE	=	SDK:C/make
MKDIR	=	C:Makedir
COPY	=	C:Copy
PACKAGE	=	lha
PKOPTS	=   -acer3 a
RM		=	C:Delete ALL FORCE QUIET

TARGET	= Perception-IME.${PACKAGE}

OBJECTS = Documents Devs Libs Locale Utilities

# Preperations for a release
archive:
	@ ${MKDIR} dummy
	@ ${RM}	/${TARGET} dummy
	@ ${PACKAGE} ${PKOPTS} /${TARGET} ${OBJECTS}

clean:
	@ ${MKDIR}	Dummy
	@ ${RM}		Libs Devs Locale Utilities Dummy
	@ ${MKDIR}	Libs Devs Locale Utilities
	@ ${MKDIR}	Devs/Keymaps Locale/Charsets Locale/Languages
	@ ${MKDIR}	Utilities/Commodities Utilities/Commodities/Dockies
