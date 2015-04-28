#ifndef PERCEPTION_LIBRARY_H
#define PERCEPTION_LIBRARY_H 1
/*
\\		"Perception"
//
\\		AmigaOS 4.x & AROS Input Method Editor Stack
*/
#include <exec/types.h>
#include <exec/resident.h>
#include <exec/ports.h>
#include <exec/libraries.h>
#include <exec/semaphores.h>
#include <exec/execbase.h>
#include <exec/interfaces.h>
#include <graphics/gfxbase.h>
#include <intuition/intuition.h>
#include <intuition/screens.h>
#include <intuition/intuitionbase.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include <utility/tagitem.h>
#include <utility/hooks.h>
#include <rexx/rxslib.h>
#include <rexx/storage.h>

#include <devices/input.h>
#include <devices/inputevent.h>

#include <libraries/keymap.h>
#include <libraries/perception.h>

#define __USE_SYSBASE
#define __NOLIBBASE__
#define __NOGLOBALIFACE__

#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/dos.h>
#include <proto/commodities.h>
#include <proto/utility.h>
#include <proto/locale.h>
#include <proto/keymap.h>
#include <proto/rexxsyslib.h>
#include <proto/application.h>

#include <interfaces/exec.h>
#include <interfaces/graphics.h>
#include <interfaces/intuition.h>
#include <interfaces/dos.h>
#include <interfaces/commodities.h>
#include <interfaces/locale.h>
#include <interfaces/keymap.h>
#include <interfaces/utility.h>
#include <interfaces/rexxsyslib.h>
#include <interfaces/application.h>
#include <interfaces/perception.h>

