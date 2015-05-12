/*
\\		"Perception"
//
\\		AmigaOS 4.x & AROS Input Method Editor Stack
*/
#include "perception.h"

/**/
APTR  GetInputContext(APTR name,struct PerceptionIFace *IPerception)
{
	APTR rc=NULL;
	struct LIBRARY_CLASS *PerceptionBase=NULL;

	if(IPerception)
		PerceptionBase = (APTR)IPerception->Data.LibBase;
	if(PerceptionBase)
	{
		if(name)
		{
			PerceptionBase->IExec->ObtainSemaphore(&PerceptionBase->Lock);
			rc=PerceptionBase->IExec->FindName(&PerceptionBase->InputContextList,name);
			PerceptionBase->IExec->ReleaseSemaphore(&PerceptionBase->Lock);
		}else{
			PerceptionBase->IExec->ObtainSemaphore(&PerceptionBase->Lock);
			rc=PerceptionBase->CurrentLanguage;
			PerceptionBase->IExec->ReleaseSemaphore(&PerceptionBase->Lock);
		};
	}

	return(rc);
}

/**/
void  SetInputContext(APTR ctxt,struct PerceptionIFace *IPerception)
{
	struct LIBRARY_CLASS *PerceptionBase=NULL;

	if(IPerception)
		PerceptionBase = (APTR)IPerception->Data.LibBase;
	if(PerceptionBase)
	{
		if(ctxt)
		{
			PerceptionBase->IExec->ObtainSemaphore(&PerceptionBase->Lock);
			PerceptionBase->CurrentLanguage=ctxt;
			PerceptionBase->IExec->ReleaseSemaphore(&PerceptionBase->Lock);
		}
	}

	return;
}

/**/
void  NextInputContext(struct PerceptionIFace *IPerception)
{
	struct LIBRARY_CLASS *PerceptionBase=NULL;
	struct Node *n=NULL;

	if(IPerception)
		PerceptionBase = (APTR)IPerception->Data.LibBase;
	if(PerceptionBase)
	{
		if(PerceptionBase->CurrentLanguage)
		{
            PerceptionBase->IExec->ObtainSemaphore(&PerceptionBase->Lock);
			n=PerceptionBase->CurrentLanguage;
			if(n->ln_Succ)
				PerceptionBase->CurrentLanguage=n->ln_Succ;
            PerceptionBase->IExec->ReleaseSemaphore(&PerceptionBase->Lock);
		}else{
            PerceptionBase->IExec->ObtainSemaphore(&PerceptionBase->Lock);
			PerceptionBase->CurrentLanguage=PerceptionBase->InputContextList.lh_Head;
			PerceptionBase->IExec->ReleaseSemaphore(&PerceptionBase->Lock);
		};
	}

	return;
}

/**/
APTR  ReadInputItem(struct InputContext *ctxt)
{
	APTR rc=NULL;
	ULONG Idx=0L;

	if(ctxt)
	{
    	Idx	= ctxt->State[ICSTATE_FIFO_IW];
	    rc	= ctxt->State[ICSTATE_FIFO_PW];
	}
    if(rc==NULL)
		rc=ctxt->Vector;

	return(rc);
}

/**/
APTR  UpdateInputItem(struct InputContext *ctxt)
{
	struct InputTagItem *rc=NULL;
	ULONG Idx=0L;

	if(ctxt)
	{
		Idx	= ctxt->State[ICSTATE_FIFO_IW];
		rc	= ctxt->State[ICSTATE_FIFO_PW];
	}
	if(Idx<IME_VECTOR_SIZE)
	{
		Idx++; rc++;
	}else{
		Idx=0L;rc=NULL;
	};
	ctxt->State[ICSTATE_FIFO_IW]=Idx;
	ctxt->State[ICSTATE_FIFO_PW]=rc;

	return((APTR)rc);
}

