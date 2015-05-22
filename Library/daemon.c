/*
\\		"Perception"
//
\\		AmigaOS 4.x & AROS Input Method Editor Stack
*/
#include "perception.h"

/*  The Daemon is the Central Manager for the entire of Perception
*/
struct	DaemonApplication
{
	struct	LIBRARY_CLASS		*PerceptionBase;
//
	struct	ExecIFace			*IExec;
	struct  PerceptionIFace		*IPerception;
	struct  Interface			*INewLib;
	struct	DOSIFace			*IDOS;
	struct	CommoditiesIFace	*ICX;
	struct	UtilityIFace		*IUtility;
	struct	LocaleIFace			*ILocale;
	struct	RexxSysIFace		*IREXX;
	struct	ApplicationIFace	*IApplication;
//
	struct	MsgPort				*cxPort;
	ULONG						cxSignal;
	APTR						CommodityKey;
	UWORD						CommodityFlags;
//
	ULONG						hApplication;
	struct	ApplicationIconInfo *hApplicationInfo;
//
	APTR						hLocale;
	APTR						hLocaleDB;
//
	struct	MsgPort				*rxPort;
	ULONG						rxSignal;
//
	struct	MsgPort				*ioPort;
	ULONG						ioSignal;
	struct	Interrupt			*imFilter;
	struct  IOStdReq			*imRequest;
	LONG						imSigBit;
	ULONG						imSignal;
//
	struct  SignalSemaphore		InputLock;
	ULONG                       InputState[IME_STATE_SIZE];
	struct InputTagItem			InputVector[IME_VECTOR_SIZE];
};

void  InitCommodity(struct DaemonApplication *Self,LONG active);
void  ExitCommodity(struct DaemonApplication *Self);
void  InitApplication(struct DaemonApplication *Self);
void  ExitApplication(struct DaemonApplication *Self);
void  InitLocalization(struct DaemonApplication *Self);
void  ExitLocalization(struct DaemonApplication *Self);
void  InitRexxHost(struct DaemonApplication *Self);
void  ExitRexxHost(struct DaemonApplication *Self);
void  InitInputHandler(struct DaemonApplication *dapp);
void  ExitInputHandler(struct DaemonApplication *dapp);
void  MainEventHandler(struct DaemonApplication *dapp);
ULONG PerceptionCommodityEvent(struct DaemonApplication *Self, APTR message);
ULONG PerceptionRexxHostEvent(struct DaemonApplication *Self, APTR message);
APTR  ExecInputHandler(APTR stream,APTR data);
void  PerceptionInputContext(struct DaemonApplication *dapp);
void  ExecLanguagePluginEntry(struct DaemonApplication *dapp);
//
void  IsValidPluginEntryPoint(struct LanguageContext *c, struct DaemonApplication *d);


/*	Process Information
*/
STATIC CONST BYTE	DaemonName[]			= "Perception-IME\0";
STATIC CONST ULONG	DaemonStackSize			= 131072L;
STATIC CONST ULONG	DaemonPriority			= 96L;
STATIC CONST BYTE	DaemonDescription[]		= "Input Method Editing\0\0";
STATIC CONST BYTE	DaemonReleaseString[]	= "Open Source Edition\0";
STATIC CONST BYTE	DaemonConfiguration[]	= "Perception-IME\0";
STATIC CONST BYTE	DaemonEnvironment[]		= "Perception\0\0";
STATIC CONST BYTE   DaemonLocale[]			= "english";

/*	Commodity Flag markers
*/
#define PERCEPTION_STATE_ACTIVE     0x1000

/*  InitPerceptionDaemon() is called to launch the PerceptionDaemon Process
*/
void InitPerceptionDaemon(struct LIBRARY_CLASS *Self)
{
	if(Self->IDOS)
		if(Self->DaemonProcess==NULL)
			Self->DaemonProcess=(APTR)Self->IDOS->CreateNewProcTags(
				NP_Entry,		(APTR)&ExecPerceptionDaemon,
				NP_Input,		NULL,			NP_CloseInput,	NULL,
				NP_Output,		NULL,			NP_CloseOutput,	NULL,
				NP_Error,		NULL,			NP_CloseError,	NULL,
				NP_CurrentDir,	NULL,           NP_Name,		DaemonName,
				NP_CommandName,	DaemonName,		NP_Priority,	DaemonPriority,
				NP_StackSize,	DaemonStackSize,NP_ConsoleTask,	NULL,
				NP_WindowPtr,	NULL,			NP_CopyVars,	NULL,
				NP_Cli,			NULL,			NP_Path,		NULL,
				TAG_END,		NULL);
	Self->CurrentLanguage=NULL;
	return;
};

