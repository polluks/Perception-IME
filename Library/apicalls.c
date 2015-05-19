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
	struct LanguageContext *rc=NULL;
	struct LIBRARY_CLASS *Self = (APTR) iface->Data.LibBase;

	if(name)
	{
		Self->IExec->ObtainSemaphore(&Self->Lock);
//		rc=(APTR)Self->IExec->FindName(&Self->LanguageContextList,name);
		KDEBUG("Perception.Library/ObtainLanguageContext(%s.%lx)\n",name,hook);
		Self->IExec->ReleaseSemaphore(&Self->Lock);
	};
	if(rc==NULL)
	{
		rc=(APTR)Self->IExec->AllocSysObjectTags( ASOT_HOOK,
			ASOHOOK_Size,		LHCONTEXTSIZE,
			ASOHOOK_Entry,		(ULONG)hook,
			ASOHOOK_Data,		0L,
			TAG_END,			0L);
		if(rc)
		{
			InitLanguageContext((APTR)rc);
			Self->IExec->ObtainSemaphore(&Self->Lock);
			Self->IExec->AddTail(&Self->LanguageContextList,(APTR)rc);
			Self->IExec->ReleaseSemaphore(&Self->Lock);
		};
	};

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

	KDEBUG("Perception.Library/ReleaseLanguageContext(o=%lx)\n",o);

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
ULONG LCALL_GetLanguageContextAttr(struct LIBIFACE_CLASS *iface, APTR lc, ULONG a)
{
	ULONG rc=0L, Message=&a;
	struct LIBRARY_CLASS *Self = (APTR) iface->Data.LibBase;
	struct LanguageContext *Language=lc;

	KDEBUG("Perception.Library/GetLanguageContextAttr(%lx,%lx)\n",lc,a);

	switch(a)
	{
		case LCSTATE_VECTOR:
			rc=(ULONG)&Language->Vector;
			break;
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
void  LCALL_SetLanguageContextAttr(struct LIBIFACE_CLASS *iface, APTR lc, ULONG a, ULONG b)
{
	ULONG rc=0L, *Message=&a;
	struct LIBRARY_CLASS *Self = (APTR) iface->Data.LibBase;

	KDEBUG("Perception.Library/SetLanguageContextAttr(%lx,%lx)\n",lc,a);

	switch(a)
	{
		case LCSTATE_VECTOR:	//	Expand the LCSTATE_VECTOR,  use of TAG_MORE is required
			break;
		default:
			break;
	}

	return(rc);
}

/**/
