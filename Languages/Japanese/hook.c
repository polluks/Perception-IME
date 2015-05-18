/*
\\		"Perception"
//
\\		Language Driver Functionality
*/
#include "language.h"

STATIC CONST BYTE LanguageName[] = LIBRARY_NAME;

#define	LCSTATE_Syllable		(LCSTATE_EXPANDED)
#define	LCSTATE_IDEOGRAPH_IDX	(LCSTATE_EXPANDED+1)
#define	LCSTATE_IDEOGRAPH_BUFF	(LCSTATE_EXPANDED+2)
#define	IDEOGRAPH_CNT			(32)
#define	LCSTATE_IDEOGRAPH_MAX	(LCSTATE_IDEOGRAPH_BUFF+IDEOGRAPH_CNT)

#define LC_MODE_ROMAJI_DIRECT	(2)
#define LC_MODE_KANA_KANJI		(0)
#define LC_MODE_KATAKANA_ONLY	(1)

#define CODEPOINT_HIRAGANA_KEY		0x3040
#define CODEPOINT_KATAKANA_KEY		0x30A0
#define	CODEPOINT_KANATOGGLE_MASK	0x00E0	// Reversible Hiragana<->Katakana Transform Key

ULONG GetLCSTATEbyValue(APTR Vector,ULONG Key,struct UtilityIFace *IUtility);
void  SetLCSTATEbyValue(APTR Vector,ULONG Key,struct UtilityIFace *IUtility,ULONG data);
ULONG FindSyllableCandidate(ULONG Key,struct UtilityIFace *IUtility);