/**/
void ExitPerceptionDaemon(struct LIBRARY_CLASS *Self)
{
	return;
};

/**/
int32 ExecPerceptionDaemon(STRPTR argv, ULONG argc)
{
	uint32 rc=0L, i=0L;
    struct ExecIFace *IExec = (struct ExecIFace *)(*(struct ExecBase **)4)->MainInterface;
	struct Library *Base = NULL;
	struct DaemonApplication *dApplication=NULL;

	dApplication=IExec->AllocVecTags(sizeof(struct DaemonApplication),
		AVT_Type,MEMF_SHARED,AVT_ClearWithValue,0L,TAG_DONE);
	if(dApplication)
	{
		if((Base = (APTR)IExec->OpenLibrary(LIBRARY_NAME, 0L)))
			dApplication->PerceptionBase = (APTR)Base;
		dApplication->IExec			= (struct ExecIFace *)(*(struct ExecBase **)4)->MainInterface;
		if(dApplication->PerceptionBase)
			dApplication->IPerception = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

		if((Base = (APTR)IExec->OpenLibrary("newlib.library", 0L)))
			dApplication->INewLib	 = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

		if((Base = (APTR)IExec->OpenLibrary("dos.library", 50L)))
			dApplication->IDOS		 = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

		if((Base = (APTR)IExec->OpenLibrary("commodities.library", 50L)))
			dApplication->ICX		 = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

		if((Base = (APTR)IExec->OpenLibrary("utility.library", 50L)))
			dApplication->IUtility	 = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

		if((Base = (APTR)IExec->OpenLibrary("application.library", 50L)))
			dApplication->IApplication = (APTR)IExec->GetInterface(Base,"main",2L,NULL);

		if((Base = (APTR)IExec->OpenLibrary("locale.library", 50L)))
			dApplication->ILocale	 = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

		if((Base = (APTR)IExec->OpenLibrary("rexxsyslib.library", 0L)))
			dApplication->IREXX		 = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

		if(dApplication->ICX)
			InitCommodity(dApplication,TRUE);

		if(dApplication->IApplication)
			InitApplication(dApplication);

		if(dApplication->ILocale)
			InitLocalization(dApplication);

		if(dApplication->IREXX)
			InitRexxHost(dApplication);

		IExec->InitSemaphore(&dApplication->InputLock);

		for(i=0L;i<IME_STATE_SIZE;i++)
			dApplication->InputState[i]=0L;
		for(i=0L;i<IME_VECTOR_SIZE;i++)
		{
			dApplication->InputVector[i].type=0;
			dApplication->InputVector[i].qual=0;
			dApplication->InputVector[i].glyph=0;
		}
		InitInputHandler(dApplication);

		MainEventHandler(dApplication);

		ExitInputHandler(dApplication);

		ExitRexxHost(dApplication);
		ExitLocalization(dApplication);
		ExitApplication(dApplication);
		ExitCommodity(dApplication);

		if(dApplication->IREXX)
		{
			Base = dApplication->IREXX->Data.LibBase;
			IExec->DropInterface((APTR)dApplication->IREXX);
			IExec->CloseLibrary((APTR)Base);
		}

		if(dApplication->ILocale)
		{
			Base = dApplication->ILocale->Data.LibBase;
			IExec->DropInterface((APTR)dApplication->ILocale);
			IExec->CloseLibrary((APTR)Base);
		}

		if(dApplication->IApplication)
		{
			Base = dApplication->IApplication->Data.LibBase;
			IExec->DropInterface((APTR)dApplication->IApplication);
			IExec->CloseLibrary((APTR)Base);
		}

		if(dApplication->IUtility)
		{
			Base = dApplication->IUtility->Data.LibBase;
			IExec->DropInterface((APTR)dApplication->IUtility);
			IExec->CloseLibrary((APTR)Base);
		}

		if(dApplication->ICX)
		{
			Base = dApplication->ICX->Data.LibBase;
			IExec->DropInterface((APTR)dApplication->ICX);
			IExec->CloseLibrary((APTR)Base);
		}

		if(dApplication->IDOS)
		{
			Base = dApplication->IDOS->Data.LibBase;
			IExec->DropInterface((APTR)dApplication->IDOS);
			IExec->CloseLibrary((APTR)Base);
		}

		if(dApplication->INewLib)
		{
			Base = dApplication->INewLib->Data.LibBase;
			IExec->DropInterface((APTR)dApplication->INewLib);
			IExec->CloseLibrary((APTR)Base);
		}

		if(dApplication->IPerception)
			IExec->DropInterface((APTR)dApplication->IPerception);
		if(dApplication->PerceptionBase)
			IExec->CloseLibrary((APTR)dApplication->PerceptionBase);

        IExec->FreeVec(dApplication);
	}
	return(rc);
};

