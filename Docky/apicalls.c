/*
\\		"Perception"
*/
#include "perception.h"

/*
 * DO NOT MODIFY THIS ANY FURTHER!!!! WE DON'T NEED ANY HIDDEN SPECIAL FUNCTIONS
*/
CONST struct TagItem LibInterfaceTags[] =
{
	    { MIT_Name,        (ULONG)"main"            },
	    { MIT_VectorTable, (ULONG)LibInterfaceTable	},
	    { MIT_Version,     1                        },
	    { TAG_END,         0                        }
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
  (APTR)&LCALL_DockyGet,
  (APTR)&LCALL_DockySet,
  (APTR)&LCALL_DockExec,
/**/
  (APTR)-1
};

/**/
BOOL  LCALL_DockyGet(struct Interface *Self, uint32 msgType, uint32 *msgData)
{
	BOOL	rc=TRUE;
	struct LIBRARY_CLASS *Base=(APTR)Self->Data.LibBase;

	KDEBUG("AMIdock<->Perception.Docky::Get(msgType=%lx, msgData=%lx)\n", msgType, msgData);

	switch(msgType)
	{
		case	DOCKYGET_Version:
				*msgData = DOCKYVERSION;
				break;

		case	DOCKYGET_GetSize:
				break;

		case	DOCKYGET_FrameDelay:
				break;

		case	DOCKYGET_RenderMode:
				break;

		case	DOCKYGET_Notifications:
				break;

		case	DOCKYGET_Name:
				break;

		case	DOCKYGET_DockyPrefs:
				break;

		case	DOCKYGET_InvisibleProcess:
				break;

		case	DOCKYGET_DockyPrefsChanged:
				break;

		case	DOCKYGET_Icon:
				break;

		case	DOCKYGET_ContextMenu:
				break;

		case	DOCKYGET_NeedsAttention:
				break;

		case	DOCKYGET_ShowsName:
				break;

		case	DOCKYGET_SizeAttribs:
				break;

		case	DOCKYGET_AllowsRename:
				break;

		case	DOCKYGET_SupportsComposite:
				break;

		case	DOCKYGET_AllowsIconChange:
				break;

		case	DOCKYGET_IconPath:
				break;

		case	DOCKYGET_CompositeMode:
				break;

		default:
				rc=FALSE;
				break;
	}

	return(rc);
};

/**/
BOOL  LCALL_DockySet(struct Interface *Self, uint32 msgType, uint32 msgData)
{
	BOOL	rc=TRUE;
	struct LIBRARY_CLASS *Base=(APTR)Self->Data.LibBase;

	KDEBUG("AMIdock<->Perception.Docky::Set(msgType=%lx, msgData=%lx)\n", msgType, msgData);

	switch(msgType)
	{
		case	DOCKYSET_RenderDestination:
				break;

		case	DOCKYSET_Window:
				break;

		case	DOCKYSET_Screen:
				break;

		case	DOCKYSET_DockyPrefs:
				break;

		case	DOCKYSET_DockyChange:
				break;

		case	DOCKYSET_DockyRemove:
				break;

		case	DOCKYSET_DockyRemoveExit:
				break;

		case	DOCKYSET_AmiDockFlashOnClickChange:
				break;

		case	DOCKYSET_AmiDockQuitsNow:
				break;

		case	DOCKYSET_RedrawNow:
				break;

		case	DOCKYSET_FileName:
				break;

		case	DOCKYSET_HotKey:
				break;

		case	DOCKYSET_DockyDebug:
				break;

		case	DOCKYSET_ContextMenuResult:
				break;

		case	DOCKYSET_DockyAttention:
				break;

		case	DOCKYSET_MouseMove:
				break;

		case	DOCKYSET_MouseIsOverDocky:
				break;

		case	DOCKYSET_SelectChange:
				break;

		case	DOCKYSET_SingleClick:
				break;

		case	DOCKYSET_DoubleClick:
				break;

		case	DOCKYSET_DNDMouseOver:
				break;

		case	DOCKYSET_DockyDND:
				break;

		case	DOCKYSET_ObjectDND:
				break;

		case	DOCKYSET_DockyVisibleChange:
				break;

		case	DOCKYSET_DockHide:
				break;

		case	DOCKYSET_DockyRawKey:
				break;

		case	DOCKYSET_DockRawKey:
				break;

		case	DOCKYSET_DockClick:
				break;

		case	DOCKYSET_ObjectClick:
				break;

		case	DOCKYSET_DockWinMove:
				break;

		case	DOCKYSET_DockResize:
				break;

		case	DOCKYSET_DockMinimizeChange:
				break;

		case	DOCKYSET_DockMinimizeableChange:
				break;

		case	DOCKYSET_DockCategoryChange:
				break;

		case	DOCKYSET_DockOrientationChange:
				break;

		case	DOCKYSET_DockSnapToChange:
				break;

		case	DOCKYSET_DockIconNameLinesChange:
				break;

		case	DOCKYSET_DockBarBackChange:
				break;

		case	DOCKYSET_DockIconBackChange:
				break;

		case	DOCKYSET_DockTypeChange:
				break;

		case	DOCKYSET_DockFontChange:
				break;

		case	DOCKYSET_DockScaleFactor:
				break;

		case	DOCKYSET_DockAdd:
				break;

		case	DOCKYSET_CategoryAdd:
				break;

		case	DOCKYSET_ObjectAdd:
				break;

		case	DOCKYSET_DockRemove:
				break;

		case	DOCKYSET_CategoryRemove:
				break;

		case	DOCKYSET_ObjectRemove:
				break;

		case	DOCKYSET_DockRename:
				break;

		case	DOCKYSET_CategoryRename:
				break;

		case	DOCKYSET_ObjectRename:
				break;

		case	DOCKYSET_DockMove:
				break;

		case	DOCKYSET_CategoryMove:
				break;

		case	DOCKYSET_ObjectMove:
				break;

		case	DOCKYSET_ObjectIconChange:
				break;

		default:
				rc=FALSE;
				break;
	};

	return(rc);
};