/*

void  QueueSyllableCandidate(ULONG c,struct TagItem *Vector,APTR LanguageContext,struct LanguageContextHook *lch);

UpdateKanjiCandidacy(Vector,LanguageContext,lch);
UpdateVocabCandidacy(Vector,LanguageContext,lch);
*/
/*	The SyllableCandidate Table has TagItems mapping Romaji to Hiragana
*/
STATIC CONST struct TagItem SyllableCandidates[] =
{
	{0X00000000,0x00000000},{0X0000003D,0x000030A0},		    				/* NULL, EQUALS */
	{0X00000061,0x00003042},                            						/* A			*/
	{0X00000069,0x00003044},                            						/* I			*/
	{0X00000075,0x00003046},                            						/* U			*/
	{0X00000065,0x00003048},                            						/* E	 		*/
	{0X0000006F,0x0000304A},                            						/* O			*/
	{0X00006B61,0x0000304B},{0X00006761,0x0000304C},    						/* KA/GA		*/
	{0X00006B69,0x0000304D},{0X00006769,0x0000304E},    						/* KI/GI		*/
	{0X00006B75,0x0000304F},{0X00006775,0x00003050},    						/* KU/GU		*/
	{0X00006B65,0x00003051},{0X00006765,0x00003052},    						/* KE/GE		*/
	{0X00006B6F,0x00003053},{0X0000676F,0x00003054},    						/* KO/GO		*/
    {0X00007361,0x00003055},{0X00007A61,0x00003056},							/* SA/ZA		*/
	{0X00736869,0x00003057},{0X00006A69,0x00003058},							/* SHI/JI		*/
	{0X00007369,0x00003057},{0X00007A69,0x00003058},							/* SI/ZI		*/
	{0X00007375,0x00003059},{0X00007A75,0x0000305A},							/* SU/ZU		*/
	{0X00007365,0x0000305B},{0X00007A65,0x0000305C},            				/* SE/ZE		*/
	{0X0000736F,0x0000305D},{0X00007A6F,0x0000305E},							/* SO/ZO		*/
	{0X00007461,0x0000305F},{0X00006461,0x00003060},							/* TA/DA		*/
	{0X00636869,0x00003061},{0X00006469,0x00003062},							/* CHI/DI		*/
	{0X00006369,0x00003061},{0X00007469,0x00003061},							/* CI/TI		*/
	{0X00747375,0x00003064},{0X00647A75,0x00003065},							/* TSU/DZU		*/
	{0X00007475,0x00003064},{0X00006475,0x00003065},							/* TU/DU		*/
	{0X00007465,0x00003066},{0X00006465,0x00003067},							/* TE/DE		*/
	{0X0000746F,0x00003068},{0X0000646F,0x00003069},							/* TO/DO		*/
	{0X00006E61,0x0000306A},													/* NA			*/
	{0X00006E69,0x0000306B},													/* NI			*/
	{0X00006E75,0x0000306C},													/* NU			*/
	{0X00006E65,0x0000306D},													/* NE			*/
	{0X00006E6F,0x0000306E},													/* NO			*/
	{0X00006861,0x0000306F},{0X00006261,0x00003070},{0X00007061,0x00003071},	/* HA/BA/PA		*/
	{0X00006869,0x00003072},{0X00006269,0x00003073},{0X00007069,0x00003074},	/* HI/BI/PI		*/
	{0X00006675,0x00003075},													/* FU			*/
	{0X00006875,0x00003075},{0X00006275,0x00003076},{0X00007075,0x00003077},	/* HU/BU/PU		*/
	{0X00006865,0x00003078},{0X00006265,0x00003079},{0X00007065,0x0000307A},	/* HE/BE/PE		*/
	{0X0000686F,0x0000307B},{0X0000626F,0x0000307C},{0X0000706F,0x0000307D},	/* HO/BO/PO		*/
	{0X00006D61,0x0000307E},													/* MA			*/
	{0X00006D69,0x0000307F},													/* MI			*/
	{0X00006D75,0x00003080},													/* MU			*/
	{0X00006D65,0x00003081},													/* ME			*/
	{0X00006D6F,0x00003082},													/* MO			*/
	{0X00007961,0x00003084},													/* YA			*/
	{0X00007975,0x00003086},													/* YU			*/
	{0X0000796F,0x00003088},													/* YO			*/
	{0X00007261,0x00003089},													/* RA			*/
	{0X00007269,0x0000308A},													/* RI			*/
	{0X00007275,0x0000308B},													/* RU			*/
	{0X00007265,0x0000308C},													/* RE			*/
	{0X0000726F,0x0000308D},													/* RO			*/
	{0X00007761,0x0000308F},													/* WA			*/
	{0X00007769,0x00003090},													/* WI			*/
	{0X00007765,0x00003091},													/* WE			*/
	{0X0000776F,0x00003092},													/* WO			*/
	{0X00006E6E,0x00003093},													/* N			*/
	{0X00007675,0x00003094},													/* VU			*/
	{0X00007661,0x000030F7},													/* VA			*/
	{0X00007669,0x000030F8},													/* VI			*/
	{0X00007665,0x000030F9},													/* VE			*/
	{0X0000766F,0x000030FA},													/* RO			*/
/*
		Syllable Chords follow
*/
	{0X006B7961,0x304D3083},{0X00677961,0x304E3083},    						/* KYA/GYA		*/
	{0X006B7975,0x304D3085},{0X00677975,0x304E3085},    						/* KYU/GYU		*/
	{0X006B796F,0x304D3087},{0X0067796F,0x304E3087},    						/* KYO/GYO		*/
	{0X00736861,0x30573083},{0X00006A61,0x30583083},{0X00647961,0x30583083},	/* SHA/JA/DYA	*/
	{0X00737961,0x30573083},{0X006A7961,0x30583083},							/* SYA/JYA		*/
	{0X00736875,0x30573085},{0X00006A75,0x30583085},{0X00647975,0x30583085},	/* SHU/JU/DYU	*/
	{0X00737975,0x30573085},{0X006A7975,0x30583085},							/* SYU/JYU		*/
	{0X00006A65,0x30583047},													/* JE			*/
	{0X0073686F,0x30573087},{0X00006A6F,0x30583087},{0X0064796F,0x30583087},	/* SHO/JO/DYO	*/
    {0X0073796F,0x30573087},{0X006A796F,0x30583087},							/* SYO/JYO		*/
	{0X00636861,0x30613083},{0X00637961,0x30613083},							/* CHA/CYA		*/
	{0X00636875,0x30613085},{0X00637975,0x30613085},							/* CHU/CYU		*/
	{0X0063686F,0x30613087},{0X0063796F,0x30613087},							/* CHO/CYO		*/
	{0X006E7961,0x306B3083},													/* NYA			*/
	{0X006E7975,0x306B3085},													/* NYU			*/
	{0X006E796F,0x306B3087},													/* NYO			*/
	{0X00687961,0x30723083},{0X00627961,0x30733083},{0X00707961,0x30743083},	/* HYA/BYA/PYA	*/
	{0X00687975,0x30723085},{0X00627975,0x30733085},{0X00707975,0x30743085},	/* HYU/BYU/PYU	*/
	{0X0068796F,0x30723087},{0X0062796F,0x30733087},{0X0070796F,0x30743087},	/* HYO/BYO/PYO	*/
	{0X006D7961,0x307F3083},													/* MYA			*/
	{0X006D7975,0x307F3085},													/* MYU			*/
	{0X006D796F,0x307F3087},													/* MYO			*/
	{0X00727961,0x308A3083},													/* RYA			*/
	{0X00727975,0x308A3085},													/* RYU			*/
	{0X0072796F,0x308A3087},													/* RYO			*/
	{TAG_END,	TAG_END}
};

