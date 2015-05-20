/*
\\		"Perception"
//
\\		Language Driver Functionality
*/
#include "language.h"

CONST struct TagItem LibInterfaceTags[] =
{
	    { MIT_Name,        (ULONG)"main"			},
	    { MIT_VectorTable, (ULONG)LibInterfaceTable	},
	    { MIT_Version,     1                    	},
	    { TAG_END,         0                    	}
};

CONST APTR LibInterfaces[] =
{
		(APTR)&LibManagerTags,
		(APTR)&LibInterfaceTags,
		NULL
};

CONST APTR LibInterfaceTable[] =
{
  (APTR)&MCALL_Obtain,
  (APTR)&MCALL_Release,
  NULL,
  NULL,
  (APTR)&LCALL_GetDriverInfo,
  (APTR)&LCALL_ConvToLower,
  (APTR)&LCALL_ConvToUpper,
  (APTR)&LCALL_GetCodeSet,
  (APTR)&LCALL_GetLocaleStr,
  (APTR)&LCALL_IsAlNum,
  (APTR)&LCALL_IsAlpha,
  (APTR)&LCALL_IsCntrl,
  (APTR)&LCALL_IsDigit,
  (APTR)&LCALL_IsLower,
  (APTR)&LCALL_IsPrint,
  (APTR)&LCALL_IsPunct,
  (APTR)&LCALL_IsSpace,
  (APTR)&LCALL_IsUpper,
  (APTR)&LCALL_IsXDigit,
  (APTR)&LCALL_StrConvert,
  (APTR)&LCALL_StrnCmp,
  (APTR)&LCALL_IsBlank,
  (APTR)-1
};

#define LOCALECODESET		106	/* 39 = ISO-2022-JP, 106 = UTF-8 */
#define MAXLOCALESTRINGS	72

/*	The Following Native Language Strings are UTF-8 Encoded Sequences,

	The Internal Catalog Strings have no differentiation for case.
	 (all Japanese Glyphs are singular with a limited subset exception for a doubled-sound marker)

	Comments are Glyph counts, Glyphs are NOT one octet per glyph idiocy
*/
CONST UBYTE LanguageNativeName[]	=	{0xE6,0x97,0xA5,0xE6,0x9C,0xAC,0xE8,0xAA,0x9E,0,0,0};	/*3*/

CONST UBYTE Monday[]				=	{0xE6,0x9C,0x88,0xE6,0x9B,0x9C,0xE6,0x97,0xA5,0,0,0};	/*3*/
CONST UBYTE Tuesday[]				=	{0xE7,0x81,0xAB,0xE6,0x9B,0x9C,0xE6,0x97,0xA5,0,0,0};	/*3*/
CONST UBYTE Wednesday[]				=	{0xE6,0xB0,0xB4,0xE6,0x9B,0x9C,0xE6,0x97,0xA5,0,0,0};	/*3*/
CONST UBYTE Thursday[]				=	{0xE6,0x9C,0xA8,0xE6,0x9B,0x9C,0xE6,0x97,0xA5,0,0,0};	/*3*/
CONST UBYTE Friday[]				=	{0xE9,0x87,0x91,0xE6,0x9B,0x9C,0xE6,0x97,0xA5,0,0,0};	/*3*/
CONST UBYTE Saturday[]				=	{0xE5,0x9C,0x9F,0xE6,0x9B,0x9C,0xE6,0x97,0xA5,0,0,0};	/*3*/
CONST UBYTE Sunday[]				=	{0xE6,0x97,0xA5,0xE6,0x9B,0x9C,0xE6,0x97,0xA5,0,0,0};	/*3*/

CONST UBYTE January[]				=	{0xE4,0xB8,0x80,0xE6,0x9C,0x88,0,0};					/*2*/
CONST UBYTE February[]				=	{0xE4,0xBA,0x8C,0xE6,0x9C,0x88,0,0};					/*2*/
CONST UBYTE March[]					=	{0xE4,0xB8,0x89,0xE6,0x9C,0x88,0,0};					/*2*/
CONST UBYTE April[]					=	{0xE5,0x9B,0x9B,0xE6,0x9C,0x88,0,0};					/*2*/
CONST UBYTE May[]					=	{0xE4,0xBA,0x94,0xE6,0x9C,0x88,0,0};					/*2*/
CONST UBYTE June[]					=	{0xE5,0x85,0xAD,0xE6,0x9C,0x88,0,0};					/*2*/
CONST UBYTE July[]					=	{0xE4,0xB8,0x83,0xE6,0x9C,0x88,0,0};					/*2*/
CONST UBYTE August[]				=	{0xE5,0x85,0xAB,0xE6,0x9C,0x88,0,0};					/*2*/
CONST UBYTE September[]				=	{0xE4,0xB9,0x9D,0xE6,0x9C,0x88,0,0};					/*2*/
CONST UBYTE October[]				=	{0xE5,0x8D,0x81,0xE6,0x9C,0x88,0,0};					/*2*/
CONST UBYTE November[]				=	{0xE5,0x8D,0x81,0xE4,0xB8,0x80,0xE6,0x9C,0x88,0,0,0};	/*3*/
CONST UBYTE December[]				=	{0xE5,0x8D,0x81,0xE4,0xBA,0x8C,0xE6,0x9C,0x88,0,0,0};	/*3*/

