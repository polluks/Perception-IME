#
#	'Perception.Library' Build Scripting
#
MAKE	=	SDK:C/make
MKDIR	=	C:Makedir
COPY	=	C:Copy
RM		=	C:Delete ALL FORCE QUIET

# Preparations for a release
world: prepack all
	${COPY}		Library/#?.library#?			Release/Libs
	${COPY}		Keymaps/#?_UTF-8				Release/Devs/Keymaps
	${COPY}		Charsets/#?/#?.charset#?		Release/Locale/Charsets
	${COPY}		Languages/#?/#?.language#?		Release/Locale/Languages
	${COPY}		Docky/#?.docky#?				Release/Utilities/Commodities/Dockies/

release: world
	@ ${MAKE}	-C Release		-f GNUmakefile

all:
	@ ${MAKE} -C Devkit 	-f GNUmakefile
	@ ${MAKE} -C Library	-f GNUmakefile
	@ ${MAKE} -C Charsets	-f GNUmakefile -j 2
	@ ${MAKE} -C Catalogs	-f GNUmakefile -j 2
	@ ${MAKE} -C Languages	-f GNUmakefile -j 2
	@ ${MAKE} -C Docky		-f GNUmakefile

autodocs: all
	@ ${MAKE} -C Library	-f GNUmakefile autodoc

prepack:
	@ ${MAKE} -C Release	-f GNUmakefile clean

clean:
	@ ${MAKE} -C Docky		-f GNUmakefile clean
	@ ${MAKE} -C Languages	-f GNUmakefile clean
	@ ${MAKE} -C Catalogs	-f GNUmakefile clean
	@ ${MAKE} -C Charsets	-f GNUmakefile clean
	@ ${MAKE} -C Library	-f GNUmakefile clean
	@ ${MAKE} -C Devkit 	-f GNUmakefile clean

install: release
	${COPY}		Release/Libs/perception.library.debug				LIBS:Perception.Library
	${COPY}		Release/Devs/Keymaps/#?_UTF-8						DEVS:Keymaps
	@ ${MAKE}	-C Library		-f GNUmakefile install
	@ ${MAKE}	-C Languages	-f GNUmakefile install
	@ ${MAKE}	-C Charsets		-f GNUmakefile install
