#
#	'Perception.Library' Build Scripting
#
MAKE		=	make
PYTHON		=	Python:Python
MKDIR		=	Makedir
RM 			=	Delete ALL QUIET

main:
	XadUnFile /Resources/Unicode/Unihan.zip /Catalogs/ OW
	$(PYTHON) CodePointParser.py

clean:
	@ Echo >dummy.txt " "
	@ Echo >dummy.cd " "
	@ Echo >dummy.ct " "
	@ $(RM) #?.txt #?.cd #?.ct ALL FORCE QUIET

#