STATIC CONST struct TagItem SyllableMiniCandidates[] =
{
	/* mini a i u e o */
	{0X00000041,0x00003041},
	{0X00000049,0x00003043},
	{0X00000055,0x00003045},
	{0X00000045,0x00003047},
	{0X0000004F,0x00003049},
	/* mini tsu */
	{0X00545355,0x00003063},
	/* mini ya yu yo */
	{0X00005941,0x00003083},
	{0X00007955,0x00003085},
	{0X0000594F,0x00003087},
	/* mini wa */
	{0X00005741,0x0000308E},
	/* mini ka ke */
	{0X0000B441,0x00003095},
	{0X0000B445,0x00003096},
	{TAG_END,	TAG_END}
};

/*  Set the Primary System Input Language
*/
STATIC CONST unsigned char LanguageKanjiCatalog[] = "Ideographs";
STATIC CONST unsigned char LanguageVocabCatalog[] = "Vocabulary";

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

/*
	This is where ALL of the Language Specific "Magic" happens...

	Please note...Characters are given in the HIGHEST octet of a 32bit value.

	CodePoints are given using a full 32bit value as EmitBuffer ti_Data points for UTF8 conversion.

	the return value is the number of Emitted CodePoints

	DEBUG: RE-FACTORING IN PROGRESS... THIS FUNCTION IS ACTIVELY BROKEN...

*/
ULONG ExecLanguageHook(struct Hook *h,struct LanguageContext *LanguageContext,ULONG *Message)
{
	ULONG rc=0L, xc=0L, Syllable=0L, Kana=0L;
	struct TagItem *Vector=NULL;

	if(LanguageContext)
		Vector=(APTR)LanguageContext->IPerception->GetLanguageContextAttr(LanguageContext,LCSTATE_VECTOR);
	if(Message)
	{
		switch(Message[0])
		{
            case LANGUAGE_TRANSLATE_AMIGA:
				KDEBUG("Japanese.Language::ExecLanguageHook()[LANGUAGE_TRANSLATE_AMIGA]\n");
				break;
            case LANGUAGE_TRANSLATE_ANSI:
				KDEBUG("Japanese.Language::ExecLanguageHook()[LANGUAGE_TRANSLATE_ANSI]\n");
				Syllable = GetLCSTATEbyValue(Vector,LCSTATE_Syllable,LanguageContext->IUtility);
			    if((Message[1] & 0xFF000000) == Message[1])
    			{
					if(((Message[1] >> 24)-0x00000061)<0x0000001B)
						xc = TAG_USER | (Message[1] >> 24);
					if(((Message[1] >> 24)-0x00000041)<0x0000001B)
						xc = TAG_USER | ((Message[1] >> 24)+0x20);
				}
				switch(Syllable)
				{
					case 0x00000000: // No Syllable
						switch(xc)
						{
							case 0x00000061: // A
							case 0x00000069: // I
							case 0x00000075: // U
							case 0x00000065: // E
							case 0x0000006F: // O
								Kana = FindSyllableCandidate(Syllable,LanguageContext->IUtility);
                                break;
							default:
								Syllable = (Syllable << 8)+(0x7F & xc);
								break;
						}
						break;
					case 0x0000006E: // 'N' Singular Usage exception
						switch(xc)
						{
							case 0x00000061: // A
							case 0x00000069: // I
							case 0x00000075: // U
							case 0x00000065: // E
							case 0x0000006F: // O
								Syllable = (Syllable << 8)+(0x7F & xc);
								Kana = FindSyllableCandidate(Syllable,LanguageContext->IUtility);
								break;
							case 0x00000079: // Y
								Syllable = (Syllable << 8)+(0x7F & xc);
								break;
							default:
								Kana = 0x00003093;
								Syllable = (0x7F && xc);
								break;
						}
						break;
					default:
						if(Syllable==xc)
						{
							Kana = 0x00003063;
						}else{
							Syllable = (Syllable << 8)+(0x7F & xc);
							Kana = FindSyllableCandidate(Syllable,LanguageContext->IUtility);
						};
						break;
				}
				SetLCSTATEbyValue(Vector,LCSTATE_Syllable,LanguageContext->IUtility,Syllable);
				break;
			default:
				break;
		}
	}

	return(rc);
}

ULONG GetLCSTATEbyValue(APTR Vector,ULONG Key,struct UtilityIFace *IUtility)
{
	ULONG rc=0L;
	struct TagItem *Item = NULL;

	if(Vector)
		Item=IUtility->FindTagItem(Key,Vector);
    if(Item)
		rc=Item->ti_Data;

	return(rc);
};

