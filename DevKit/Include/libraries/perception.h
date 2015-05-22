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
#define		TRANSLATE_IGNORE		0x0001	/* These are for re-fed marked Input Events		*/
#define		TRANSLATE_AMIGA			0x0010	/* Default Keymap Native without Translation	*/
#define		TRANSLATE_ANSI			0x0011	/* Default Keymap Native with ANSI Translation	*/

/*	LanguageContext Definition

	This structure is for record-keeping within each Language plugin.
*/
#define     IME_STATE_SIZE		96L		/* Size is Arbitrary and subject to change				*/
#define		IME_MESSAGE_SIZE	8L		/* Size is Arbitrary and subject to change				*/
#define     IME_VECTOR_SIZE     96L		/* Size is NOT-Arbitrary and still subject to change!	*/

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
	LanguageContext Protocol Attributes
*/
#define	LCSTATE_VECTOR				(TAG_USER+0)
#define	LCSTATE_LNAME				(TAG_USER+1)
#define	LCSTATE_LMODE				(TAG_USER+2)
#define	LCSTATE_LMODENAME			(TAG_USER+3)
#define	LCSTATE_LTEMP				(TAG_USER+4)
#define	LCSTATE_EMIT				(TAG_USER+5)
//
#define	LCSTATE_EXPANDED			(TAG_USER+64)
//
#define	LCSTATE_MAX					(TAG_USER+128)

/*
**	EMIT_BLOCK is an Array with 16 Entries

	These are where the actual "output" glyph bytes to be re-fed through the input.device
		are presented back to the Perception-IME core from within a plugin.
*/
#define	LANGUAGE_EMIT				(TAG_USER+0x0010)
#define LANGUAGE_EMIT_MIN			LANGUAGE_EMIT
#define	LANGUAGE_EMIT_MAX			(TAG_USER+0x001F)
/*
**	LANGUAGE_MODE is an Array with 16 entries

    These are primarily the Boundaries for multi-mode Language support,
		labels for IDs within the range are subject to the Language concerned.
*/
#define LANGUAGE_MODE				(TAG_USER+0x0020)
#define LANGUAGE_MODE_MIN			LANGUAGE_MODE
#define LANGUAGE_MODE_MAX			(TAG_USER+0x002F)

/*
**	CONVERSION_MODE is an Array with 16 entries
*/
#define CONVERSION_MODE				(TAG_USER+0x0030)
#define CONVERSION_MODE_MIN			CONVERSION_MODE
#define CONVERSION_MODE_MAX			(TAG_USER+0x003F)

#ifdef __cplusplus
}
#ifdef __USE_AMIGAOS_NAMESPACE__
}
#endif
#endif

#endif /* LIBRARIES_PERCEPTION_H */