/**/
APTR  ReadOutputItem(struct InputContext *ctxt)
{
	APTR rc=NULL;
	ULONG Idx=0L;

	if(ctxt)
	{
    	Idx	= ctxt->State[ICSTATE_FIFO_IR];
	    rc	= ctxt->State[ICSTATE_FIFO_PR];
	}
    if(rc==NULL)
		rc=ctxt->Vector;

	return(rc);
}

/**/
APTR  UpdateOutputItem(struct InputContext *ctxt)
{
	struct InputTagItem *rc=NULL;
	ULONG Idx=0L;

	if(ctxt)
	{
		Idx	= ctxt->State[ICSTATE_FIFO_IR];
		rc	= ctxt->State[ICSTATE_FIFO_PR];
	}
	if(Idx<IME_VECTOR_SIZE)
	{
		Idx++; rc++;
	}else{
		Idx=0L;rc=NULL;
	};
	ctxt->State[ICSTATE_FIFO_IR]=Idx;
	ctxt->State[ICSTATE_FIFO_PR]=rc;

	return((APTR)rc);
}

/**/
void  InitLanguageContext(struct InputContext *lc,APTR LHook)
{
	ULONG x=0L;
	struct TagItem *Vector=(APTR)lc->Vector;

	lc->Hook.Hook.h_Entry=LHook;
	lc->Hook.Hook.h_Data=lc;

	for(x=0L;x<IME_STATE_SIZE;x++)
    {
		lc->State[x]	=0L;
	}
	for(x=0L;x<IME_MESSAGE_SIZE;x++)
    {
		lc->Message[x].ti_Tag	=	LANGUAGE_IME_NOP;
		lc->Message[x].ti_Data	=	LANGUAGE_IME_NOP_NULL;
	}
	for(x=0L;x<IME_VECTOR_SIZE;x++)
    {
		lc->Vector[x].type	=0x8000L;
		lc->Vector[x].qual	=0L;
		lc->Vector[x].glyph	=0L;
	}

	for(x=0L;x<IME_VECTOR_SIZE;x++)
    {
		Vector[x].ti_Tag	= TAG_USER+x;
		Vector[x].ti_Data	= (ULONG)0L;
	}

    return;
}

/**/
void  ExitLanguageContext(struct InputContext *lc)
{
	lc->Hook.Hook.h_Entry=NULL;
	return;
}

/**/
void  DefaultLanguageContext(struct InputContext *lc)
{
	return;
}

