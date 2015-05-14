#ifndef LIBRARIES_PERCEPTION_H
#define LIBRARIES_PERCEPTION_H

#ifdef __cplusplus
#ifdef __USE_AMIGAOS_NAMESPACE__
namespace AmigaOS {
#endif
extern "C" {
#endif

#define PERCEPTION_NAME				"perception.library"
#define	PERCEPTION_API_VERSION		1
#define	PERCEPTION_API_REVISION		0

struct InputTagItem
{
	UWORD	type;
	UWORD	qual;
	ULONG	glyph;
};
/*
			Name                    16 Bits formatted in 1:3:4:4:3:1 notation
*/
#define		TRANSLATE_NONE			0x0000	/* Initial State								*/
#define		TRANSLATE_AMIGA			0x0001	/* Default Keymap Native without Translation	*/
#define		TRANSLATE_ANSI			0x0011	/* Default Keymap Native with ANSI Translation	*/

/*	LanguageContext Definition

	This structure is for record-keeping within each Language plugin.
*/
#define     IME_STATE_SIZE		32L		/* Size is Arbitrary and subject to change				*/
#define		IME_MESSAGE_SIZE	8L		/* Size is Arbitrary and subject to change				*/
#define     IME_VECTOR_SIZE     32L		/* Size is NOT-Arbitrary and still subject to change!	*/

struct LanguageContext
{
	struct Hook				Hook;
	struct PerceptionIFace	*IPerception;
	struct UtilityIFace		*IUtility;
	ULONG					State[IME_STATE_SIZE];
	struct InputTagItem		Vector[IME_VECTOR_SIZE];
};
#define	LHCONTEXTSIZE    sizeof(struct LanguageContext)
/*
	System-Wide Attributes (LanguageContext *Expanded* settings)
*/
#define DEFAULT_SYSTEM_LANGUAGE		(TAG_USER+0x0001)
#define DEFAULT_ENABLE_LANGUAGE		(TAG_USER+0x0002)
/*
	LanguageContext Protocol Attributes
*/
#define	LCSTATE_VECTOR				(TAG_USER+0)
#define	LCSTATE_LNAME				(TAG_USER+1)
#define	LCSTATE_LMODE				(TAG_USER+2)
#define	LCSTATE_LMODENAME			(TAG_USER+3)
#define	LCSTATE_LTEMP				(TAG_USER+4)
#define	LCSTATE_CATALOG1			(TAG_USER+5)
#define	LCSTATE_CATALOG2			(TAG_USER+6)
#define	LCSTATE_CATALOG3			(TAG_USER+7)
#define	LCSTATE_EMIT				(TAG_USER+8)
/*
	I have deliberately been using TAG_USER+9 through TAG_USER+33 as
		an Embedded Data Array within the TagItem Array...
    so...

	for ( index=LCSTATE_EMITBUFF; index<LCSTATE_EMITBMAX; index++ )
	{
		bTagItem = IUtility->FindTagItem(index, lcsTagItemChain);
		... bTagItem->ti_Data ...
	}

	Push CodePoint values into this Emitting Buffer,  they will be automagically converted to UTF8
		for Internal Forwarding back into the OS
*/
#define	LCSTATE_EMITBUFF			(TAG_USER+9)
#define	LCSTATE_EMITBMAX			(TAG_USER+34)
/**/
/* Language Defined Tags start from here */
#define	LCSTATE_EXPANDED			(TAG_USER+64)
#define	LCSTATE_MAX					(TAG_USER+128)


/*
**	LANGUAGE_MODE is an Array with 16 entries

    These are primarily the Boundaries for multi-mode Language support,
		labels for IDs within the range are subject to the Language concerned.
*/
#define LANGUAGE_MODE				(TAG_USER+0x0010)
#define LANGUAGE_MODE_MIN			LANGUAGE_MODE
#define LANGUAGE_MODE_MAX			(TAG_USER+0x001F)
/*
**	CONVERSION_MODE is an Array with 16 entries
*/
#define CONVERSION_MODE				(TAG_USER+0x0020)
#define CONVERSION_MODE_MIN			CONVERSION_MODE
#define CONVERSION_MODE_MAX			(TAG_USER+0x002F)
/*
*/

/*
**	LANGUAGE_METHOD_***() functional tag sets

	Each following SET of tags below are combined as a single "message" for updating the
		"LanguageContext" TagList in retaining any form of "state" information between calls
		to the Language Library specific context registered with the IME subsystem.

	All TagItems in a set use ... markers in Comments for common naming
*/
/*
	No-Operation and Null Attribute,  Place Marker for "Message" TagList Initialization
*/
#define	LANGUAGE_IME_NOP				(TAG_USER+0x00010000)
#define	LANGUAGE_IME_NOP_NULL			(TAG_USER+0x00010001)

/*
	ti_Tag:ti_Data
		...ANSI:Glyph
		...ANSI_QUAL:Qualifier
*/
#define	LANGUAGE_TRANSLATE_ANSI			(TAG_USER+0x00020000)
#define	LANGUAGE_TRANSLATE_ANSI_QUAL	(TAG_USER+0x00020001)

/*
	ti_Tag:ti_Data
		...ANSI:Glyph
		...ANSI_QUAL:Qualifier
*/
#define	LANGUAGE_TRANSLATE_AMIGA		(TAG_USER+0x00030000)
#define	LANGUAGE_TRANSLATE_AMIGA_QUAL	(TAG_USER+0x00030001)

/*
*/

#ifdef __cplusplus
}
#ifdef __USE_AMIGAOS_NAMESPACE__
}
#endif
#endif

#endif /* LIBRARIES_PERCEPTION_H */