void  SetLCSTATEbyValue(APTR Vector,ULONG Key,struct UtilityIFace *IUtility,ULONG data)
{
	struct TagItem *Item = NULL;

	if(Vector)
		Item=IUtility->FindTagItem(Key,Vector);
    if(Item)
		Item->ti_Data=data;

	return;
};

ULONG FindSyllableCandidate(ULONG Key,struct UtilityIFace *IUtility)
{
	ULONG rc = 0L;
	struct TagItem *Candidate = NULL;

	if(Key)
		Candidate=IUtility->FindTagItem((Key & 0x7FFFFFFF),SyllableCandidates);
	if(Candidate)
		rc=Candidate->ti_Data;

	return(rc);
};

/*
	ULONG rc=0L, *Message=m, c=0L, Syllable=0L, Kana=0L;

	KDEBUG("Japanese.Language[ExecLanguageHook]()[Message]\n");
	if(Message)
	{
	}
	KDEBUG("ExecLanguageContextHook[Syllable =%lx][Character=%lx]\n",Syllable,c);

	return(rc);
};

	ULONG rc=0L, *Message=m, c=0L, Syllable=0L, Kana=0L;
	struct PerceptionIFace	*IPerception= lch->PerceptionLib;
	struct TagItem *Vector = NULL, VCommand;

	switch(Message[0])
	{
		case LANGUAGE_TRANSLATE_AMIGA:
			KDEBUG("Japanese.Language::ExecLanguageHook()[LANGUAGE_TRANSLATE_AMIGA]\n");
			switch(c)
			{
//	Unofficial Mappings used with Developer Restricted Keymap.Library.Kmod
				case	0x00:	//	Change the Constant here for this Key when officially mapped
					KDEBUG("Perception-IME//Japanese.Language::Hankaku~Zenkaku\n");
					break;
				case	0x78:	//	Change the Constant here for this Key when officially mapped
					KDEBUG("Perception-IME//Japanese.Language::Romaji~Hiragana~Katakana\n");
					break;
				case	0x79:	//	Change the Constant here for this Key when officially mapped
					KDEBUG("Perception-IME//Japanese.Language::Henkan\n");
					break;
				case	0x7A:	//	Change the Constant here for this Key when officially mapped
					KDEBUG("Perception-IME//Japanese.Language::MuHenkan\n");
					break;
//	Official Mappings that need to be recognised
				case	0x08:	//  Official Key mapping
					KDEBUG("Perception-IME//Japanese.Language::Backspace\n");
					break;
				case	0x40:	//  Official Key mapping
					KDEBUG("Perception-IME//Japanese.Language::Space\n");
					break;
				case	0x43:	//  Official Key mapping
					KDEBUG("Perception-IME//Japanese.Language::Enter\n");
					break;
				case	0x44:	//  Official Key mapping
					KDEBUG("Perception-IME//Japanese.Language::Return\n");
					break;
				case	0x7F:	//  Official Key mapping
					KDEBUG("Perception-IME//Japanese.Language::Delete\n");
					break;
//  Anything Unhandled as an AMIGA key translation				default:
  					KDEBUG("Perception-IME//Japanese.Language::[%lx] is Unknown!\n", Message[1]);
					break;
			};break;
		case LANGUAGE_TRANSLATE_ANSI:
			KDEBUG("Japanese.Language::ExecLanguageHook()[LANGUAGE_TRANSLATE_ANSI]\n");
			KDEBUG("Japanese.Language::ExecLanguageHook()[LCSTATE 'Syllable' Write]\n");
			KDEBUG("Japanese.Language::ExecLanguageHook()[%lx]\n",Kana);
			if(Kana)
			{
				if(Kana && 0x7FFF0000)
					QueueSyllableCandidate((Kana >> 16),Vector,LanguageContext,lch);
				if(Kana && 0x00007FFF)
					QueueSyllableCandidate((Kana & 0xFFFF),Vector,LanguageContext,lch);
				UpdateKanjiCandidacy(Vector,LanguageContext,lch);
				UpdateVocabCandidacy(Vector,LanguageContext,lch);

			}
			break;
		default:
			KDEBUG("Japanese.Language - Unknown Command Path");
			break;
	}

void  QueueSyllableCandidate(ULONG c,struct TagItem *Vector,APTR LanguageContext,struct LanguageContextHook *lch)
{
	KDEBUG("QueueSyllableCandidate[CodePoint=%lx]\n",c);
	return;
};

ULONG FindKanjiCandidates(void)
{
	ULONG rc=0L;
	return(rc);
};

ULONG FindVocabCandidates(void)
{
	ULONG rc=0L;
	return(rc);
};

*/
