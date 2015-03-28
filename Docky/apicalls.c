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
  (APTR)&LCALL_DockyExec,
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
		default:
				rc=FALSE;
				break;
	};

	return(rc);
};

/**/

BOOL  LCALL_DockyExec(struct Interface *Self, uint32 TurnCnt, uint32 msgType, uint32 msgData, BOOL Again)
{
	BOOL	rc=0L;

	KDEBUG("AMIdock<->Perception.Docky::Exec(msgType=%lx, msgData=%lx, TurnCnt=%lx, Again=%lx)\n", msgType, msgData, TurnCnt, Again);

	switch(msgType)
	{
		default:
			break;
	}

	return(rc);
};

/**/
