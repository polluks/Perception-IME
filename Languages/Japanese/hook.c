/*
\\		"Perception"
//
\\		Language Driver Functionality
*/
#include "language.h"

STATIC CONST BYTE LanguageName[] = LIBRARY_NAME;

#define CODEPOINT_HIRAGANA_KEY		0x3040
#define CODEPOINT_KATAKANA_KEY		0x30A0
#define CODEPOINT_KANATOGGLE_KEY	0x00E0
#define	CODEPOINT_KANAMASK_KEY		0xFF1F

#define	LCSTATE_Syllable			(TAG_USER+0x00+LCSTATE_EXPANDED)

ULONG FindSyllableCandidate(ULONG Key, struct UtilityIFace *IUtility);
ULONG TransformSyllableCodepoint(ULONG Kana,ULONG Base);
ULONG QueueSyllableCandidate(ULONG codepoint,struct LanguageContext *LanguageContext);
ULONG UpdateKanjiCandidates(ULONG codepoint,struct LanguageContext *LanguageContext);
ULONG UpdateVocabCandidates(ULONG codepoint,struct LanguageContext *LanguageContext);

/*	The SyllableCandidate Table has TagItems mapping Romaji to Hiragana
*/
STATIC CONST struct TagItem SyllableCandidates[] =
{
	{0X80000000,0x00000000},{0X8000003D,0x000030A0},		    				/* NULL, EQUALS */
	{0X80000061,0x00003042},                            						/* A			*/
	{0X80000069,0x00003044},                            						/* I			*/
	{0X80000075,0x00003046},                            						/* U			*/
	{0X80000065,0x00003048},                            						/* E	 		*/
	{0X8000006F,0x0000304A},                            						/* O			*/
	{0X80006B61,0x0000304B},{0X80006761,0x0000304C},    						/* KA/GA		*/
	{0X80006B69,0x0000304D},{0X80006769,0x0000304E},    						/* KI/GI		*/
	{0X80006B75,0x0000304F},{0X80006775,0x00003050},    						/* KU/GU		*/
	{0X80006B65,0x00003051},{0X80006765,0x00003052},    						/* KE/GE		*/
	{0X80006B6F,0x00003053},{0X8000676F,0x00003054},    						/* KO/GO		*/
    {0X80007361,0x00003055},{0X80007A61,0x00003056},							/* SA/ZA		*/
	{0X80736869,0x00003057},{0X80006A69,0x00003058},							/* SHI/JI		*/
	{0X80007369,0x00003057},{0X80007A69,0x00003058},							/* SI/ZI		*/
	{0X80007375,0x00003059},{0X80007A75,0x0000305A},							/* SU/ZU		*/
	{0X80007365,0x0000305B},{0X80007A65,0x0000305C},            				/* SE/ZE		*/
	{0X8000736F,0x0000305D},{0X80007A6F,0x0000305E},							/* SO/ZO		*/
	{0X80007461,0x0000305F},{0X80006461,0x00003060},							/* TA/DA		*/
	{0X80636869,0x00003061},{0X80006469,0x00003062},							/* CHI/DI		*/
	{0X80006369,0x00003061},{0X80007469,0x00003061},							/* CI/TI		*/
	{0X80747375,0x00003064},{0X80647A75,0x00003065},							/* TSU/DZU		*/
	{0X80007475,0x00003064},{0X80006475,0x00003065},							/* TU/DU		*/
	{0X80007465,0x00003066},{0X80006465,0x00003067},							/* TE/DE		*/
	{0X8000746F,0x00003068},{0X8000646F,0x00003069},							/* TO/DO		*/
	{0X80006E61,0x0000306A},													/* NA			*/
	{0X80006E69,0x0000306B},													/* NI			*/
	{0X80006E75,0x0000306C},													/* NU			*/
	{0X80006E65,0x0000306D},													/* NE			*/
	{0X80006E6F,0x0000306E},													/* NO			*/
	{0X80006861,0x0000306F},{0X80006261,0x00003070},{0X80007061,0x00003071},	/* HA/BA/PA		*/
	{0X80006869,0x00003072},{0X80006269,0x00003073},{0X80007069,0x00003074},	/* HI/BI/PI		*/
	{0X80006675,0x00003075},													/* FU			*/
	{0X80006875,0x00003075},{0X80006275,0x00003076},{0X80007075,0x00003077},	/* HU/BU/PU		*/
	{0X80006865,0x00003078},{0X80006265,0x00003079},{0X80007065,0x0000307A},	/* HE/BE/PE		*/
	{0X8000686F,0x0000307B},{0X8000626F,0x0000307C},{0X8000706F,0x0000307D},	/* HO/BO/PO		*/
	{0X80006D61,0x0000307E},													/* MA			*/
	{0X80006D69,0x0000307F},													/* MI			*/
	{0X80006D75,0x00003080},													/* MU			*/
	{0X80006D65,0x00003081},													/* ME			*/
	{0X80006D6F,0x00003082},													/* MO			*/
	{0X80007961,0x00003084},													/* YA			*/
	{0X80007975,0x00003086},													/* YU			*/
	{0X8000796F,0x00003088},													/* YO			*/
	{0X80007261,0x00003089},													/* RA			*/
	{0X80007269,0x0000308A},													/* RI			*/
	{0X80007275,0x0000308B},													/* RU			*/
	{0X80007265,0x0000308C},													/* RE			*/
	{0X8000726F,0x0000308D},													/* RO			*/
	{0X80007761,0x0000308F},													/* WA			*/
	{0X80007769,0x00003090},													/* WI			*/
	{0X80007765,0x00003091},													/* WE			*/
	{0X8000776F,0x00003092},													/* WO			*/
	{0X80006E6E,0x00003093},													/* N			*/
	{0X80007675,0x00003094},													/* VU			*/
	{0X80007661,0x000030F7},													/* VA			*/
	{0X80007669,0x000030F8},													/* VI			*/
	{0X80007665,0x000030F9},													/* VE			*/
	{0X8000766F,0x000030FA},													/* RO			*/
/*
		Syllable Chords follow
*/
	{0X806B7961,0x304D3083},{0X80677961,0x304E3083},    						/* KYA/GYA		*/
	{0X806B7975,0x304D3085},{0X80677975,0x304E3085},    						/* KYU/GYU		*/
	{0X806B796F,0x304D3087},{0X8067796F,0x304E3087},    						/* KYO/GYO		*/
	{0X80736861,0x30573083},{0X80006A61,0x30583083},{0X80647961,0x30583083},	/* SHA/JA/DYA	*/
	{0X80737961,0x30573083},{0X806A7961,0x30583083},							/* SYA/JYA		*/
	{0X80736875,0x30573085},{0X80006A75,0x30583085},{0X80647975,0x30583085},	/* SHU/JU/DYU	*/
	{0X80737975,0x30573085},{0X806A7975,0x30583085},							/* SYU/JYU		*/
	{0X80006A65,0x30583047},													/* JE			*/
	{0X8073686F,0x30573087},{0X80006A6F,0x30583087},{0X8064796F,0x30583087},	/* SHO/JO/DYO	*/
    {0X0073796F,0x30573087},{0X806A796F,0x30583087},							/* SYO/JYO		*/
	{0X80636861,0x30613083},{0X80637961,0x30613083},							/* CHA/CYA		*/
	{0X80636875,0x30613085},{0X80637975,0x30613085},							/* CHU/CYU		*/
	{0X8063686F,0x30613087},{0X8063796F,0x30613087},							/* CHO/CYO		*/
	{0X806E7961,0x306B3083},													/* NYA			*/
	{0X806E7975,0x306B3085},													/* NYU			*/
	{0X806E796F,0x306B3087},													/* NYO			*/
	{0X80687961,0x30723083},{0X80627961,0x30733083},{0X80707961,0x30743083},	/* HYA/BYA/PYA	*/
	{0X80687975,0x30723085},{0X80627975,0x30733085},{0X80707975,0x30743085},	/* HYU/BYU/PYU	*/
	{0X8068796F,0x30723087},{0X8062796F,0x30733087},{0X8070796F,0x30743087},	/* HYO/BYO/PYO	*/
	{0X806D7961,0x307F3083},													/* MYA			*/
	{0X806D7975,0x307F3085},													/* MYU			*/
	{0X806D796F,0x307F3087},													/* MYO			*/
	{0X80727961,0x308A3083},													/* RYA			*/
	{0X80727975,0x308A3085},													/* RYU			*/
	{0X8072796F,0x308A3087},													/* RYO			*/
	{TAG_END,	TAG_END}
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
	ULONG rc=0L, Mode=0L, Syllable=0L, xc=0L, Kana=0L;
	struct TagItem *Vector=NULL;

	if(LanguageContext)
		Vector=(APTR)LanguageContext->IPerception->GetLanguageContextAttr((APTR)LanguageContext,(LONG)LCSTATE_VECTOR);
	if(Message)
	{
		Mode=LanguageContext->IPerception->GetLanguageContextAttr(
			(APTR)LanguageContext,
			(LONG)LCSTATE_LMODE);
		switch(Message[0])
		{
            case TRANSLATE_AMIGA: // Unmapped "Raw" Keyboard Input
				switch(Message[1]);
				{
//	Unofficial Mappings used with Developer Restricted Keymap.Library.Kmod
					case 0x00000000:	// Perception-IME//Japanese.Language::Hankaku~Zenkaku
					case 0x78000000:	// Perception-IME//Japanese.Language::Romaji~Hiragana~Katakana
					case 0x79000000:	// Perception-IME//Japanese.Language::Henkan
					case 0x7A000000:	// Perception-IME//Japanese.Language::MuHenkan
//	Official Mappings that need to be recognised
					case 0x40000000:	// Perception-IME//Japanese.Language::Space
					case 0x43000000:	// Perception-IME//Japanese.Language::Enter
					case 0x44000000:	// Perception-IME//Japanese.Language::Return
					case 0x7F000000:	// Perception-IME//Japanese.Language::Delete
//  Anything Unhandled as an AMIGA key translation					default:
						break;
				}
//				KDEBUG("LOCALE:/Japanese.Language::LanguageHook() [AMIGA:%lx:%lx]\n",Message[1],Message[2]);
				break;
            case TRANSLATE_ANSI:  // Mapped "Vanilla" Keyboard Input
				Syllable=LanguageContext->IPerception->GetLanguageContextAttr(
					(APTR)LanguageContext,
					(LONG)LCSTATE_Syllable);
//				KDEBUG("LOCALE:/Japanese.Language::LanguageHook() [ANSI/%lx/%lx]\n", (Message[1] >> 24) & 0x7F, Message[2]);
				switch((Message[1] >> 24) & 0x7F)
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
						xc=(Message[1] >> 24) & 0x7F;
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
						xc=((Message[1] >> 24) & 0x7F)+0x20;
						break;
                    case 0x08:
						Syllable = Syllable >> 8;
					default:
						xc=0L;
						break;
				}
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
						}
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
						}
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
				}
				KDEBUG("LOCALE:/Japanese.Language/:Hiragana [ASCII=%lx,CodePoints=%lx:%lx]\n",
					Syllable,(Kana >> 16),(Kana & 0xFFFF));
				if(Kana)
					Syllable=0L;
				LanguageContext->IPerception->SetLanguageContextAttr(
					(APTR)LanguageContext,
					(LONG)LCSTATE_Syllable,
					(LONG)Syllable);
				if(Kana)
					Kana=TransformSyllableCodepoint(Kana,CODEPOINT_KATAKANA_KEY);
				KDEBUG("LOCALE:/Japanese.Language/:Katakana [ASCII=%lx,CodePoints=%lx:%lx]\n",
					Syllable,(Kana >> 16),(Kana & 0xFFFF));
				switch(Mode)
				{
					case 0x0F:	// Mode 15=
					case 0x0E:	// Mode 14=
					case 0x0D:	// Mode 13=
					case 0x0C:	// Mode 12=
					case 0x0B:	// Mode 11=
					case 0x0A:	// Mode 10=
					case 0x09:	// Mode 09=
					case 0x08:	// Mode 08=
					case 0x07:	// Mode 07=
					case 0x06:	// Mode 06=
					case 0x05:	// Mode 05=
					case 0x04:	// Mode 04=
					case 0x03:	// Mode 03=
					case 0x02:	// Mode 02=
					case 0x01:	// Mode 01=
					default:	// Mode 00= Pass-Through
						break;
				}
				break;
			default:
				break;
		}
		LanguageContext->IPerception->SetLanguageContextAttr(
			(APTR)LanguageContext,
			(LONG)LCSTATE_LMODE,
			(LONG)Mode);
	};


	return(rc);
}

