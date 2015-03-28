/*
\\		"Perception"
//
\\		AmigaOS 4.x & AROS Input Method Editor Stack
*/
#include "perception.h"

/**/
APTR  NewInputContext(struct LIBRARY_CLASS *Self,APTR InputContextHook)
{
	struct InputContext *rc=NULL;

	if(Self)
		rc=(APTR)Self->IExec->AllocVecTags(IHCONTEXTSIZE,MEMF_SHARED);
	if(rc)
		InitInputContext(rc,InputContextHook);

	return((APTR)rc);
}

void  EndInputContext(struct LIBRARY_CLASS *Self,struct InputContext *Key)
{
	if(Self)
		if(Key)
        	Self->IExec->FreeVec(Key);
	return;
}

void  InitInputContext(struct InputContext *ic,APTR ich)
{
	ULONG x;
	ic->Hook.Hook.h_Entry=ich;
	for(x=0L;x<IME_STATE_SIZE;x++)
    {
		ic->State[x]	=0L;
	}
	for(x=0L;x<IME_MESSAGE_SIZE;x++)
    {
		ic->Message[x].ti_Tag	=	LANGUAGE_IME_NOP;
		ic->Message[x].ti_Data	=	LANGUAGE_IME_NOP_NULL;
	}
	for(x=0L;x<IME_VECTOR_SIZE;x++)
    {
		ic->Vector[x].type	=0x8000L;
		ic->Vector[x].qual	=0L;
		ic->Vector[x].glyph	=0L;
	}

    return;
}

void  ExitInputContext(struct InputContext *ic)
{
	ic->Hook.Hook.h_Entry=NULL;
	return;
}

/**/
void  InitLanguageContext(struct InputContext *lc,APTR LHook)
{
	ULONG x=0L;
	APTR  h=LHook;
	struct TagItem *Vector=(APTR)lc->Vector;
	if(!h)
		h=(APTR)&LanguageDefaultHook;
	InitInputContext(lc,h);
	lc->Hook.Hook.h_Data=lc;

	/*DEBUG: LanguageContext specific setup */

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
	ExitInputContext(lc);
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