/**/
void  TranslateCP32UTF8(ULONG *codepoint)
{
	ULONG cpoint=0L, cglyph=0L;
    UBYTE *eglyph=(APTR)&cglyph;

	if(codepoint)
	{
		cpoint = codepoint[0];

		if((cpoint & 0x7F)== cpoint)
		{
			eglyph[0] = (UBYTE)(cpoint);
		}else if((cpoint & 0x7FF)==cpoint)
		{
			eglyph[0] = (UBYTE)(0xC0 | (((cpoint) & 0xFC0)>>6));
			eglyph[1] = (UBYTE)(0x80 | ( (cpoint) &  0x3F));
		}else if((cpoint & 0xFFFF)==cpoint)
		{
			eglyph[0] = (UBYTE)(0xE0 | (((cpoint) & 0xF000)>>12));
			eglyph[1] = (UBYTE)(0x80 | (((cpoint) &  0xFC0)>>6));
			eglyph[2] = (UBYTE)(0x80 | ( (cpoint) &   0x3F));
		}else if((cpoint & 0x1FFFFF)==cpoint)
		{
			eglyph[0] = (UBYTE)(0xF0 | (((cpoint) & 0x70000)>>18));
			eglyph[1] = (UBYTE)(0x80 | (((cpoint) &  0xF000)>>12));
			eglyph[2] = (UBYTE)(0x80 | (((cpoint) &   0xFC0)>>6));
			eglyph[3] = (UBYTE)(0x80 | ( (cpoint) &    0x3F));
/*
\\	The following two clauses are for standard completeness and exceed the 32bit storage available!
//
		}else if((cpoint & 0x3FFFFFF)==cpoint)
		{
			eglyph[0] = (UBYTE)(0xF8 | (((cpoint) & 0x3000000)>>24));
			eglyph[1] = (UBYTE)(0x80 | (((cpoint) &  0xFC0000)>>18));
			eglyph[2] = (UBYTE)(0x80 | (((cpoint) &   0x3F000)>>12));
			eglyph[3] = (UBYTE)(0x80 | (((cpoint) &     0xFC0)>>6));
			eglyph[4] = (UBYTE)(0x80 | ( (cpoint) &      0x3F));
		}else if((cpoint & 0x7FFFFFFF)==cpoint)
		{
			eglyph[0] = (UBYTE)(0xFE | (((cpoint) & 0xC0000000)>>30));
			eglyph[1] = (UBYTE)(0x80 | (((cpoint) & 0x3F000000)>>24));
			eglyph[2] = (UBYTE)(0x80 | (((cpoint) &   0xFC0000)>>18));
			eglyph[3] = (UBYTE)(0x80 | (((cpoint) &    0x3F000)>>12));
			eglyph[4] = (UBYTE)(0x80 | (((cpoint) &      0xFC0)>>6));
			eglyph[5] = (UBYTE)(0x80 | ( (cpoint) &       0x3F));
*/
		};
		codepoint[0] = cglyph;
	};

	return;
}

/*
void  TranslateUTF8CP32(struct TagItem *item)
{
	ULONG cglyph=0L, cpoint=0L;
	UBYTE *eglyph=NULL;

	if(item)
	{
		eglyph=(APTR)&item->ti_Data;
		if(item->ti_Tag & TAG_USER)
		{
			cglyph=(ULONG)eglyph[0];
			if((cglyph & 0x7F)==cglyph)
			{
				cpoint=cglyph;
			}else if((cglyph & 0xC0)==cglyph)
			{
				cpoint =(((ULONG)eglyph[0] & 0x1F)<<6)  |
						 ((ULONG)eglyph[1] & 0x3F);
			}else if((cglyph & 0xE0)==cglyph)
			{
				cpoint =(((ULONG)eglyph[0] & 0x0F)<<12) |
						(((ULONG)eglyph[1] & 0x3F)<<6)  |
						 ((ULONG)eglyph[2] & 0x3F);
			}else if((cglyph & 0xF0)==cglyph)
			{
				cpoint =(((ULONG)eglyph[0] & 0x07)<<18) |
						(((ULONG)eglyph[1] & 0x3F)<<12) |
						(((ULONG)eglyph[2] & 0x3F)<<6)  |
						 ((ULONG)eglyph[3] & 0x3F);
**
\\	The following two clauses are for standard completeness and exceed the 32bit storage available!
//
			}else if((cglyph & 0xF8)==cglyph)
			{
				cpoint =(((ULONG)eglyph[0] & 0x07)<<24) |
						(((ULONG)eglyph[1] & 0x3F)<<18) |
						(((ULONG)eglyph[2] & 0x3F)<<12) |
						(((ULONG)eglyph[3] & 0x3F)<<6)  |
						 ((ULONG)eglyph[4] & 0x3F);
			}else if((cglyph & 0xFE)==cglyph)
			{
				cpoint =(((ULONG)eglyph[0] & 0x03)<<30) |
						(((ULONG)eglyph[1] & 0x3F)<<24) |
						(((ULONG)eglyph[2] & 0x3F)<<18) |
						(((ULONG)eglyph[3] & 0x3F)<<12) |
						(((ULONG)eglyph[4] & 0x3F)<<6)  |
						 ((ULONG)eglyph[5] & 0x3F);
**
			}
			item->ti_Data=cpoint;
		}
	}

	return;
}
*/
/**/
