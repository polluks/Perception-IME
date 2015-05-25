
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

/*
void  QueueSyllableCandidate(ULONG c,struct TagItem *Vector,APTR LanguageContext,struct LanguageContextHook *lch);
UpdateKanjiCandidacy(Vector,LanguageContext,lch);
UpdateVocabCandidacy(Vector,LanguageContext,lch);
*/

STATIC CONST struct TagItem SyllableMiniCandidates[] =
{
	/* mini a i u e o */
	{0X80000041,0x00003041},
	{0X80000049,0x00003043},
	{0X80000055,0x00003045},
	{0X80000045,0x00003047},
	{0X8000004F,0x00003049},
	/* mini tsu */
	{0X80545355,0x00003063},
	/* mini ya yu yo */
	{0X80005941,0x00003083},
	{0X80007955,0x00003085},
	{0X8000594F,0x00003087},
	/* mini wa */
	{0X80005741,0x0000308E},
	/* mini ka ke */
	{0X8000B441,0x00003095},
	{0X8000B445,0x00003096},
	{TAG_END,	TAG_END}
};

/*  Set the Primary System Input Language
*/
STATIC CONST unsigned char LanguageKanjiCatalog[] = "Ideographs";
STATIC CONST unsigned char LanguageVocabCatalog[] = "Vocabulary";

/*
				}
				//
                switch(mode)
				{
//					case LANG_MODE_KATAKANA:			//	Mode[2]
//					case LANG_MODE_HIRAGANA_KANJI:		//	Mode[1]
					default:							//  Mode[0]
						break;
				}
				break;
			default:
				break;
		}
	};

	return(rc);
}
*/
/**/
ULONG GetLCSTATEvalue(APTR Vector,ULONG Key,struct UtilityIFace *IUtility)
{
	ULONG rc=0L;
	struct TagItem *Item = NULL;

	if(Vector)
		Item=IUtility->FindTagItem(Key,Vector);
    if(Item)
		rc=Item->ti_Data;

	return(rc);
};

void  SetLCSTATEvalue(APTR Vector,ULONG Key,ULONG data,struct UtilityIFace *IUtility)
{
	struct TagItem *Item = NULL;

	if(Vector)
		Item=IUtility->FindTagItem(Key,Vector);
    if(Item)
		Item->ti_Data=data;

	return;
};

/*
		case LANGUAGE_TRANSLATE_ANSI:
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
*/
