/*
\\		"Perception"
//
\\		Language Driver Functionality
*/
#include "language.h"

STATIC CONST BYTE LanguageName[] = LIBRARY_NAME;

#define	LCSTATE_CHORDBUFF		(LCSTATE_EXPANDED+0)
#define	LCSTATE_JAMOBUFF		(LCSTATE_EXPANDED+1)

STATIC CONST struct TagItem ChordCandidatesTable[] =
{
	{TAG_END,	0L}
};
/*	Set the Primary System Input Language	*/
STATIC CONST struct TagItem DefaultSystemLanguage = { DEFAULT_SYSTEM_LANGUAGE, TRUE };
STATIC CONST unsigned char LanguageCatalogName[] = "Korean-Jamo";

/* Once-or-more Init & Exit calls will occur...
*/
void InitPerceptionHook(struct LIBRARY_CLASS *Self)
{
    APTR Language=NULL;
	APTR Catalog=NULL;
	struct TagItem LoadCatalog;

	LoadCatalog.ti_Tag=0L;
	LoadCatalog.ti_Data=0L;

	if(Self->IPerception)
		Language=Self->IPerception->ObtainLanguageContext((APTR)LanguageName,(APTR)&ExecPerceptionHook);
	if(Language)
	{
        Self->IPerception->SetLanguageContextAttr(Language,(APTR)&DefaultSystemLanguage);
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

ULONG ExecLanguageContextHook(struct Hook *h,struct LanguageContext LanguageContext,ULONG *Message)
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
		*Jamo=NULL;

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
				KDEBUG("Perception-IME//Korean.Language [Chord=%lx]\n",chord);
				Jamo=IUtility->FindTagItem(chord,ChordCandidatesTable);
				if(Jamo)
					chord=0L;
				chBuffer->ti_Data=chord;
			};
			if(Jamo)
				KDEBUG("Perception-IME//Korean.Language [Jamo=%lx:%lx]\n",Jamo,Jamo->ti_Data);
			break;
		default:
			KDEBUG("Perception-IME//Korean.Language::[%lx] is Unknown!\n", Message[0]);
			break;
	}

*/