/**/
void InitCommodity(struct DaemonApplication *Self,LONG active)
{
	LONG	error=0L;
	struct NewBroker NewCommodityDaemon;

	NewCommodityDaemon.nb_Version			= NB_VERSION;
	NewCommodityDaemon.nb_Name				= (APTR)DaemonName;
	NewCommodityDaemon.nb_Title				= (APTR)DaemonName;
	NewCommodityDaemon.nb_Descr				= (APTR)DaemonDescription;
	NewCommodityDaemon.nb_Unique			= 0;
	NewCommodityDaemon.nb_Flags				= 0;
	NewCommodityDaemon.nb_Pri				= 0;
	NewCommodityDaemon.nb_ReservedChannel	= 0;

	if(Self->IExec)
		Self->cxPort =	(APTR)Self->IExec->AllocSysObjectTags( ASOT_PORT,
			ASOPORT_Size,		sizeof(struct MsgPort),
			ASOPORT_Pri,		0L,
			ASOPORT_Name,		DaemonName,
			ASOPORT_Public,		FALSE,
			TAG_END,			NULL);
	if(Self->cxPort)
		Self->cxSignal=1L << Self->cxPort->mp_SigBit;
	if(Self->cxSignal)
		NewCommodityDaemon.nb_Port = Self->cxPort;
	Self->CommodityKey=Self->ICX->CxBroker(&NewCommodityDaemon,&error);
	if(Self->CommodityKey)
		Self->ICX->ActivateCxObj(Self->CommodityKey, active);
	if(active)
		Self->CommodityFlags = Self->CommodityFlags | PERCEPTION_STATE_ACTIVE;
	return;
}

/**/
void ExitCommodity(struct DaemonApplication *Self)
{
	if(Self->CommodityKey)
	{
		Self->ICX->ActivateCxObj(Self->CommodityKey,TRUE);
		Self->ICX->DeleteCxObjAll(Self->CommodityKey);
		Self->CommodityKey=NULL;
	}
	return;
}

/**/
void  InitApplication(struct DaemonApplication *Self)
{
	struct ApplicationIconInfo *info=NULL;
    info=Self->IExec->AllocVecTags(sizeof(struct ApplicationIconInfo),
		AVT_Type,			MEMF_SHARED,
		AVT_ClearWithValue,	0L,
		TAG_DONE,			0L);
	if(info)
	{
		info->iconType=APPICONT_Docky;
		Self->hApplicationInfo=info;
	}

	Self->hApplication=Self->IApplication->RegisterApplication((APTR)DaemonName,
		REGAPP_UniqueApplication,	TRUE,
		REGAPP_LoadPrefs,			TRUE,
		REGAPP_SavePrefs,			TRUE,
		REGAPP_CustomPrefsFileName,	DaemonConfiguration,
		REGAPP_ENVDir,				DaemonEnvironment,
		REGAPP_Hidden,				FALSE,
		REGAPP_AppIconInfo,			info,
		REGAPP_Description,			DaemonDescription,
		TAG_END,					NULL);
}

