/*
\\		"Perception"
//
\\		Language Driver Functionality
*/
#include "language.h"

STATIC CONST BYTE LanguageName[] = LIBRARY_NAME;

/* Once-or-more Init & Exit calls will occur...
*/
void InitPerceptionHook(struct LIBRARY_CLASS *Self)
{
    APTR Context=NULL;

	if(Self->IPerception)
		Context=Self->IPerception->ObtainLanguageContext((APTR)LanguageName,(APTR)&ExecLanguageHook);
	if(Context)
		Self->HPerception=Context;

	return;
}

void ExitPerceptionHook(struct LIBRARY_CLASS *Self)
{
	if(Self->HPerception)
		Self->IPerception->ReleaseLanguageContext(Self->HPerception);
	return;
}

ULONG ExecLanguageHook(struct Hook *h,struct LanguageContext *LanguageContext,ULONG *Message)
{
	ULONG rc=0L;
	return(rc);
}
