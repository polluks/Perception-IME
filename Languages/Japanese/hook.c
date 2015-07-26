/*
\\		"Perception"
//
\\		Language Driver Functionality
*/
#include "language.h"

STATIC CONST BYTE LanguageName[] = LIBRARY_NAME;

extern CONST struct TagItem SyllableCandidates[];
extern CONST struct TagItem DictionaryOrderKey[];
extern CONST struct TagItem RadicalOrderKey[];

#define CODEPOINT_HIRAGANA_KEY		0x3040
#define CODEPOINT_KATAKANA_KEY		0x30A0
#define CODEPOINT_KANADIFF_KEY		0x0060

#define	LCSTATE_Syllable			(TAG_USER+0x00+LCSTATE_EXPANDED)

int32 IsValidVanilla(uint32 vanilla);
int32 FindSyllableCandidate(uint32 Key, struct UtilityIFace *IUtility);
int32 TransformSyllableCodepoint(uint32 Kana);
int32 QueueSyllableCandidate(uint32 codepoint,struct LanguageContext *LanguageContext);
int32 UpdateKanjiCandidates(uint32 codepoint,struct LanguageContext *LanguageContext);
int32 UpdateVocabCandidates(uint32 codepoint,struct LanguageContext *LanguageContext);

/* Once-or-more Init & Exit calls will occur...
*/
void InitPerceptionHook(struct LIBRARY_CLASS *Self)
{
    APTR Context=NULL;//, Resource=NULL;

	if(!(Self->HPerception))
	{
		if(Self->IPerception)
			Context=Self->IPerception->ObtainLanguageContext((APTR)LanguageName,(APTR)&ExecLanguageHook);
		if(Context)
			Self->HPerception=Context;
//		if(Self->HPerception)
//			Resource=Self->IExec->
	};

	return;
}

/**/
void ExitPerceptionHook(struct LIBRARY_CLASS *Self)
{
	if(!(Self->Library.lib_OpenCnt))
	{
//		if(Self->RPerception)
//			Self->IExec->
		if(Self->HPerception)
			Self->IPerception->ReleaseLanguageContext(Self->HPerception);
	}
	return;
}

/*
*/
/**/
ULONG ExecLanguageHook(struct Hook *h,struct LanguageContext *LanguageContext,ULONG *Message)
{
	ULONG rc=0L, Mode=0L, Syllable=0L, xc=0L, Kana=0L;
	struct TagItem *Vector=NULL;

	if(LanguageContext)
		Vector=(APTR)LanguageContext->IPerception->GetLanguageContextAttr((APTR)LanguageContext,LCSTATE_VECTOR);
	if(Message)
	{
		Mode=LanguageContext->IPerception->GetLanguageContextAttr((APTR)LanguageContext,(ULONG)LCSTATE_LMODE);
		Syllable=LanguageContext->IPerception->GetLanguageContextAttr((APTR)LanguageContext,(ULONG)LCSTATE_Syllable);
		switch(Message[0])
		{
            case TRANSLATE_AMIGA: // Unmapped Raw Keyboard Input
				switch(Message[1])
                {
					case 0x00000000:		// Zenkaku~Hankaku
					case 0x40000000:		// Space
					case 0x43000000:		// Enter
					case 0x44000000:		// Return
					case 0x7F000000:		// Delete
					case 0x01000000:		// Hiragana/Katakana
					case 0x01010000:		// Henkan
					case 0x01020000:		// MuHenkan
					default:
						break;
				};
				KDEBUG("LOCALE:/Japanese.Language::LanguageHook() [AMIGA:%lx:%lx]\n",Message[1],Message[2]);
				break;
            case TRANSLATE_ANSI:  // Mapped "Vanilla" Keyboard Input
                xc=IsValidVanilla((Message[1] >> 24) & 0x7F);
				if(((Message[1] >> 24) & 0x7F)==0x08)
				{
					Syllable=Syllable >> 8;
				}else{
					switch(Syllable)
					{
						case 0x00:
                            switch(xc)
							{
								case 0x61: // A
								case 0x69: // I
								case 0x75: // U
								case 0x65: // E
								case 0x6F: // O
									Kana=FindSyllableCandidate(xc,LanguageContext->IUtility);
									break;
								default:
									Syllable=(Syllable << 8)+(xc & 0x7F);
									break;
							};
							break;
						case 0x6E:
                            switch(xc)
							{
								case 0x61: // A
								case 0x69: // I
								case 0x75: // U
								case 0x65: // E
								case 0x6F: // O
									Kana=FindSyllableCandidate((Syllable << 8)+(xc & 0x7F),LanguageContext->IUtility);
									break;
								case 0x79: // Y
									Syllable=(Syllable << 8)+(xc & 0x7F);
									break;
								default:
									Kana = 0x00003093;
									Syllable=(xc & 0x7F);
									break;
							};
							break;
						default:
							if(Syllable==xc)
							{
								Kana = 0x00003063;
							}else{
                                if(xc)
								{
									Syllable=(Syllable << 8)+(xc & 0x7F);
									Kana=FindSyllableCandidate(Syllable,LanguageContext->IUtility);
								}else{
									Syllable=0L;
								};
							};
							break;
					};
				};
				if(Kana)
					Syllable=0L;
				Kana=TransformSyllableCodepoint(Kana);
				LanguageContext->IPerception->SetLanguageContextAttr((APTR)LanguageContext,(int32)LCSTATE_Syllable,(int32)Syllable);
				KDEBUG("LOCALE:/Japanese.Language/:Hiragana [ASCII=%lx,CodePoints=%lx:%lx]\n",
					Syllable,(Kana >> 16),(Kana & 0xFFFF));
				break;
			default:
				break;
		};
		LanguageContext->IPerception->SetLanguageContextAttr((APTR)LanguageContext,(int32)LCSTATE_LMODE,(int32)Mode);
    };
	return(rc);
}