/**/
void  ExitApplication(struct DaemonApplication *Self)
{
	Self->IApplication->UnregisterApplication(Self->hApplication,0L);
}

/**/
void  InitLocalization(struct DaemonApplication *Self)
{
	Self->hLocale	= Self->ILocale->OpenLocale(NULL);
	if(Self->hLocale)
		Self->hLocaleDB	= Self->ILocale->OpenCatalog(Self->hLocale, "Perception",
			OC_BuiltInLanguage, (LONG)DaemonLocale,
			OC_BuiltInCodeSet,	106L,
			OC_PreferExternal,	TRUE,
			TAG_END,			NULL);
	return;
}

/**/
void  ExitLocalization(struct DaemonApplication *Self)
{
	if(Self->hLocaleDB)
		Self->ILocale->CloseCatalog(Self->hLocaleDB);
	if(Self->hLocale)
		Self->ILocale->CloseLocale(Self->hLocale);
	return;
}

/**/
void  InitRexxHost(struct DaemonApplication *Self)
{
	if(Self->IExec)
		Self->rxPort =	(APTR)Self->IExec->AllocSysObjectTags( ASOT_PORT,
			ASOPORT_Size,		sizeof(struct MsgPort),
			ASOPORT_Pri,		0L,
			ASOPORT_Name,		DaemonName,
			ASOPORT_Public,		TRUE,
			TAG_END,			NULL);
	if(Self->rxPort)
		Self->rxSignal=1L << Self->rxPort->mp_SigBit;
	return;
}

/**/
void  ExitRexxHost(struct DaemonApplication *Self)
{
	if(Self->rxPort)
    	Self->IExec->FreeSysObject( ASOT_PORT, Self->rxPort );
	return;
}

/**/
void  InitInputHandler(struct DaemonApplication *Self)
{
	uint32	ioError=0L;

	Self->ioPort				= (APTR)Self->IExec->AllocSysObjectTags( ASOT_PORT,
			ASOPORT_Size,		sizeof(struct MsgPort),
			ASOPORT_Pri,		0L,
			ASOPORT_Name,		DaemonName,
			ASOPORT_Public,		FALSE,
			TAG_END,			NULL);
	if(Self->ioPort)
		Self->imFilter			= (APTR)Self->IExec->AllocSysObjectTags( ASOT_INTERRUPT,
			ASOINTR_Size,		sizeof(struct Interrupt),
			ASOINTR_Code,		(APTR)&ExecInputHandler,
			ASOINTR_Data,		Self,
			NULL,				NULL);
	if(Self->ioPort)
		Self->ioSignal=1L << Self->ioPort->mp_SigBit;
	if(Self->ioSignal)
		Self->imSignal=SIGBREAKF_CTRL_E;
	if(Self->imFilter)
		Self->imRequest			= (APTR)Self->IExec->AllocSysObjectTags( ASOT_IOREQUEST,
			ASOIOR_Size,		sizeof(struct IOStdReq),
			ASOIOR_ReplyPort,	Self->ioPort,
			NULL,				NULL);
	if(Self->imRequest)
	{
		Self->imFilter->is_Node.ln_Pri	= 52L; // Follow Commodities, Precede Intuition
		Self->imFilter->is_Node.ln_Name	= Self->PerceptionBase->Library.lib_Node.ln_Name;
//		Self->imFilter->is_Data			= (APTR)Self;
//		Self->imFilter->is_Code			= (APTR)&ExecInputHandler;
		Self->imRequest->io_Data		= (APTR)Self->imFilter;
		Self->imRequest->io_Command		= IND_ADDHANDLER;

		ioError = Self->IExec->OpenDevice("input.device",0L,(APTR)Self->imRequest,0L);
    }

	if(ioError)
	{
		KDEBUG("Perception-IME::InitInputHandler() [%lx] FATAL: Internal Error\n", ioError);
	}else{
		Self->IExec->DoIO((APTR)Self->imRequest);
	};

	return;
}

