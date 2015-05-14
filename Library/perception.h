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
#include <libraries/locale.h>
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
	struct	List	LanguageContextList;
/**/
    APTR			CurrentLanguage;				/*  Active LanguageContext for PluginCalls  */
/**/
};

/*	LanguageContext State Information

	>> Global, Shared, Private << is the declared Attributes order

	Global Attributes make use of the first 24 entries, after that is any "shared" attributes,
	Attributes defined here may not use the last 32 Entries which are "private" and reserved
    for any Language Drivers which plugin to accessing the IME functionality

	Numbers here are arbitrary.
*/
#define	ICSTATE_FIFO_IVW		(0)		//  Index 	// InputHandler InputEvent->InputTagItem
#define	ICSTATE_FIFO_IVR		(1)		//  Index 	// Translation of InputTagItem
#define	ICSTATE_FIFO_PVW		(2)		//  Index 	// InputHandler InputEvent->InputTagItem
#define	ICSTATE_FIFO_PVR		(3)		//  Index 	// Translation of InputTagItem
/**/
#define ICSTATE_MAX_COUNT		IME_STATE_SIZE		// ICSTATE Enumeration Ends at this value

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
/*objects.c*/
extern APTR  GetInputContext(APTR name,struct PerceptionIFace *IPerception);
extern void  SetInputContext(APTR ctxt,struct PerceptionIFace *IPerception);
extern void  InitLanguageContext(struct LanguageContext *lc);
extern void  ExitLanguageContext(struct LanguageContext *lc);
extern void  TranslateCP32UTF8(ULONG *codepoint);
/*extern void  TranslateUTF8CP32(struct TagItem *item);*/
/**/

#endif                 
/*PERCEPTION_LIBRARY_H*/
