#ifndef LANGUAGE_DRIVER_H
#define LANGUAGE_DRIVER_H
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
#include <rexx/storage.h>

#include <libraries/perception.h>

#define __USE_SYSBASE
#define __NOLIBBASE__
#define __NOGLOBALIFACE__

#include <proto/exec.h>
#include <proto/locale.h>
#include <proto/utility.h>
#include <proto/perception.h>

#include <interfaces/exec.h>
#include <interfaces/locale.h>
#include <interfaces/utility.h>
#include <interfaces/perception.h>

/*
\\	KDEBUG() macro
//
\\	Usage = KDEBUG("At line [%lld] address [%lld]\n", __LINE__, address)
*/
#ifndef KDEBUG
#define KDEBUG(format, argv...) \
	((struct ExecIFace *)((*(struct ExecBase **)4)->MainInterface))->DebugPrintF(format, ## argv)
#endif

/*
\\	"Perception"	Developer Materials
*/
#define LIBRARY_NAME		"japanese.language"
#define LIBRARY_IDENTITY	"Japanese.Language r0"

#define LIBRARY_CLASS		LanguageBase
#define	LIBIFACE_CLASS      LibraryManagerInterface
#define LIBRARY_VERSION		1
#define LIBRARY_REVISION	0

struct LIBRARY_CLASS
{
	struct Library Library;
	BPTR SegmentList;

	struct ExecIFace *IExec;
	struct UtilityIFace *IUtility;
	struct PerceptionIFace *IPerception;

	struct SignalSemaphore Lock;

	APTR HPerception;

    struct TagItem *RPerception;
};

STATIC CONST struct Resident RomTag;
STATIC CONST BYTE LibName[];
STATIC CONST BYTE LibIdentity[];
extern CONST struct TagItem InitTable[];
extern CONST APTR LibInterfaces[];
extern CONST APTR LibManagerTable[];
extern CONST struct TagItem LibManagerTags[];
extern CONST APTR LibInterfaceTable[];
extern CONST struct TagItem LibInterfaceTags[];
extern CONST APTR EmulationTable[];
extern CONST struct TagItem LibEmulationTags[];

#define GDIB_CONVTOLOWER    0
#define GDIB_CONVTOUPPER    1
#define GDIB_GETCODESET     2
#define GDIB_GETLOCALESTR   3
#define GDIB_ISALNUM        4
#define GDIB_ISALPHA        5
#define GDIB_ISCNTRL        6
#define GDIB_ISDIGIT        7
#define GDIB_ISGRAPH        8
#define GDIB_ISLOWER        9
#define GDIB_ISPRINT        10
#define GDIB_ISPUNCT        11
#define GDIB_ISSPACE        12
#define GDIB_ISUPPER        13
#define GDIB_ISXDIGIT       14
#define GDIB_STRCONVERT     15
#define GDIB_STRNCMP        16
#define GDIB_ISBLANK        17

#define GDIF_CONVTOLOWER    (1<<0)
#define GDIF_CONVTOUPPER    (1<<1)
#define GDIF_GETCODESET     (1<<2)
#define GDIF_GETLOCALESTR   (1<<3)
#define GDIF_ISALNUM        (1<<4)
#define GDIF_ISALPHA        (1<<5)
#define GDIF_ISCNTRL        (1<<6)
#define GDIF_ISDIGIT        (1<<7)
#define GDIF_ISGRAPH        (1<<8)
#define GDIF_ISLOWER        (1<<9)
#define GDIF_ISPRINT        (1<<10)
#define GDIF_ISPUNCT        (1<<11)
#define GDIF_ISSPACE        (1<<12)
#define GDIF_ISUPPER        (1<<13)
#define GDIF_ISXDIGIT       (1<<14)
#define GDIF_STRCONVERT     (1<<15)
#define GDIF_STRNCMP        (1<<16)
#define GDIF_ISBLANK        (1<<17)

#define ALL_FUNCS           0x0003ffff
#define ALL_LANGFUNCS       0x0000000c /* GETCODESET|GETLOCALESTR */
#define ALL_CHARSETFUNCS    0x0003fff3 /* opposite of ALL_LANGFUNCS */

/*main.c*/
extern int32 _start(void);
extern struct library *LCALL_Init(struct LIBRARY_CLASS *Self,
				 BPTR SegmentList,
				 struct Interface *Executive);
extern struct Library *LCALL_Open(struct LibraryManagerInterface *Iface, ULONG version);
extern APTR  LCALL_Close(struct LibraryManagerInterface *Iface);
extern APTR  LCALL_Expunge(struct LibraryManagerInterface *Iface);
extern APTR  LCALL_Reserved(void);
extern ULONG MCALL_Obtain(struct Interface *Iface);
extern ULONG MCALL_Release(struct Interface *Iface);
/*apicalls.c*/
extern ULONG LCALL_GetDriverInfo(struct LIBIFACE_CLASS *Iface);
extern ULONG LCALL_ConvToLower(struct LIBIFACE_CLASS *Iface,ULONG c);
extern ULONG LCALL_ConvToUpper(struct LIBIFACE_CLASS *Iface,ULONG c);
extern ULONG LCALL_GetCodeSet(struct LIBIFACE_CLASS *Iface);
extern APTR  LCALL_GetLocaleStr(struct LIBIFACE_CLASS *Iface,ULONG cStringID);
extern LONG  LCALL_IsAlNum(struct LIBIFACE_CLASS *Iface, ULONG c);
extern LONG  LCALL_IsAlpha(struct LIBIFACE_CLASS *Iface, ULONG c);
extern LONG  LCALL_IsCntrl(struct LIBIFACE_CLASS *Iface, ULONG c);
extern LONG  LCALL_IsDigit(struct LIBIFACE_CLASS *Iface, ULONG c);
extern LONG  LCALL_IsLower(struct LIBIFACE_CLASS *Iface, ULONG c);
extern LONG  LCALL_IsPrint(struct LIBIFACE_CLASS *Iface, ULONG c);
extern LONG  LCALL_IsPunct(struct LIBIFACE_CLASS *Iface, ULONG c);
extern LONG  LCALL_IsSpace(struct LIBIFACE_CLASS *Iface, ULONG c);
extern LONG  LCALL_IsUpper(struct LIBIFACE_CLASS *Iface, ULONG c);
extern LONG  LCALL_IsXDigit(struct LIBIFACE_CLASS *Iface, ULONG c);
extern APTR  LCALL_StrConvert(	struct LIBIFACE_CLASS *Iface,
								ULONG srcLength,	ULONG dstLength,
								APTR srcStringPtr,	APTR dstStringPtr);
extern APTR  LCALL_StrnCmp(	struct LIBIFACE_CLASS *Iface,
							ULONG srcLength,	ULONG dstLength,
							APTR srcStringPtr,	APTR dstStringPtr);
extern LONG  LCALL_IsBlank(	struct LIBIFACE_CLASS *Iface, ULONG c);
/*hook.c*/
extern void  InitPerceptionHook(struct LIBRARY_CLASS *Self);
extern void  ExitPerceptionHook(struct LIBRARY_CLASS *Self);
extern ULONG ExecLanguageHook(struct Hook *h,struct LanguageContext *LanguageContext,ULONG *Message);
/**/

#endif
/*LANGUAGE_DRIVER_H*/
