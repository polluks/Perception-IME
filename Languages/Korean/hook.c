/*
\\		"Perception"
//
\\		Language Driver Functionality
*/
#include "language.h"

STATIC CONST BYTE LanguageName[] = LIBRARY_NAME;

STATIC CONST struct TagItem ChordCandidatesTable[] =
{
	{TAG_END,	0L}
};

/* Once-or-more Init & Exit calls will occur...
*/
void InitPerceptionHook(struct LIBRARY_CLASS *Self)
{
    APTR Context=NULL;

	if(!(Self->HPerception))
	{
		if(Self->IPerception)
			Context=Self->IPerception->ObtainLanguageContext((APTR)LanguageName,(APTR)&ExecLanguageHook);
		if(Context)
			Self->HPerception=Context;
	};

	return;
}

/**/
void ExitPerceptionHook(struct LIBRARY_CLASS *Self)
{
	if(!(Self->Library.lib_OpenCnt))
		if(Self->HPerception)
			Self->IPerception->ReleaseLanguageContext(Self->HPerception);
	return;
}

/**/
ULONG ExecLanguageHook(struct Hook *h,struct LanguageContext *LanguageContext,ULONG *Message)
{
	ULONG rc=0L;
	return(rc);
}

/**/
