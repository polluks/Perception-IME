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

# Preperations for a release
archive:
	@ ${MKDIR} dummy
	@ ${RM}	/${TARGET} dummy
	@ ${PACKAGE} ${PKOPTS} /${TARGET} ${OBJECTS}

clean:
	@ ${MKDIR}	Dummy
	@ ${RM}		Libs Devs Locale Dummy
	@ ${MKDIR}	Libs Devs Locale
	@ ${MKDIR}	Devs/Keymaps Locale/Charsets Locale/Languages
	@ ${MKDIR}	Release/Utilities/Commodities/Dockies/