/**/
BOOL  LCALL_DockExec(struct Interface *Self, uint32 TurnCnt, uint32 msgType, uint32 msgData, BOOL Again)
{
	BOOL	rc=0L;

	KDEBUG("AMIdock<->Perception.Docky::Exec(msgType=%lx, msgData=%lx, TurnCnt=%lx, Again=%lx)\n", msgType, msgData, TurnCnt, Again);

	switch(msgType)
	{
		case	DOCKYPROCESS_DockyGetSelf:
				break;

		case	DOCKYPROCESS_DockyRemoveSelf:
				break;

		case	DOCKYPROCESS_DockyChangeName:
				break;

		case	DOCKYPROCESS_DockyChangeRendSize:
				break;

		case	DOCKYPROCESS_DockyGetPosSize:
				break;

		case	DOCKYPROCESS_DockyChangeFrameDelay:
				break;

		case	DOCKYPROCESS_QuitAmiDock:
				break;

		case	DOCKYPROCESS_IsAmiDockIconified:
				break;

		case	DOCKYPROCESS_IconifyAmiDock:
				break;

		case	DOCKYPROCESS_UniconifyAmiDock:
				break;

		case	DOCKYPROCESS_IgnoreNextObjectClick:
				break;

		case	DOCKYPROCESS_GetFlashOnClick:
				break;

		case	DOCKYPROCESS_SetFlashOnClick:
				break;

		case	DOCKYPROCESS_RunWB:
				break;

		case	DOCKYPROCESS_SetNeedsDebugMode:
				break;

		case	DOCKYPROCESS_GetDockCount:
				break;

		case	DOCKYPROCESS_GetDockInfo:
				break;

		case	DOCKYPROCESS_InsertDock:
				break;

		case	DOCKYPROCESS_RemoveDock:
				break;

		case	DOCKYPROCESS_MoveDock:
				break;

		case	DOCKYPROCESS_RenameDock:
				break;

		case	DOCKYPROCESS_GetDockHidden:
				break;

		case	DOCKYPROCESS_SetDockHidden:
				break;

		case	DOCKYPROCESS_SetDockHiddenAnim:
				break;

		case	DOCKYPROCESS_GetDockMinimized:
				break;

		case	DOCKYPROCESS_SetMinimizeDock:
				break;

		case	DOCKYPROCESS_GetDockMinimizeable:
				break;

		case	DOCKYPROCESS_SetDockMinimizeable:
				break;

		case	DOCKYPROCESS_GetDockOrientation:
				break;

		case	DOCKYPROCESS_SetDockOrientation:
				break;

		case	DOCKYPROCESS_GetDockSnapTo:
				break;

		case	DOCKYPROCESS_SetDockSnapTo:
				break;

		case	DOCKYPROCESS_GetDockIconNameLines:
				break;

		case	DOCKYPROCESS_SetDockIconNameLines:
				break;

		case	DOCKYPROCESS_GetDockBarBack:
				break;

		case	DOCKYPROCESS_SetDockBarBack:
				break;

		case	DOCKYPROCESS_GetDockIconBack:
				break;

		case	DOCKYPROCESS_SetDockIconBack:
				break;

		case	DOCKYPROCESS_GetDockOrigin:
				break;

		case	DOCKYPROCESS_SetDockOrigin:
				break;

		case	DOCKYPROCESS_GetDockPosition:
				break;

		case	DOCKYPROCESS_SetDockPosition:
				break;

		case	DOCKYPROCESS_GetDockSize:
				break;

		case	DOCKYPROCESS_SetDockSize:
				break;

		case	DOCKYPROCESS_ResizeDock:
				break;

		case	DOCKYPROCESS_RelayoutDock:
				break;

		case	DOCKYPROCESS_DockToFront:
				break;

		case	DOCKYPROCESS_DockToBack:
				break;

		case	DOCKYPROCESS_GetDockWindowPS:
				break;

		case	DOCKYPROCESS_GetDockDragBarPS:
				break;

		case	DOCKYPROCESS_GetCategoryCount:
				break;

		case	DOCKYPROCESS_GetCategoryInfo:
				break;

		case	DOCKYPROCESS_InsertCategory:
				break;

		case	DOCKYPROCESS_RemoveCategory:
				break;

		case	DOCKYPROCESS_MoveCategory:
				break;

		case	DOCKYPROCESS_RenameCategory:
				break;

		case	DOCKYPROCESS_GetObjectCount:
				break;

		case	DOCKYPROCESS_GetObjectInfo:
				break;

		case	DOCKYPROCESS_InsertObject:
				break;

		case	DOCKYPROCESS_RemoveObject:
				break;

		case	DOCKYPROCESS_MoveObject:
				break;

		case	DOCKYPROCESS_GetObjectPS:
				break;

		default:
			break;
	}

	return(rc);
};

/**/
