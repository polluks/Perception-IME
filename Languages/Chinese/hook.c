/*
\\		"Perception"
//
\\		Language Driver Functionality
*/
#include "language.h"

STATIC CONST BYTE LanguageName[] = LIBRARY_NAME;

#define	LCSTATE_CHORDBUFF		(LCSTATE_EXPANDED+0)
#define	LCSTATE_HANZIBUFF		(LCSTATE_EXPANDED+2)

STATIC CONST struct TagItem ChordCandidatesTable[] =
{
	{TAG_END,	0L}
};
/**/

/*	Set the Primary System Input Language	*/
STATIC CONST struct TagItem DefaultLanguage = { DEFAULT_SYSTEM_LANGUAGE, TRUE };
STATIC CONST unsigned char LanguageCatalogName[] = "Chinese-Hanzi";

/* Once-or-more Init & Exit calls will occur...
*/
void InitPerceptionHook(struct LIBRARY_CLASS *Self)
{
    APTR Language=NULL;

	if(Self->IPerception)
		Language=Self->IPerception->ObtainLanguageContext((APTR)LanguageName,(APTR)&ExecPerceptionHook);
	if(Language)
	{
        Self->IPerception->SetLanguageContextAttr(Language,(APTR)&DefaultLanguage);
		Self->HPerception=Language;
	}
	return;
}

void ExitPerceptionHook(struct LIBRARY_CLASS *Self)
{
	if(Self->HPerception)
		Self->IPerception->ReleaseLanguageContext(Self->HPerception);

	return;
}

ULONG ExecPerceptionHook(struct Hook *h,struct LanguageContext *LanguageContext,ULONG *Message)
{
	ULONG rc=0L;
	return(rc);
}
/*
	ULONG rc=0L, *Message=m, VCommand=0L, chord=0L, emIndex=LCSTATE_EMITBUFF;
	struct PerceptionIFace	*IPerception= lch->PerceptionLib;
	struct UtilityIFace 	*IUtility	= lch->UtilityLib;
	struct TagItem *Vector=NULL, *LMode=NULL, *emit=NULL,
		*emBuffer=NULL, *chBuffer=NULL, *icBuffer=NULL,
		*Hanzi=NULL;

	if(LanguageContext)
	{
		VCommand=LCSTATE_VECTOR;
		Vector=(APTR)IPerception->GetLanguageContextAttr(LanguageContext,(APTR)&VCommand);
		if(Vector)
		{
			LMode=IUtility->FindTagItem(LCSTATE_LMODE,Vector);
			emit=IUtility->FindTagItem(LCSTATE_EMIT,Vector);
			chBuffer=IUtility->FindTagItem(LCSTATE_CHORDBUFF,Vector);
		};
	};

	switch(Message[0])
	{
		case LANGUAGE_TRANSLATE_AMIGA:
			break;
		case LANGUAGE_TRANSLATE_ANSI:
			if((Message[1] & 0xFF000000) == Message[1])
				chord=(Message[1] & 0xFF000000)>>24;
			if((chord & 0x0000007F)==(chord & 0x000000FF))
            {
				if((chord-0x00000041)<0x0000001A)
					chord=TAG_USER+(chBuffer->ti_Data << 8)+0x20+chord;
				if((chord-0x00000061)<0x0000001A)
					chord=TAG_USER+(chBuffer->ti_Data << 8)+chord;
			};
			if(chord & TAG_USER)
			{
				KDEBUG("Perception-IME//Chinese.Language [Chord=%lx]\n",chord);
				Hanzi=IUtility->FindTagItem(chord,ChordCandidatesTable);
				if(Hanzi)
					chord=0L;
				chBuffer->ti_Data=chord;
			};
			if(Hanzi)
				KDEBUG("Perception-IME//Chinese.Language [Hanzi=%lx:%lx]\n",Hanzi,Hanzi->ti_Data);
			break;
		default:
			KDEBUG("Perception-IME//Chinese.Language::[%lx] is Unknown!\n", Message[0]);
			break;
	}
*/
