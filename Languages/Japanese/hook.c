/*
\\		"Perception"
//
\\		Language Driver Functionality
*/
#include "language.h"

STATIC CONST BYTE LanguageName[] = LIBRARY_NAME;

/* This is where the incoming ANSI characters get stored */
#define	LCSTATE_CHORDBUFF		(LCSTATE_EXPANDED)
#define	LCSTATE_IDEOGRAPH_IDX	(LCSTATE_EXPANDED+1)
#define	LCSTATE_IDEOGRAPH_BUFF	(LCSTATE_EXPANDED+2)
#define	IDEOGRAPH_CNT			(32)
#define	LCSTATE_IDEOGRAPH_MAX	(LCSTATE_IDEOGRAPH_BUFF+IDEOGRAPH_CNT)

#define LC_MODE_ROMAJI_DIRECT	(2)
#define LC_MODE_KANA_KANJI		(0)
#define LC_MODE_KATAKANA		(1)

#define CODEPOINT_HIRAGANA_KEY		0x3040
#define CODEPOINT_KATAKANA_KEY		0x30A0
#define	CODEPOINT_KANATOGGLE_MASK	0x00E0	/* Reversible Hiragana<->Katakana Transform Key */

ULONG FindKanaCandidate(ULONG key,struct TagItem *ChordBuffer,struct UtilityIFace *IUtility);
ULONG FindKanjiCandidate(struct TagItem *ChordBuffer,struct UtilityIFace *IUtility);
ULONG QueueCodePoint(ULONG key,struct TagItem *qVector,ULONG idx,struct UtilityIFace *IUtility);

/*
**	The ChordCandidates Table has TagItems in the same order as the following Kana Table
**
**
	NULL Candidate / EQUALS_SIGN_KATAKANA
	a i u e o
	ka ki ku ke ko
	kya kyu kyo
	ga ga gu ge go
	gya gyu gyo
	sa si shi su se so
	sha/sya shu/syu sho/syo
	za ji zi  zu ze zo
	ja/jya ju/jyu je jo/jyo
	dya dyu dyo
	ta chi/ci/ti tsu/tu te to
	cha/cya chu/cyu cho/cyo
	da di dzu/du de do
	na ni nu ne no
	nya nyu nyo
	ha hi fu/hu he ho
	hya hyu hyo
	ba bi bu be bo
	bya byu byo
	pa pi pu pe po
	pya pyu pyo
	ma mi mu me mo
	mya myu myo
	ya yu yo
	ra ri ru re ro
	rya ryu ryo
	wa wi we wo
	nn
	vu va vi ve vo
*/
STATIC CONST struct TagItem ChordCandidates[] =		/* From Romaji To Hiragana,
	 Katakana=TransformHiraganaKatakana(Hiragana);
	 Hiragana=TransformHiraganaKatakana(Katakana);
 */
{
	{0X80000000,0x00000000},{0X8000003D,0x000030A0},
	{0x80000061,0x00003042},{0x80000069,0x00003044},{0x80000075,0x00003046},{0x80000065,0x00003048},{0x8000006F,0x0000304A},
	{0x80006B61,0x0000304B},{0x80006B69,0x0000304D},{0x80006B75,0x0000304F},{0x80006B65,0x00003051},{0x80006B6F,0x00003053},
	{0x806B7961,0x304D3083},{0x806B7975,0x304D3085},{0x806B796F,0x304D3087},
	{0x80006761,0x0000304C},{0x80006769,0x0000304E},{0x80006775,0x00003050},{0x80006765,0x00003052},{0x8000676F,0x00003054},
	{0x80677961,0x304E3083},{0x80677975,0x304E3085},{0x8067796F,0x304E3087},
	{0x80007361,0x00003055},{0x80736869,0x00003057},{0x80007369,0x00003057},{0x80007375,0x00003059},{0x80007365,0x0000305B},{0x8000736F,0x0000305D},
	{0x80736861,0x30573083},{0x80737961,0x30573083},{0x80736875,0x30573085},{0x80737975,0x30573085},{0x8073686F,0x30573087},{0x8073796F,0x30573087},
	{0x80007A61,0x00003056},{0x80006A69,0x00003058},{0x80007A69,0x00003058},{0x80007A75,0x0000305A},{0x80007A65,0x0000305C},{0x80007A6F,0x0000305E},
	{0x80006A61,0x30583083},{0x806A7961,0x30583083},{0x80006A75,0x30583085},{0x806A7975,0x30583085},{0x80006A65,0x30583047},{0x80006A6F,0x30583087},{0x806A796F,0x30583087},
	{0x80647961,0x30583083},{0x80647975,0x30583085},{0x8064796F,0x30583087},
	{0x80007461,0x0000305F},{0x80636869,0x00003061},{0x80006369,0x00003061},{0x80007469,0x00003061},{0x80747375,0x00003064},{0x80007475,0x00003064},{0x80007465,0x00003066},{0x8000746F,0x00003068},
	{0x80636861,0x30613083},{0x80637961,0x30613083},{0x80636875,0x30613085},{0x80637975,0x30613085},{0x8063686F,0x30613087},{0x8063796F,0x30613087},
	{0x80006461,0x00003060},{0x80006469,0x00003062},{0x80647A75,0x00003065},{0x80006475,0x00003065},{0x80006465,0x00003067},{0x8000646F,0x00003069},
	{0x80006E61,0x0000306A},{0x80006E69,0x0000306B},{0x80006E75,0x0000306C},{0x80006E65,0x0000306D},{0x80006E6F,0x0000306E},
	{0x806E7961,0x306B3083},{0x806E7975,0x306B3085},{0x806E796F,0x306B3087},
	{0x80006861,0x0000306F},{0x80006869,0x00003072},{0x80006675,0x00003075},{0x80006875,0x00003075},{0x80006865,0x00003078},{0x8000686F,0x0000307B},
	{0x80687961,0x30723083},{0x80687975,0x30723085},{0x8068796F,0x30723087},
	{0x80006261,0x00003070},{0x80006269,0x00003073},{0x80006275,0x00003076},{0x80006265,0x00003079},{0x8000626F,0x0000307C},
	{0x80627961,0x30733083},{0x80627975,0x30733085},{0x8062796F,0x30733087},
	{0x80007061,0x00003071},{0x80007069,0x00003074},{0x80007075,0x00003077},{0x80007065,0x0000307A},{0x8000706F,0x0000307D},
	{0x80707961,0x30743083},{0x80707975,0x30743085},{0x8070796F,0x30743087},
	{0x80006D61,0x0000307E},{0x80006D69,0x0000307F},{0x80006D75,0x00003080},{0x80006D65,0x00003081},{0x80006D6F,0x00003082},
	{0x806D7961,0x307F3083},{0x806D7975,0x307F3085},{0x806D796F,0x307F3087},
	{0x80007961,0x00003084},{0x80007975,0x00003086},{0x8000796F,0x00003088},
	{0x80007261,0x00003089},{0x80007269,0x0000308A},{0x80007275,0x0000308B},{0x80007265,0x0000308C},{0x8000726F,0x0000308D},
	{0x80727961,0x308A3083},{0x80727975,0x308A3085},{0x8072796F,0x308A3087},
	{0x80007761,0x0000308F},{0x80007769,0x00003090},{0x80007765,0x00003091},{0x8000776F,0x00003092},
	{0x80006E6E,0x00003093},
	{0x80007675,0x00003094},{0x80007661,0x000030F7},{0x80007669,0x000030F8},{0x80007665,0x000030F9},{0x8000766F,0x000030FA},
	{TAG_END,	TAG_END}
};

