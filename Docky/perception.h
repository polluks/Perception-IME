#ifndef PERCEPTION_DOCKY_H
#define PERCEPTION_DOCKY_H 1
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

#include <libraries/docky.h>

#include <libraries/perception.h>

#define __USE_SYSBASE
#define __NOLIBBASE__
#define __NOGLOBALIFACE__

#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/locale.h>
#include <proto/docky.h>

#include <interfaces/exec.h>
#include <interfaces/graphics.h>
#include <interfaces/intuition.h>
#include <interfaces/dos.h>
#include <interfaces/utility.h>
#include <interfaces/locale.h>
#include <interfaces/perception.h>
#include <interfaces/docky.h>

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
#define LIBRARY_NAME		"perception.docky"
#define LIBRARY_IDENTITY	"perception.docky v1.0"

#define LIBRARY_CLASS		PerceptionDockBase
#define	LIBIFACE_CLASS		PerceptionIFace
#define LIBRARY_VERSION     64
#define LIBRARY_REVISION    1

struct LIBRARY_CLASS
{
	struct Library Library;
	BPTR SegmentList;
	struct SignalSemaphore Lock;

	struct ExecIFace *IExec;
	struct GraphicsIFace *IGfx;
	struct IntuitionIFace *IIntuition;
	struct DOSIFace *IDOS;
	struct UtilityIFace *IUtility;
	struct LocaleIFace *ILocale;

	/*	Global Attributes here
	*/
};

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
extern BOOL  LCALL_DockyGet(struct Interface *Self, uint32 msgType, uint32 *msgData);
extern BOOL  LCALL_DockySet(struct Interface *Self, uint32 msgType, uint32 msgData);
extern BOOL  LCALL_DockyExec(struct Interface *Self, uint32 TurnCnt, uint32 msgType, uint32 msgData, BOOL Again);
/**/

#endif
/*PERCEPTION_DOCKY_H*/