/**/
void  ExitInputHandler(struct DaemonApplication *Self)
{
	if(Self->imRequest)
	{
		Self->imRequest->io_Command	= IND_REMHANDLER;
		Self->IExec->DoIO((APTR)Self->imRequest);
		Self->IDOS->Delay(1);
	    Self->IExec->FreeSysObject( ASOT_IOREQUEST, Self->imRequest );
	}
	if(Self->imFilter)
	    Self->IExec->FreeSysObject( ASOT_INTERRUPT, Self->imFilter );
	if(Self->ioPort)
    	Self->IExec->FreeSysObject( ASOT_PORT, Self->ioPort );

	return;
};

void  MainEventHandler(struct DaemonApplication *dapp)
{
	ULONG sigmask=0L, signals=0L, exit=0L,
		cxSignal=dapp->cxSignal,
		ioSignal=dapp->ioSignal,
		rxSignal=dapp->rxSignal;
	APTR	message = NULL;

	do{
		sigmask = ioSignal | rxSignal | cxSignal;
		signals = dapp->IExec->Wait(sigmask);

		if(signals & cxSignal)
			while((message=(APTR)dapp->IExec->GetMsg(dapp->cxPort)))
				exit=PerceptionCommodityEvent(dapp,message);
		if(signals & rxSignal)
			while((message=(APTR)dapp->IExec->GetMsg(dapp->rxPort)))
				exit=PerceptionRexxHostEvent(dapp,message);
		if(signals & ioSignal)
			ExecLanguagePluginEntry(dapp);
		if(exit)
			sigmask=0L;
	}while(sigmask);
}

/*
//	Commodities Event Handling

	Disable	-> Stop Input Editing
	Enable	-> Start Input Editing
	Kill	-> Flush Everything
*/
ULONG PerceptionCommodityEvent(struct DaemonApplication *Self, APTR message)
{
	ULONG rc=0L;
	if((Self->ICX->CxMsgType(message))==CXM_COMMAND)
		switch(Self->ICX->CxMsgID(message))
		{
			case	CXCMD_DISABLE:
				Self->CommodityFlags = Self->CommodityFlags & (!PERCEPTION_STATE_ACTIVE);
				break;
			case	CXCMD_ENABLE:
				Self->CommodityFlags = Self->CommodityFlags | PERCEPTION_STATE_ACTIVE;
				break;
			case	CXCMD_APPEAR:   	/* External Forwarded? */
				break;
			case	CXCMD_DISAPPEAR:	/* External Forwarded? */
				break;
			case	CXCMD_KILL:			/* External then Internal */
				rc=TRUE;
				break;
			case	CXCMD_UNIQUE:		/* Internal Special */
				break;
			default:
				break;
		}
	return(rc);
}

/**/
ULONG PerceptionRexxHostEvent(struct DaemonApplication *Self, APTR message)
{
	ULONG rc=0L;

	KDEBUG("Perception-IME[Daemon]::PerceptionRexxHost();\n");

	return(rc);
}

/*	EntryPoint:Input.Device	- This needs to be refactored
*/
APTR  ExecInputHandler(APTR stream,APTR data)
{
	APTR rc=stream; ULONG bInputItem=0L, bMapKey=0L; USHORT l=4;
    struct InputEvent *cInputEvent=stream, *nInputEvent=NULL;
	struct DaemonApplication *dapp=data;
	struct LIBRARY_CLASS *Self=dapp->PerceptionBase;
	struct InputTagItem *pInputItem=NULL;

	do{
		nInputEvent=cInputEvent->ie_NextEvent;
		switch(cInputEvent->ie_Class)
		{
			case IECLASS_RAWKEY:
			case IECLASS_EXTENDEDRAWKEY:
				Self->IExec->ObtainSemaphore(&dapp->InputLock);
				bInputItem=dapp->InputState[ICSTATE_FIFO_IVW];
				pInputItem=(APTR)dapp->InputState[ICSTATE_FIFO_PVW];
				if(pInputItem==NULL)
					pInputItem=&dapp->InputVector;
			    if(dapp->CommodityFlags && PERCEPTION_STATE_ACTIVE)
				{
					if(Self->IKeymap->MapRawKey((APTR)cInputEvent,(APTR)&bMapKey,l,NULL))
					{
						pInputItem->type=TRANSLATE_ANSI;
					}else{
						pInputItem->type=TRANSLATE_AMIGA;
					};
					pInputItem->glyph=bMapKey;
					pInputItem->qual=cInputEvent->ie_Qualifier;
					KDEBUG("input.device::Perception-IME//type=%lx,qual=%lx,glyph=%lx [Item=%lx]\n",
						pInputItem->type, pInputItem->qual, pInputItem->glyph, pInputItem);
					if(bInputItem<IME_VECTOR_SIZE)
					{
						bInputItem++;pInputItem++;
					}else{
						bInputItem=0L;pInputItem=NULL;
					};
					pInputItem->type=0;
					pInputItem->qual=0;
					pInputItem->glyph=0L;
					dapp->InputState[ICSTATE_FIFO_IVW]=bInputItem;
					dapp->InputState[ICSTATE_FIFO_PVW]=(ULONG)pInputItem;
				};
				if(Self->DaemonProcess)
					Self->IExec->Signal(Self->DaemonProcess,dapp->ioSignal);
				Self->IExec->ReleaseSemaphore(&dapp->InputLock);
				break;
			default:
				rc=stream;
				break;
		};
		cInputEvent=nInputEvent;
	}while(cInputEvent);

	return(rc);
}