/**/
int32 IsValidVanilla(uint32 vanilla)
{
	int32 rc=0L;

	switch(vanilla)
	{
		case 0x7A: // z
		case 0x79: // y
		case 0x78: // x
		case 0x77: // w
		case 0x76: // v
		case 0x75: // u
		case 0x74: // t
		case 0x73: // s
		case 0x72: // r
		case 0x71: // q
		case 0x70: // p
		case 0x6F: // o
		case 0x6E: // n
		case 0x6D: // m
		case 0x6C: // l
		case 0x6B: // k
		case 0x6A: // j
		case 0x69: // i
		case 0x68: // h
		case 0x67: // g
		case 0x66: // f
		case 0x65: // e
		case 0x64: // d
		case 0x63: // c
		case 0x62: // b
		case 0x61: // a
			rc=vanilla;
			break;
		case 0x5A: // Z
		case 0x59: // Y
		case 0x58: // X
		case 0x57: // W
		case 0x56: // V
		case 0x55: // U
		case 0x54: // T
		case 0x53: // S
		case 0x52: // R
		case 0x51: // Q
		case 0x50: // P
		case 0x4F: // O
		case 0x4E: // N
		case 0x4D: // M
		case 0x4C: // L
		case 0x4B: // K
		case 0x4A: // J
		case 0x49: // I
		case 0x48: // H
		case 0x47: // G
		case 0x46: // F
		case 0x45: // E
		case 0x44: // D
		case 0x43: // C
		case 0x42: // B
		case 0x41: // A
			rc=vanilla+0x20;
			break;
		default:
			rc=0L;
			break;
	};
	return(rc);
}
/**/
int32 FindSyllableCandidate(uint32 Key, struct UtilityIFace *IUtility)
{
	int32 rc=0L;
	struct TagItem *item=NULL;

	if(IUtility)
		item=IUtility->FindTagItem(TAG_USER|Key,SyllableCandidates);
	if(item)
		rc=item->ti_Data;

	return(rc);
}

/**/
int32 TransformSyllableCodepoint(uint32 Kana)
{
	int32 rc=0L, h=0L, l=0L;

	if(Kana & 0xFFFF0000)
		h=(Kana & 0xFFFF0000) >> 16;
	if(Kana & 0x0000FFFF)
		l=(Kana & 0x0000FFFF);
	if(h)
		h = TransformSyllableCodepoint(h);	// Recursive Transform
	if(l)
	{
		if((l-CODEPOINT_HIRAGANA_KEY)<CODEPOINT_KANADIFF_KEY)
			l+=CODEPOINT_KANADIFF_KEY;
		if((l-CODEPOINT_KATAKANA_KEY)<CODEPOINT_KANADIFF_KEY)
			l-=CODEPOINT_KANADIFF_KEY;
	}
	rc=(h << 16)|l;

	return(rc);
};

/**/
int32 QueueSyllableCandidate(uint32 codepoint,struct LanguageContext *LanguageContext)
{
	int32 rc=0L;
	return(rc);
};

/**/
int32 UpdateKanjiCandidates(uint32 codepoint,struct LanguageContext *LanguageContext)
{
	int32 rc=0L;
	return(rc);
};

/**/
int32 UpdateVocabCandidates(uint32 codepoint,struct LanguageContext *LanguageContext)
{
	int32 rc=0L;
	return(rc);
};

/**/