CONST UBYTE Yes[]					=	{0xE3,0x81,0xAF,0xE3,0x81,0x84,0,0};					/*2*/
CONST UBYTE No[]					=	{0xE3,0x81,0x84,0xE3,0x81,0x84,0xE3,0x81,0x88,0};		/*3*/

CONST UBYTE Morning[]				=	{0xE5,0x8D,0x88,0xE5,0x89,0x8D,0,0};					/*2*/
CONST UBYTE Evening[]				=	{0xE5,0xA4,0x9C,0};										/*1*/

CONST UBYTE SoftHyphen[]			=	{0xE3,0x83,0xBC,0};										/*1*/
CONST UBYTE HardHyphen[]			=	{0xE3,0x83,0xBC,0};										/*1*/
CONST UBYTE OpenQuote[]				=	{0xE3,0x80,0x8C,0};										/*1*/
CONST UBYTE CloseQuote[]			=	{0xE3,0x80,0x8D,0};										/*1*/

CONST UBYTE Yesterday[]				=	{0xE6,0x98,0xA8,0xE6,0x97,0xA5,0,0,0};					/*2*/
CONST UBYTE Today[]					=	{0xE4,0xBB,0x8A,0xE6,0x97,0xA5,0,0,0};					/*2*/
CONST UBYTE Tomorrow[]				=	{0xE6,0x98,0x8E,0xE6,0x97,0xA5,0,0,0};					/*2*/
CONST UBYTE Future[]				=	{0xE6,0x9C,0xAA,0xE6,0x9D,0xA5,0,0,0};					/*2*/

CONST UBYTE *InternalCatalogString[] =
{
	/*
	** The Following String Constants need to be updated for actual usage
	** 72 Strings are provided
	*/
	(APTR)"jpn",											/* ISO-639-2 ID */
	(APTR)&Monday,											/* Day1 */
	(APTR)&Tuesday,											/* Day2 */
	(APTR)&Wednesday,										/* Day3 */
	(APTR)&Thursday,										/* Day4 */
	(APTR)&Friday,											/* Day5 */
	(APTR)&Saturday,										/* Day6 */
	(APTR)&Sunday,											/* Day7 */
	(APTR)&Monday,											/* Day1 */
	(APTR)&Tuesday,											/* Day2 */
	(APTR)&Wednesday,										/* Day3 */
	(APTR)&Thursday,										/* Day4 */
	(APTR)&Friday,											/* Day5 */
	(APTR)&Saturday,										/* Day6 */
	(APTR)&Sunday,											/* Day7 */
	(APTR)&January,											/* Month1 */
	(APTR)&February,										/* Month2 */
	(APTR)&March,											/* Month3 */
	(APTR)&April,											/* Month4 */
	(APTR)&May,												/* Month5 */
	(APTR)&June,											/* Month6 */
	(APTR)&July,											/* Month7 */
	(APTR)&August,											/* Month8 */
	(APTR)&September,										/* Month9 */
	(APTR)&October,											/* MonthA */
	(APTR)&November,										/* MonthB */
	(APTR)&December,										/* MonthC */
	(APTR)&January,											/* Month1 */
	(APTR)&February,										/* Month2 */
	(APTR)&March,											/* Month3 */
	(APTR)&April,											/* Month4 */
	(APTR)&May,												/* Month5 */
	(APTR)&June,											/* Month6 */
	(APTR)&July,											/* Month7 */
	(APTR)&August,											/* Month8 */
	(APTR)&September,										/* Month9 */
	(APTR)&October,											/* MonthA */
	(APTR)&November,										/* MonthB */
	(APTR)&December,										/* MonthC */
	(APTR)&Yes,												/* Yes */
	(APTR)&No,												/* No */
	(APTR)&Morning,											/* AM */
	(APTR)&Evening,											/* PM */
	(APTR)&SoftHyphen,										/* SoftHyphen */
	(APTR)&HardHyphen,										/* HardHyphen */
	(APTR)&OpenQuote,										/* OpenQuote */
	(APTR)&CloseQuote,										/* CloseQuote */
	(APTR)&Yesterday,										/* Yesterday */
	(APTR)&Today,											/* Today */
	(APTR)&Tomorrow,										/* Tomorrow */
	(APTR)&Future,											/* Future */
	(APTR)&LanguageNativeName,								/* LanguageName */
	(APTR)&Monday,											/* Day1 */
	(APTR)&Tuesday,											/* Day2 */
	(APTR)&Wednesday,										/* Day3 */
	(APTR)&Thursday,										/* Day4 */
	(APTR)&Friday,											/* Day5 */
	(APTR)&Saturday,										/* Day6 */
	(APTR)&Sunday,											/* Day7 */
	(APTR)&January,											/* Month1 */
	(APTR)&February,										/* Month2 */
	(APTR)&March,											/* Month3 */
	(APTR)&April,											/* Month4 */
	(APTR)&May,												/* Month5 */
	(APTR)&June,											/* Month6 */
	(APTR)&July,											/* Month7 */
	(APTR)&August,											/* Month8 */
	(APTR)&September,										/* Month9 */
	(APTR)&October,											/* MonthA */
	(APTR)&November,										/* MonthB */
	(APTR)&December,										/* MonthC */
};