/*	LanguageContext style
*/
void  PerceptionInputContext(struct DaemonApplication *dapp)
{
	ULONG x=0L;

	dapp->IExec->InitSemaphore(&dapp->InputLock);
	for(x=0L;x<IME_STATE_SIZE;x++)
    {
		dapp->InputState[x] = 0;
	}
	for(x=0L;x<IME_VECTOR_SIZE;x++)
    {
		dapp->InputVector[x].type=0;
		dapp->InputVector[x].qual=0;
		dapp->InputVector[x].glyph=0;
	}
}

/*  LanguageContextHook Processing of InputTagItems and Isolation
*/
void  ExecLanguagePluginEntry(struct DaemonApplication *dapp)
{
	struct LIBRARY_CLASS *Self=dapp->PerceptionBase;
	struct LanguageContext *cLanguage=NULL, *nLanguage=NULL;
	struct InputTagItem *pInputItem=NULL;
	ULONG  bInputItem=0L, type=0L, qual=0, glyph=0L, Message[IME_MESSAGE_SIZE];

	Self->IExec->ObtainSemaphore(&dapp->InputLock);
	bInputItem=dapp->InputState[ICSTATE_FIFO_IVR];
	pInputItem=(APTR)dapp->InputState[ICSTATE_FIFO_PVR];
	if(pInputItem==NULL)
		pInputItem=&dapp->InputVector;
//
	type=pInputItem->type;
	glyph=pInputItem->glyph;
	qual=pInputItem->qual;
//
	KDEBUG("Perception-IME[DAEMON] ExecLanguagePluginEntry @%lx[type=%lx,qual=%lx,glyph=%lx]\n",
		pInputItem,type,qual,glyph);
//
	Message[7]=0L;
	Message[6]=0L;
	Message[5]=0L;
	Message[4]=0L;
	Message[3]=0L;
	Message[2]=qual;
	Message[1]=glyph;
	Message[0]=type;

    if(dapp->CommodityFlags && PERCEPTION_STATE_ACTIVE)
	{
		if(bInputItem<IME_VECTOR_SIZE)
		{
			bInputItem++;pInputItem++;
		}else{
			bInputItem=0L;pInputItem=NULL;
		};
		dapp->InputState[ICSTATE_FIFO_IVR]=bInputItem;
		dapp->InputState[ICSTATE_FIFO_PVR]=(ULONG)pInputItem;
//
		cLanguage=(APTR)Self->LanguageContextList.lh_Head;
		do{
			nLanguage=(APTR)cLanguage->Hook.h_MinNode.mln_Succ;
			cLanguage->IPerception=dapp->IPerception;
			cLanguage->IUtility=dapp->IUtility;
			if(dapp->IExec->IsNative(cLanguage->Hook.h_Entry))
				dapp->IUtility->CallHookPkt((APTR)cLanguage,(APTR)cLanguage,(APTR)Message);
			cLanguage=nLanguage;
		}while(cLanguage);
	};
	Self->IExec->ReleaseSemaphore(&dapp->InputLock);

	return;
}

/**/
