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

#define LOCALECODESET		106		/* UTF-8,  see ISO-2022-KR for encoding 37 */
#define MAXLOCALESTRINGS	72

CONST UBYTE LanguageNativeName[]	=	{0xED,0x95,0x9C,0xEA,0xB5,0xAD,0xEC,0x94,0xB4,0};	/*3*/

CONST UBYTE Monday[]				=	{0};
CONST UBYTE Tuesday[]				=	{0};
CONST UBYTE Wednesday[]				=	{0};
CONST UBYTE Thursday[]				=	{0};
CONST UBYTE Friday[]				=	{0};
CONST UBYTE Saturday[]				=	{0};
CONST UBYTE Sunday[]				=	{0};

CONST UBYTE January[]				=	{0};
CONST UBYTE February[]				=	{0};
CONST UBYTE March[]					=	{0};
CONST UBYTE April[]					=	{0};
CONST UBYTE May[]					=	{0};
CONST UBYTE June[]					=	{0};
CONST UBYTE July[]					=	{0};
CONST UBYTE August[]				=	{0};
CONST UBYTE September[]				=	{0};
CONST UBYTE October[]				=	{0};
CONST UBYTE November[]				=	{0};
CONST UBYTE December[]				=	{0};

CONST UBYTE Yes[]					=	{0};
CONST UBYTE No[]					=	{0};

CONST UBYTE Morning[]				=	{0};
CONST UBYTE Evening[]				=	{0};

CONST UBYTE SoftHyphen[]			=	{0};
CONST UBYTE HardHyphen[]			=	{0};
CONST UBYTE OpenQuote[]				=	{0};
CONST UBYTE CloseQuote[]			=	{0};

CONST UBYTE Yesterday[]				=	{0};
CONST UBYTE Today[]					=	{0};
CONST UBYTE Tomorrow[]				=	{0};
CONST UBYTE Future[]				=	{0};

CONST UBYTE *InternalCatalogString[] =
{
	/*
	** The Following String Constants need to be updated for actual usage
	** 72 Strings are provided
	*/
	(APTR)"kor",											/* ISO-639-2 ID */
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
	ULONG rc=ALL_LANGFUNCS;
	return(rc);
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