/* These are the "double-mora" timing markers for various sound combinations *
STATIC CONST struct TagItem ChordExtensions[] =
{
	* mini a i u e o *
	{0x80000041,0x00003041},
	{0x80000049,0x00003043},
	{0x80000055,0x00003045},
	{0x80000045,0x00003047},
	{0x8000004F,0x00003049},
	* mini tsu *
	{0x80545355,0x00003063},
	* mini ya yu yo *
	{0x80005941,0x00003083},
	{0x80007955,0x00003085},
	{0x8000594F,0x00003087},
	* mini wa *
	{0x80005741,0x0000308E},
	* mini ka ke *
	{0x8000B441,0x00003095},
	{0x8000B445,0x00003096},
	{TAG_END,	TAG_END}
};
*/

/*	Set the Primary System Input Language	*/
STATIC CONST struct TagItem DefaultSystemLanguage = { DEFAULT_SYSTEM_LANGUAGE, TRUE };
STATIC CONST unsigned char LanguageKanjiCatalog[] = "Ideographs";
STATIC CONST unsigned char LanguageVocabCatalog[] = "Vocabulary";

/* Once-or-more Init & Exit calls will occur...
*/
void InitPerceptionHook(struct LIBRARY_CLASS *Self)
{
    APTR Language=NULL;

	if(Self->IPerception)
		Language=Self->IPerception->ObtainLanguageContext((APTR)LanguageName,(APTR)&ExecLanguageContextHook);
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

ULONG FindKanaCandidate(ULONG key, struct TagItem *ChordBuffer,struct UtilityIFace *IUtility)
{
	ULONG rc=0L, chord=0L, prefix=0L;
	struct TagItem *Buffer=NULL;

	if((key & 0x000000FF)==(key & 0x0000007F))
	{
		if((key-0x00000061)<0x0000001B)
			chord=TAG_USER|(ChordBuffer->ti_Data << 8)|key;
		if((key-0x00000041)<0x0000001B)
			chord=(TAG_USER|(ChordBuffer->ti_Data << 8)|key)+0x20;
	};
	if(chord & TAG_USER)
	{
		if((chord & 0x8000FFFF)==chord)
		{
			if((chord & 0x00007F00)==0x00006E00)
				prefix=prefix|0x00003093;
			if(((chord & 0x00007F00) >> 8)==(chord & 0x00007F))
				prefix=prefix|0x30630000;
			if(prefix)
				chord=(chord & 0x800000FF);
		};
		Buffer=IUtility->FindTagItem(chord,ChordCandidates);
		if(Buffer)
		{
			chord=0L;
			rc=Buffer->ti_Data;
		}else{
			ChordBuffer->ti_Data=chord;
		};
        if(rc)
		{
			if(prefix & 0x7FFF0000)
				rc=0x80000000|rc;
			if(prefix & 0x00007FFF)
				rc=0x00008000|rc;
		};
    };
	return(rc);
}

ULONG FindKanjiCandidate(struct TagItem *ChordBuffer,struct UtilityIFace *IUtility)
{
	ULONG rc=0L;
	return(rc);
}

ULONG QueueCodePoint(ULONG key,struct TagItem *qVector,ULONG idx,struct UtilityIFace *IUtility)
{
	ULONG rc=idx;
	struct TagItem *Buffer=IUtility->FindTagItem(idx,qVector);
	if(Buffer)
	{
		Buffer->ti_Data=key;
		rc++;
	};
	return(rc);
}

/*
	This is where ALL of the Language Specific "Magic" happens...

	Please note...Characters are given in the HIGHEST octet of a 32bit value.

	CodePoints are given using a full 32bit value as EmitBuffer ti_Data points for UTF8 conversion.

	the return value is the number of Emitted CodePoints
*/
ULONG ExecLanguageContextHook(struct LanguageContextHook *lch,APTR LanguageContext,APTR m)
{
	ULONG rc=0L, *Message=m, glyph=0L, emIndex=LCSTATE_EMITBUFF;
	struct PerceptionIFace	*IPerception= lch->PerceptionLib;
	struct UtilityIFace 	*IUtility	= lch->UtilityLib;
	struct TagItem *Vector=NULL, *chBuffer=NULL, *icBuffer=NULL;
	struct TagItem *LMode=NULL,  *emit=NULL,     *ideograph=NULL;
	struct TagItem VCommand;

	if(LanguageContext)
	{
		VCommand.ti_Tag  = LCSTATE_CATALOG1;
		VCommand.ti_Data = (ULONG)&LanguageKanjiCatalog;
		Vector=(APTR)IPerception->GetLanguageContextAttr(LanguageContext,(APTR)&VCommand);
		VCommand.ti_Tag  = LCSTATE_CATALOG2;
		VCommand.ti_Data = (ULONG)&LanguageVocabCatalog;
		Vector=(APTR)IPerception->GetLanguageContextAttr(LanguageContext,(APTR)&VCommand);
		VCommand.ti_Tag  = LCSTATE_VECTOR;
		VCommand.ti_Data = 0L;
		Vector=(APTR)IPerception->GetLanguageContextAttr(LanguageContext,(APTR)&VCommand);
		if(Vector)
		{
			LMode=IUtility->FindTagItem(LCSTATE_LMODE,Vector);
			emit=IUtility->FindTagItem(LCSTATE_EMIT,Vector);
			ideograph=IUtility->FindTagItem(LCSTATE_IDEOGRAPH_IDX,Vector);
			chBuffer=IUtility->FindTagItem(LCSTATE_CHORDBUFF,Vector);
			icBuffer=IUtility->FindTagItem(LCSTATE_IDEOGRAPH_BUFF,Vector);
		};
	};

	switch(Message[0])
	{
		case LANGUAGE_TRANSLATE_AMIGA:
            if((Message[1] & 0xFF000000) == Message[1])
				switch(Message[1] >> 24)
				{
//	Unofficial Mappings used with a Developer only Keymap.Library.Kmod (Limited to P-IME dev usage)
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
//  Anything Unhandled as an AMIGA key translation					default:
						KDEBUG("Perception-IME//Japanese.Language::[%lx] is Unknown!\n", Message[1]);
						break;
				}
			break;
		case LANGUAGE_TRANSLATE_ANSI:
			if((Message[1] & 0xFF000000) == Message[1])
				glyph=FindKanaCandidate(Message[1] >> 24,chBuffer,IUtility);
			if(glyph)
			{
				if(glyph & 0x80000000)
					emIndex=QueueCodePoint(0x00003063,Vector,emIndex,IUtility);
				if(glyph & 0x7FFF0000)
					emIndex=QueueCodePoint(((glyph & 0xFFFF0000) >> 16),Vector,emIndex,IUtility);
				if(glyph & 0x0000FFFF)
					emIndex=QueueCodePoint((glyph & 0x0000FFFF),Vector,emIndex,IUtility);
				glyph=FindKanjiCandidate(chBuffer,IUtility);
			};
			rc=emIndex-LCSTATE_EMITBUFF;
			break;
		default:
			KDEBUG("Perception-IME//Japanese.Language::[%lx] is Unknown!\n", Message[0]);
			break;
	}

	return(rc);
}

/**/