/**/
ULONG FindSyllableCandidate(ULONG Key, struct UtilityIFace *IUtility)
{
	ULONG rc=0L;
	struct TagItem *item=NULL;

	if(IUtility)
		item=IUtility->FindTagItem(TAG_USER|Key,SyllableCandidates);
	if(item)
		rc=item->ti_Data;

	return(rc);
}

/**/
ULONG TransformSyllableCodepoint(ULONG Kana, ULONG Base)
{
	ULONG rc=0L, h=0L, l=0L;

	if(Kana & 0xFFFF0000)
		h=(Kana & 0xFFFF0000) >> 16;
	if(Kana & 0xFFFF0000)
		l=(Kana & 0xFFFF0000);
	if(h)
		h = (h & CODEPOINT_KANAMASK_KEY)|Base;
	if(l)
		l = (l & CODEPOINT_KANAMASK_KEY)|Base;
	rc=(h << 16)|l;

	return(rc);
};

/**/
ULONG QueueSyllableCandidate(ULONG codepoint,struct LanguageContext *LanguageContext)
{
	ULONG rc=0L;
	return(rc);
};

/**/
ULONG UpdateKanjiCandidates(ULONG codepoint,struct LanguageContext *LanguageContext)
{
	ULONG rc=0L;
	return(rc);
};

/**/
ULONG UpdateVocabCandidates(ULONG codepoint,struct LanguageContext *LanguageContext)
{
	ULONG rc=0L;
	return(rc);
};

/**/
