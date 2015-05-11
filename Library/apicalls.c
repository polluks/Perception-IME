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
	APTR rc=NULL;
	struct LIBRARY_CLASS *Self = (APTR) iface->Data.LibBase;

	return(rc);
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
	APTR rc=NULL;
	struct LIBRARY_CLASS *Self = (APTR) iface->Data.LibBase;

	if(options)
		Self=Self;

	return(rc);
}

/****** perception.library/ObtainLanguageContext() *************************
*
*	NAME
*		ObtainLanguageContext()
*
*	SYNOPSIS
*		rc = ObtainLanguageContextTagList(UBYTE *LanguageName,struct LanguageContextHook *hook);
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
	struct InputContext *rc=NULL;
	struct LIBRARY_CLASS *Self = (APTR) iface->Data.LibBase;

	if(name)
	{
		Self->IExec->ObtainSemaphore(&Self->Lock);
		rc=(APTR)Self->IExec->FindName(&Self->InputContextList,name);
		Self->IExec->ReleaseSemaphore(&Self->Lock);
		if(rc==NULL)
		{
			rc=(APTR)Self->IExec->AllocVecTags(IHCONTEXTSIZE,MEMF_SHARED);
			if(rc)
			{
				Self->IExec->InitSemaphore((APTR)rc);
				InitLanguageContext((APTR)rc,(APTR)hook);
				rc->Lock.ss_Link.ln_Name=name;
				Self->IExec->ObtainSemaphore(&Self->Lock);
				Self->IExec->AddTail(&Self->InputContextList,(APTR)rc);
				Self->CurrentLanguage=rc;
				Self->IExec->ReleaseSemaphore(&Self->Lock);
				KDEBUG("Perception.Library//ObtainLanguageContext[%lx/%s]\n",rc,name);
			}
		}
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
	APTR rc=NULL;
	struct LIBRARY_CLASS *Self = (APTR) iface->Data.LibBase;

	ExitLanguageContext(o);
	Self->IExec->ObtainSemaphore(&Self->Lock);
	Self->IExec->Remove(o);
	Self->IExec->ReleaseSemaphore(&Self->Lock);
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
	ULONG rc=0L, *msg=m;
	struct LIBRARY_CLASS *Self = (APTR) iface->Data.LibBase;

	switch(msg[0])
	{
		case 0L:
		default:
			break;
	}

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
	ULONG rc=0L, *msg=m;
	struct LIBRARY_CLASS *Self = (APTR) iface->Data.LibBase;

	switch(msg[0])
	{
		case 0L:
		default:
			break;
	}

	return(rc);
}

/**/
