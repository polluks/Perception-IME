/*
\\		"Perception"
*/
#include "perception.h"

/*
 * DO NOT MODIFY THIS ANY FURTHER!!!! WE DON'T NEED ANY HIDDEN SPECIAL FUNCTIONS
*/
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
/**/
  (APTR)&LCALL_ScriptExec,
  (APTR)&LCALL_OptionTagList,
  (APTR)&LCALL_ObtainLanguageContext,
  (APTR)&LCALL_ReleaseLanguageContext,
  (APTR)&LCALL_GetLanguageContextAttr,
  (APTR)&LCALL_SetLanguageContextAttr,
/**/
/**/
  (APTR)-1
};

/****** perception.library/-Background- *************************************
*
*   BACKGROUND
*
*	I began this project in an attempt to create a modular expansion to the
*	 L10n support on AmigaOS 4.1 and newer to have Japanese support.
*
*	I additionally gave consideration to allowing additional languages to
*     be pluggable into this IME as a more universal option.
*
*****************************************************************************
*
*/

/****** perception.library/-RexxHost- ***************************************
*
*   REXXHOST
*
*	ARexx is supported through the -30 Library Vector Offset
*
*****************************************************************************
*
*/
APTR LCALL_ScriptExec(struct LIBIFACE_CLASS *iface)
{
	struct LIBRARY_CLASS *Self = (APTR) iface->Data.LibBase;

	return(NULL);
}

/****** perception.library/OptionTagList() **********************************
*
*	NAME
*		PerceptionOptionTags(TAG_X,value,TAG_Y,value,...,TAG_END,NULL);
*		PerceptionOptionTagList(struct TagItem *);
*
*	SYNOPSIS
*
*	DESCRIPTION
*
*****************************************************************************
*
*/
APTR LCALL_OptionTagList(struct LIBIFACE_CLASS *iface, struct TagItem *options)
{
	struct LIBRARY_CLASS *Self = (APTR) iface->Data.LibBase;
	struct TagItem *mSTATE=options, *Method=NULL;

	while((Method=Self->IUtility->NextTagItem(&mSTATE)))
		switch(Method->ti_Tag)
		{
			default:
				break;
		};

	return(NULL);
}

/****** perception.library/ObtainLanguageContext() *************************
*
*	NAME
*		ObtainLanguageContext()
*
*	SYNOPSIS
*		rc = ObtainLanguageContextTags(APTR LanguageName,TAG_END,NULL);
*		rc = ObtainLanguageContextTagList(UBYTE *LanguageName,struct TagItem *tags);
*
*	DESCRIPTION
*
*		Objects on the Internal List are always a Language Name,
*		and this function is to find any existing entry or create a new entry.
*
*****************************************************************************
*
*/
APTR LCALL_ObtainLanguageContext(struct LIBIFACE_CLASS *iface,APTR name,APTR hook)
{
	struct LIBRARY_CLASS *Self = (APTR) iface->Data.LibBase;
	struct	InputContext *rc=NULL;

	if(name)
	{
		Self->IExec->ObtainSemaphore(&Self->Lock);
		rc=(APTR)Self->IExec->FindName(&Self->InputContextList,name);
		if(!rc)
		{
			rc=(APTR)Self->IExec->AllocVecTags(IHCONTEXTSIZE,MEMF_SHARED);
			InitLanguageContext((APTR)rc,(APTR)hook);
			Self->IExec->InitSemaphore((APTR)rc);
			Self->IExec->AddHead(&Self->InputContextList,(APTR)rc);
		};
		Self->IExec->ReleaseSemaphore(&Self->Lock);
    }
	return((APTR)rc);
}

/****** perception.library/ReleaseLanguageContext() ************************
*
*	NAME
*		void ReleaseLanguageContext(APTR o)
*
*	SYNOPSIS
*
*	DESCRIPTION
*
*****************************************************************************
*
*/
APTR LCALL_ReleaseLanguageContext(struct LIBIFACE_CLASS *iface, APTR o)
{
	struct LIBRARY_CLASS *Self = (APTR) iface->Data.LibBase;
	APTR rc=NULL;

	Self->IExec->FreeVec(o);

	return(rc);
}

/****** perception.library/GetLanguageContextAttr() ************************
*
*	NAME
*		GetLanguageContextAttr()
*
*	SYNOPSIS
*
*	DESCRIPTION
*
*****************************************************************************
*
*/
ULONG LCALL_GetLanguageContextAttr(struct LIBIFACE_CLASS *iface, APTR lc, APTR m)
{
	struct LIBRARY_CLASS *Self = (APTR) iface->Data.LibBase;
	ULONG rc=0L, *Method=m;
	struct InputContext *LanguageContext=lc;

	Self->IExec->ObtainSemaphore(&Self->Lock);
	switch(Method[0])
	{
		case DEFAULT_SYSTEM_LANGUAGE:
			rc=(ULONG)Self->CurrentLanguage;
			break;
		case DEFAULT_ENABLE_LANGUAGE:
			rc=LanguageContext->State[ICSTATE_CURRENT_LOCALE];
			break;
		case LCSTATE_VECTOR:
			rc=(ULONG)LanguageContext->Vector;
			break;
		default:
			break;
	}
	Self->IExec->ReleaseSemaphore(&Self->Lock);

	return(rc);
}

/****** perception.library/SetLanguageContextAttr() ************************
*
*	NAME
*		SetLanguageContextAttr()
*
*	SYNOPSIS
*
*	DESCRIPTION
*
*****************************************************************************
*
*/
ULONG LCALL_SetLanguageContextAttr(struct LIBIFACE_CLASS *iface, APTR lc, APTR m)
{
	struct LIBRARY_CLASS *Self = (APTR) iface->Data.LibBase;
	ULONG rc=0L, *Method=m;
	struct InputContext *LanguageContext=lc;

	Self->IExec->ObtainSemaphore(&Self->Lock);
	switch(Method[0])
	{
		case DEFAULT_SYSTEM_LANGUAGE:
			if(!Self->CurrentLanguage)
			{
				Self->CurrentLanguage=lc;
			}
			break;
		default:
			break;
	}
	Self->IExec->ReleaseSemaphore(&Self->Lock);

	return(rc);
}

/**/