/*
\\	KDEBUG() macro

	Usage = KDEBUG("At line [%lld] address [%lld]\n", __LINE__, address)
*/
#ifndef KDEBUG
#define KDEBUG(format, argv...) \
	((struct ExecIFace *)((*(struct ExecBase **)4)->MainInterface))->DebugPrintF(format, ## argv)
#endif

/*
\\	"Perception"	Developer Materials
*/
#define LIBRARY_NAME		"perception.library"
#define LIBRARY_IDENTITY	"perception.library v1.0"

#define LIBRARY_CLASS		PerceptionBase
#define	LIBIFACE_CLASS		PerceptionIFace
#define LIBRARY_VERSION     2
#define LIBRARY_REVISION    80

struct LIBRARY_CLASS
{
	struct	Library	Library;
	BPTR	SegmentList;
	struct	SignalSemaphore	Lock;
/**/
	struct	ExecIFace			*IExec;
	struct	GraphicsIFace		*IGfx;
	struct	IntuitionIFace		*IIntuition;
	struct	DOSIFace			*IDOS;
	struct	UtilityIFace		*IUtility;
	struct	LocaleIFace			*ILocale;
	struct	KeymapIFace			*IKeymap;
	struct	CommoditiesIFace	*ICX;
	struct	RexxSysIFace		*IRexxSys;
/**/
	APTR	DaemonProcess;
	APTR	DaemonDHandle;
/**/
	struct	List	InputContextList;
/**/
	APTR			InputContext;           		/*	Active LanguageContext for InputHandler */
    APTR			CurrentLanguage;				/*  Active LanguageContext for PluginCalls  */
/**/
};

#define     IME_STATE_SIZE		64L		/* Size is Arbitrary and subject to change				*/
#define		IME_MESSAGE_SIZE	16L		/* Size is Arbitrary and subject to change				*/
#define     IME_VECTOR_SIZE     512L	/* Size is NOT-Arbitrary and still subject to change!	*/

/*
	The Global Context (Each Language provides its own LanguageContext

	This structure is the basic definition for both Input and Language Context structures,

		LanguageContext Attributes are embedded as TagItem Chains within this structure.
		This overload of TagItems within a fixed structure allows the Language modules
		to override and use Internal DataBase expansion upon the Language specific contexts
		along with overloading the attributes for other purposes
*/
struct InputContext
{
	struct	SignalSemaphore		Lock;
	struct	LanguageContextHook	Hook;
	ULONG						State[IME_STATE_SIZE];
	struct	TagItem				Message[IME_MESSAGE_SIZE];
	struct	InputTagItem		Vector[IME_VECTOR_SIZE];
};
/*
	"InputTagItem"s are deliberately the same size as "TagItem" structures for interchange purposes
	When the "Hook" within the InputContext is called from the Perception-IME process

	"State[]" has both System-Wide attributes defined following the size definition
		along with Language plugin specific Attributes stored within each Instance.

	Expect the InputContext to be subject to change based on which Language is currently active.
		User preferences decide which Languages are Active along with selection.

	LanguageContext usage ***overrides*** ALL InputContext Defined Attributes and behaviours
		so you need to look twice and DO NOT CHANGE OR RELY on InputContext Data,
		Instead rely exclusively on your own LanguageContext Allocation where possible.
*/
#define	IHCONTEXTSIZE    sizeof(struct InputContext)
#define	LHCONTEXTSIZE    sizeof(struct InputContext)

/*	InputContext State Information

	>> Global, Shared, Private << is the declared Attributes order

	Global Attributes make use of the first 24 entries, after that is any "shared" attributes,
	Attributes defined here may not use the last 32 Entries which are "private" and reserved
    for any Language Drivers which plugin to accessing the IME functionality

	Numbers here are arbitrary.
*/
#define	ICSTATE_FIFO_IW				(0)		/*  Index 	// InputHandler InputEvent->InputTagItem	*/
#define	ICSTATE_FIFO_IR				(1)		/*  Index 	// Translation of InputTagItem				*/
#define	ICSTATE_FIFO_PW				(2)		/*  Pointer // InputHandler InputEvent->InputTagItem	*/
#define	ICSTATE_FIFO_PR				(3)		/*  Pointer // Translation of InputTagItem				*/
#define ICSTATE_CURRENT_LOCALE		(16)	/*	Pointer // This is the Current System Locale		*/
#define ICSTATE_RESERVED			(64)	/*	Pointer // This is a Reserved Entry					*/

/**/
#define ICSTATE_MAX_ATTRIBUTE_COUNT	IME_STATE_SIZE	/*	ICSTATE Enumeration Ends at this value */

/*main.c*/
STATIC CONST struct Resident RomTag;
STATIC CONST BYTE LibName[];
STATIC CONST BYTE LibIdentity[];
extern CONST struct TagItem InitTable[];
extern CONST APTR LibManagerTable[];
extern CONST struct TagItem LibManagerTags[];
/*apicalls.c*/
extern CONST APTR LibInterfaces[];
extern CONST APTR LibInterfaceTable[];
extern CONST struct TagItem LibInterfaceTags[];

/*main.c*/
extern int32 _start(void);
extern struct library *LCALL_Init(struct LIBRARY_CLASS *SelfBase,
				 BPTR SegmentList,
				 struct Interface *Executive);
extern struct Library *LCALL_Open(struct LIBIFACE_CLASS *Iface, ULONG version);
extern APTR  LCALL_Close(struct LIBIFACE_CLASS *Iface);
extern APTR  LCALL_Expunge(struct LIBIFACE_CLASS *Iface);
extern APTR  LCALL_Reserved(void);
extern ULONG MCALL_Obtain(struct Interface *Iface);
extern ULONG MCALL_Release(struct Interface *Iface);
/*apicalls.c*/
extern APTR  LCALL_ScriptExec(struct LIBIFACE_CLASS *Self);
extern APTR  LCALL_OptionTagList(struct LIBIFACE_CLASS *Self, struct TagItem *options);
extern APTR  LCALL_ObtainLanguageContext(struct LIBIFACE_CLASS *Self, APTR name, APTR hook);
extern APTR  LCALL_ReleaseLanguageContext(struct LIBIFACE_CLASS *Self, APTR name);
extern ULONG LCALL_GetLanguageContextAttr(struct LIBIFACE_CLASS *Self, APTR lc, APTR m);
extern ULONG LCALL_SetLanguageContextAttr(struct LIBIFACE_CLASS *Self, APTR lc, APTR m);
/*daemon.c*/
extern void  InitPerceptionDaemon(struct LIBRARY_CLASS *Self);
extern void  ExitPerceptionDaemon(struct LIBRARY_CLASS *Self);
extern int32 ExecPerceptionDaemon(STRPTR argv, ULONG argc);
extern ULONG LanguageDefaultHook(struct LanguageContextHook *lch,APTR LanguageContext,APTR m);
/*objects.c*/
extern APTR  NewInputContext(struct LIBRARY_CLASS *Self,APTR InputContextHook);
extern void  EndInputContext(struct LIBRARY_CLASS *Self,struct InputContext *Key);
extern void  InitInputContext(struct InputContext *ic,APTR ich);
extern void  ExitInputContext(struct InputContext *ic);
extern void  InitLanguageContext(struct InputContext *lc,APTR LHook);
extern void  ExitLanguageContext(struct InputContext *lc);
extern void  TranslateCP32UTF8(ULONG *codepoint);
/*extern void  TranslateUTF8CP32(struct TagItem *item);*/
/**/

#endif                 
/*PERCEPTION_LIBRARY_H*/