/*  32bit Mask Value for Routines Actually implimented
*/
ULONG LCALL_GetDriverInfo(struct LIBIFACE_CLASS *Iface)
{
	return(ALL_LANGFUNCS);
};

ULONG LCALL_ConvToLower(struct LIBIFACE_CLASS *Iface,ULONG c)
{
	ULONG rc=0L;
	return(rc);
};

ULONG LCALL_ConvToUpper(struct LIBIFACE_CLASS *Iface,ULONG c)
{
	ULONG rc=0L;
	return(rc);
};

ULONG LCALL_GetCodeSet(struct LIBIFACE_CLASS *Iface)
{
	ULONG rc=LOCALECODESET;
	return(rc);
};

/*	Two Boundary conditions were deliberately enforced using Input Typing
 *   and the value comparison,  this allows for a shorter routine to be written.
 *   No point in wasting space without need is there?
*/
APTR  LCALL_GetLocaleStr(struct LIBIFACE_CLASS *Iface,ULONG cStringID)
{
	APTR rc=NULL;
	if(cStringID < MAXLOCALESTRINGS)
		rc=(APTR)InternalCatalogString[cStringID];
	return(rc);
};

LONG  LCALL_IsAlNum(struct LIBIFACE_CLASS *Iface, ULONG c)
{
	LONG rc=0L;
	return(rc);
};

LONG  LCALL_IsAlpha(struct LIBIFACE_CLASS *Iface, ULONG c)
{
	LONG rc=0L;
	return(rc);
};

LONG  LCALL_IsCntrl(struct LIBIFACE_CLASS *Iface, ULONG c)
{
	LONG rc=0L;
	return(rc);
};

LONG  LCALL_IsDigit(struct LIBIFACE_CLASS *Iface, ULONG c)
{
	LONG rc=0L;
	return(rc);
};

LONG  LCALL_IsLower(struct LIBIFACE_CLASS *Iface, ULONG c)
{
	LONG rc=0L;
	return(rc);
};

LONG  LCALL_IsPrint(struct LIBIFACE_CLASS *Iface, ULONG c)
{
	LONG rc=0L;
	return(rc);
};

LONG  LCALL_IsPunct(struct LIBIFACE_CLASS *Iface, ULONG c)
{
	LONG rc=0L;
	return(rc);
};

LONG  LCALL_IsSpace(struct LIBIFACE_CLASS *Iface, ULONG c)
{
	LONG rc=0L;
	return(rc);
};

LONG  LCALL_IsUpper(struct LIBIFACE_CLASS *Iface, ULONG c)
{
	LONG rc=0L;
	return(rc);
};

LONG  LCALL_IsXDigit(struct LIBIFACE_CLASS *Iface, ULONG c)
{
	LONG rc=0L;
	return(rc);
};

APTR  LCALL_StrConvert(	struct LIBIFACE_CLASS *Iface,
						ULONG srcLength,	ULONG dstLength,
						APTR srcStringPtr,	APTR dstStringPtr)
{
	APTR rc=NULL;
	return(rc);
};

APTR  LCALL_StrnCmp(	struct LIBIFACE_CLASS *Iface,
					ULONG srcLength,	ULONG dstLength,
					APTR srcStringPtr,	APTR dstStringPtr)
{
	APTR rc=NULL;
	return(rc);
};

LONG  LCALL_IsBlank(	struct LIBIFACE_CLASS *Iface, ULONG c)
{
	LONG rc=0L;
	return(rc);
};

/**/
