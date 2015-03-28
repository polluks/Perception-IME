/*
\\		"Perception"
//
\\		Language Driver Functionality
*/
#include "charset.h"

CONST struct TagItem LibInterfaceTags[] =
{
	    { MIT_Name,        (ULONG)"main"			},
	    { MIT_VectorTable, (ULONG)LibInterfaceTable	},
	    { MIT_Version,     1                    	},
	    { TAG_END,         0                    	}
};

CONST APTR LibInterfaces[] =
{
		LibManagerTags,
		LibInterfaceTags,
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

#define LOCALECODESET		104	/* ISO-2022-CN */

/*  32bit Mask Value for Routines Actually implimented
*/
ULONG LCALL_GetDriverInfo(struct LIBIFACE_CLASS *IFace)
{
	ULONG rc=ALL_FUNCS;	/* *,ALL_CHARSETFUNCS,ALL_LANGFUNCS */
	return(rc);
};

ULONG LCALL_ConvToLower(struct LIBIFACE_CLASS *IFace,ULONG c)
{
	ULONG rc=0L;
	return(rc);
};

ULONG LCALL_ConvToUpper(struct LIBIFACE_CLASS *IFace,ULONG c)
{
	ULONG rc=0L;
	return(rc);
};

ULONG LCALL_GetCodeSet(struct LIBIFACE_CLASS *IFace)
{
	ULONG rc=LOCALECODESET;
	return(rc);
};

/*	Two Boundary conditions were deliberately enforced using Input Typing
 *   and the value comparison,  this allows for a shorter routine to be written.
 *   No point in wasting space without need is there?
*/
APTR  LCALL_GetLocaleStr(struct LIBIFACE_CLASS *IFace,ULONG cStringID)
{
	APTR rc=NULL;
/*
	if(cStringID < MAXLOCALESTRINGS)
		rc=(APTR)InternalCatalogString[cStringID];
*/
	return(rc);
};

LONG  LCALL_IsAlNum(struct LIBIFACE_CLASS *IFace, ULONG c)
{
	LONG rc=0L;
	return(rc);
};

LONG  LCALL_IsAlpha(struct LIBIFACE_CLASS *IFace, ULONG c)
{
	LONG rc=0L;
	return(rc);
};

LONG  LCALL_IsCntrl(struct LIBIFACE_CLASS *IFace, ULONG c)
{
	LONG rc=0L;
	return(rc);
};

LONG  LCALL_IsDigit(struct LIBIFACE_CLASS *IFace, ULONG c)
{
	LONG rc=0L;
	return(rc);
};

LONG  LCALL_IsLower(struct LIBIFACE_CLASS *IFace, ULONG c)
{
	LONG rc=0L;
	return(rc);
};

LONG  LCALL_IsPrint(struct LIBIFACE_CLASS *IFace, ULONG c)
{
	LONG rc=0L;
	return(rc);
};

LONG  LCALL_IsPunct(struct LIBIFACE_CLASS *IFace, ULONG c)
{
	LONG rc=0L;
	return(rc);
};

LONG  LCALL_IsSpace(struct LIBIFACE_CLASS *IFace, ULONG c)
{
	LONG rc=0L;
	return(rc);
};

LONG  LCALL_IsUpper(struct LIBIFACE_CLASS *IFace, ULONG c)
{
	LONG rc=0L;
	return(rc);
};

LONG  LCALL_IsXDigit(struct LIBIFACE_CLASS *IFace, ULONG c)
{
	LONG rc=0L;
	return(rc);
};

APTR  LCALL_StrConvert(	struct LIBIFACE_CLASS *IFace,
						ULONG srcLength,	ULONG dstLength,
						APTR srcStringPtr,	APTR dstStringPtr)
{
	APTR rc=NULL;
	return(rc);
};

APTR  LCALL_StrnCmp(	struct LIBIFACE_CLASS *IFace,
					ULONG srcLength,	ULONG dstLength,
					APTR srcStringPtr,	APTR dstStringPtr)
{
	APTR rc=NULL;
	return(rc);
};

LONG  LCALL_IsBlank(	struct LIBIFACE_CLASS *IFace, ULONG c)
{
	LONG rc=0L;
	return(rc);
};

/**/
