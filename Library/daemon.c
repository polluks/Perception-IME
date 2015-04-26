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
/**/
	struct	ExecIFace			*IExec;
	struct  PerceptionIFace		*IPerception;
	struct	DOSIFace			*IDOS;
	struct	CommoditiesIFace	*ICX;
	struct	UtilityIFace		*IUtility;
	struct	LocaleIFace			*ILocale;
	struct	RexxSysIFace		*IREXX;
	struct	ApplicationIFace	*IApplication;
/**/
	uint32	ApplicationID;
/**/
	struct	MsgPort				*cxPort;
	ULONG						cxSignal;
	APTR						CommodityKey;
	UWORD						CommodityFlags;
/**/
	APTR						hLocale;
	APTR						hDataBase;
/**/
	struct	MsgPort				*ioPort;
	ULONG						ioSignal;
	struct	Interrupt			*imFilter;
	struct  IOStdReq			*imRequest;
	LONG						imSigBit;
	ULONG						imSignal;
/**/
	struct	MsgPort				*rxPort;
	ULONG						rxSignal;
/**/
	struct  InputContext		DaemonContext;
/**/
};

void  InitCommodity(struct DaemonApplication *Self);
void  ExitCommodity(struct DaemonApplication *Self);
void  PerceptionCommodityEvent(struct DaemonApplication *Self, APTR message);
void  PerceptionRexxHostEvent(struct DaemonApplication *Self, APTR message);
void  InitInputHandler(struct DaemonApplication *dapp);
void  ExitInputHandler(struct DaemonApplication *dapp);
void  ProcInputHandler(struct DaemonApplication *hDaemon);
APTR  ExecInputHandler(struct DaemonApplication *hDaemon,APTR ieStream);
void  InputForward(struct TagItem *EGlyph,struct DaemonApplication *hDaemon);

/*
 *	Process Information
*/
STATIC CONST BYTE	DaemonName[]			= "Perception-IME\0";
STATIC CONST ULONG	DaemonStackSize			= 98304UL;
STATIC CONST ULONG	DaemonPriority			= 96UL;
STATIC CONST BYTE	DaemonDescription[]		= "Input Method Editing\0";
STATIC CONST BYTE	DaemonReleaseString[]	= "Open Source Edition\0";

STATIC CONST BYTE	EnvironmentSettings[]	= "Env:Perception/Environment\0";
STATIC CONST BYTE	EnvironmentStorage[]	= "Env:Perception/\0";

/*
 *	Commodity Flag markers
*/
#define PERCEPTION_STATE_ACTIVE     0x1000

/*
 *	Internal Default Language Processing
*/
#define	LCSTATE_CHORDBUFF		(LCSTATE_EXPANDED+0)

/*
		InitPerceptionDaemon() is called to launch the PerceptionDaemon Process
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
	return;
};

void ExitPerceptionDaemon(struct LIBRARY_CLASS *Self)
{
	return;
};

int32 ExecPerceptionDaemon(STRPTR argv, ULONG argc)
{
	uint32	rc=0L;
    struct ExecIFace *IExec = (struct ExecIFace *)(*(struct ExecBase **)4)->MainInterface;
	struct Library *Base = NULL;
	struct DaemonApplication *dApplication=NULL;

	ULONG	signals = 0L, sigmask = 0L;
	APTR	message = NULL;

	dApplication=IExec->AllocVecTags(sizeof(struct DaemonApplication),
		AVT_Type,MEMF_SHARED,AVT_ClearWithValue,0L,TAG_DONE);
	if(dApplication)
	{
		dApplication->IExec			= (struct ExecIFace *)(*(struct ExecBase **)4)->MainInterface;
		if((Base = (APTR)IExec->OpenLibrary(LIBRARY_NAME, 0L)))
			dApplication->IPerception = (APTR)IExec->GetInterface(Base,"main",1L,NULL);
		dApplication->PerceptionBase = (APTR)Base;

		if((Base = (APTR)IExec->OpenLibrary("dos.library", 50L)))
			dApplication->IDOS		 = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

		if((Base = (APTR)IExec->OpenLibrary("commodities.library", 50L)))
			dApplication->ICX		 = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

		if((Base = (APTR)IExec->OpenLibrary("utility.library", 50L)))
			dApplication->IUtility	 = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

		if((Base = (APTR)IExec->OpenLibrary("locale.library", 50L)))
			dApplication->ILocale	 = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

		if((Base = (APTR)IExec->OpenLibrary("rexxsyslib.library", 0L)))
			dApplication->IREXX		 = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

		if((Base = (APTR)IExec->OpenLibrary("application.library", 50L)))
			dApplication->IApplication = (APTR)IExec->GetInterface(Base,"main",2L,NULL);

		if(dApplication->ICX)
			InitCommodity(dApplication);

		if(dApplication->IApplication)
			dApplication->ApplicationID=dApplication->IApplication->RegisterApplication((APTR)DaemonName,
				REGAPP_UniqueApplication,		TRUE,
				REGAPP_Hidden,					TRUE,
				REGAPP_CustomPrefsFileName,		EnvironmentSettings,
				REGAPP_ENVDir,					EnvironmentStorage,
				REGAPP_AppIconInfo,				NULL,
				REGAPP_Description,				DaemonDescription,
				NULL,							NULL);

		InitInputContext(&dApplication->DaemonContext,NULL);
		dApplication->DaemonContext.Hook.PerceptionLib=(APTR)dApplication->IPerception;
		dApplication->DaemonContext.Hook.UtilityLib=(APTR)dApplication->IUtility;
        dApplication->PerceptionBase->InputContext=&dApplication->DaemonContext;

		InitInputHandler(dApplication);

		do{
			sigmask = dApplication->ioSignal | dApplication->cxSignal | dApplication->rxSignal;
			signals = IExec->Wait(sigmask);

			if(signals && dApplication->ioSignal)
				ProcInputHandler(dApplication);

			if(signals && dApplication->cxSignal)
				while((message=(APTR)IExec->GetMsg(dApplication->cxPort)))
					PerceptionCommodityEvent(dApplication,message);

			if(signals && dApplication->cxSignal)
				while((message=(APTR)IExec->GetMsg(dApplication->rxPort)))
					PerceptionRexxHostEvent(dApplication,message);

		}while(dApplication != NULL);

		ExitInputHandler(dApplication);
		ExitInputContext(&dApplication->DaemonContext);

		if(dApplication->ApplicationID)
			dApplication->IApplication->UnregisterApplication(dApplication->ApplicationID, NULL);

		if(dApplication->cxPort)
			ExitCommodity(dApplication);

		if(dApplication->IApplication)
		{
			Base = dApplication->IApplication->Data.LibBase;
			IExec->DropInterface((APTR)dApplication->IApplication);
			IExec->CloseLibrary((APTR)Base);
		}

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

		if(dApplication->IPerception)
			IExec->DropInterface((APTR)dApplication->IPerception);
		if(dApplication->PerceptionBase)
			IExec->CloseLibrary((APTR)dApplication->PerceptionBase);

        IExec->FreeVec(dApplication);
	}
	return(rc);
};

void InitCommodity(struct DaemonApplication *Self)
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
			NULL,				NULL);
	if(Self->cxPort)
		Self->cxSignal=1L << Self->cxPort->mp_SigBit;
	if(Self->cxSignal)
		NewCommodityDaemon.nb_Port = Self->cxPort;
	Self->CommodityKey=Self->ICX->CxBroker(&NewCommodityDaemon,&error);
	if(Self->CommodityKey)
    {
		Self->ICX->ActivateCxObj(Self->CommodityKey, TRUE);
		Self->CommodityFlags = Self->CommodityFlags | PERCEPTION_STATE_ACTIVE;
	}
	return;
}

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
/*
//	Commodities Event Handling

	Disable	-> Stop Input Editing
	Enable	-> Start Input Editing
	Kill	-> Flush Everything
*/
void PerceptionCommodityEvent(struct DaemonApplication *Self, APTR message)
{
	KDEBUG("Perception[Commodity]");
	if((Self->ICX->CxMsgType(message))==CXM_COMMAND)
		switch(Self->ICX->CxMsgID(message))
		{
			case	CXCMD_DISABLE:
				Self->CommodityFlags = Self->CommodityFlags & (!PERCEPTION_STATE_ACTIVE);
				KDEBUG("::Disable(%x)\n",Self->CommodityFlags);	break;
				break;
			case	CXCMD_ENABLE:
				Self->CommodityFlags = Self->CommodityFlags | PERCEPTION_STATE_ACTIVE;
				KDEBUG("::Enable(%x)\n",Self->CommodityFlags);	break;
				break;
			case	CXCMD_APPEAR:   	/* External Forwarded */
				KDEBUG("::Appear()\n");	break;
				break;
			case	CXCMD_DISAPPEAR:	/* External Forwarded */
				KDEBUG("::Disappear()\n");	break;
				break;
			case	CXCMD_KILL:			/* External then Internal */
				KDEBUG("::Kill()\n");		break;
				break;
			case	CXCMD_UNIQUE:		/* Internal Special */
				KDEBUG("::Unique()\n");	break;
				break;
			default:
				KDEBUG("Perception[Commodity::Unknown()]\n");	break;
				break;
		}
	return;
}

/*
*/
void  PerceptionRexxHostEvent(struct DaemonApplication *Self, APTR message)
{

	return;
}

/*
*/
void  InitInputHandler(struct DaemonApplication *Self)
{
	uint32	ioError=0L;

	Self->ioPort				= (APTR)Self->IExec->AllocSysObjectTags( ASOT_PORT,
			ASOPORT_Size,		sizeof(struct MsgPort),
			ASOPORT_Pri,		0L,
			ASOPORT_Name,		Self->PerceptionBase->Library.lib_Node.ln_Name,
			NULL,				NULL);

	if(Self->ioPort)
		Self->imFilter			= (APTR)Self->IExec->AllocSysObjectTags( ASOT_INTERRUPT,
			ASOINTR_Size,		sizeof(struct Interrupt),
			ASOINTR_Code,		(ULONG)&ExecInputHandler,
			ASOINTR_Data,		0L,
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
		Self->imFilter->is_Node.ln_Pri	= 51L; /* Immediately Before Intuition */
		Self->imFilter->is_Node.ln_Name	= Self->PerceptionBase->Library.lib_Node.ln_Name;
		Self->imFilter->is_Data			= (APTR)Self;
		Self->imFilter->is_Code			= (APTR)&ExecInputHandler;

		Self->imRequest->io_Data		= (APTR)Self->imFilter;
		Self->imRequest->io_Command		= IND_ADDHANDLER;

		ioError = Self->IExec->OpenDevice("input.device",0L,(APTR)Self->imRequest,0L);
    }

	if(ioError)
	{
		KDEBUG("Perception-IME::InitInputHandler() [%lx] FATAL: Internal Error", ioError);
	}else{
		Self->IExec->DoIO((APTR)Self->imRequest);
	};

	return;
}

/*
*/
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

void  ProcInputHandler(struct DaemonApplication *hDaemon)
{
	ULONG xc=0L, InputIndex=0L, OutputIndex=0L, OutputMask=0L;
	struct LIBRARY_CLASS *Self=hDaemon->PerceptionBase;
	struct InputContext *Context=Self->InputContext, *Language=Self->CurrentLanguage;
	struct InputTagItem *bInputItem=NULL;
	struct TagItem *Message=NULL, *Vector=NULL, *EmitGlyph=NULL;

	if(hDaemon->CommodityFlags && PERCEPTION_STATE_ACTIVE)
	{
		KDEBUG("Perception[Commodity//ProcInputHandler]()\n");
		if(Context)
		{
			InputIndex=Context->State[ICSTATE_FIFO_IR];
			bInputItem=(APTR)Context->State[ICSTATE_FIFO_PR];
			if(!bInputItem)
				bInputItem=Context->Vector;
			Self->IExec->ObtainSemaphore(&Self->Lock);
			if(Language)
            {
				Language->Hook.Hook.h_Data=(APTR)Language;
				Language->Hook.PerceptionLib=hDaemon->IPerception;
				Language->Hook.UtilityLib=hDaemon->IUtility;
		        Message=(struct TagItem *)Language->Message;
				Vector=(struct TagItem *)Language->Vector;
			};
			switch(bInputItem->type)
			{
				case TRANSLATE_ANSI:
					Message[0].ti_Tag	= LANGUAGE_TRANSLATE_ANSI;
					Message[0].ti_Data	= bInputItem->glyph;
					Message[1].ti_Tag	= LANGUAGE_TRANSLATE_ANSI_QUAL;
					Message[1].ti_Data	= bInputItem->qual;
					Message[2].ti_Tag	= 0L;
					Message[2].ti_Data	= 0L;
					Message[3].ti_Tag	= 0L;
					Message[3].ti_Data	= 0L;
					break;
				case TRANSLATE_AMIGA:
					Message[0].ti_Tag	= LANGUAGE_TRANSLATE_AMIGA;
					Message[0].ti_Data	= bInputItem->glyph;
					Message[1].ti_Tag	= LANGUAGE_TRANSLATE_AMIGA_QUAL;
					Message[1].ti_Data	= bInputItem->qual;
					Message[2].ti_Tag	= 0L;
					Message[2].ti_Data	= 0L;
					Message[3].ti_Tag	= 0L;
					Message[3].ti_Data	= 0L;
					break;
				default:
					break;
			};
			if(InputIndex<IME_VECTOR_SIZE)
			{
				InputIndex++; bInputItem++;
			}else{
				InputIndex=0L; bInputItem=NULL;
			};
			Context->State[ICSTATE_FIFO_IR]=InputIndex;
			Context->State[ICSTATE_FIFO_PR]=(ULONG)bInputItem;
			Self->IExec->ReleaseSemaphore(&Self->Lock);
			KDEBUG("ProcInputHandler()[Hook::PreEntry]\n");
			if(Language->Hook.Hook.h_Entry)
	        {
				xc=hDaemon->IUtility->CallHookPkt((APTR)&Language->Hook,(APTR)Language,(APTR)Message);
			}else{
				xc=hDaemon->IUtility->CallHookPkt((APTR)&Context->Hook,(APTR)Language,(APTR)Message);
			};
			for(OutputIndex=LCSTATE_EMITBUFF;OutputIndex<LCSTATE_EMITBMAX;OutputIndex++)
			{
				EmitGlyph=(APTR)hDaemon->IUtility->FindTagItem(OutputIndex,Vector);
				if(EmitGlyph->ti_Data)
				{
					OutputMask = OutputMask || 1L << (OutputIndex-LCSTATE_EMITBUFF);
					TranslateCP32UTF8((APTR)&EmitGlyph->ti_Data);
					KDEBUG("[UTF8=%lx]\n",EmitGlyph->ti_Data);
					InputForward(EmitGlyph,hDaemon);
				}
				EmitGlyph->ti_Data=0L;
			};
		};
	};

	return;
}

APTR  ExecInputHandler(struct DaemonApplication *hDaemon,APTR ieStream)
{
	ULONG InputIndex=0L, buffer=0L;
	struct InputEvent *rc=ieStream, *prev=ieStream, *hInputEvent=ieStream;
	struct LIBRARY_CLASS *Self=hDaemon->PerceptionBase;
	struct InputContext *Context=NULL;
	struct InputTagItem *bInputItem=NULL;

	if(hDaemon->CommodityFlags && PERCEPTION_STATE_ACTIVE)
		do{
			if(hInputEvent->ie_Class==IECLASS_RAWKEY)
				Context=Self->InputContext;
			if(hInputEvent->ie_Class==IECLASS_EXTENDEDRAWKEY)
				Context=Self->InputContext;
			if(Context)
			{
				InputIndex=Context->State[ICSTATE_FIFO_IW];
				bInputItem=(APTR)Context->State[ICSTATE_FIFO_PW];
				if(!bInputItem)
					bInputItem=Context->Vector;
				Self->IExec->ObtainSemaphore(&Self->Lock);
				if(Self->IKeymap->MapRawKey(hInputEvent,(APTR)&buffer,4,NULL))
				{
					bInputItem->glyph	= buffer;
					bInputItem->qual	= hInputEvent->ie_Qualifier;
					bInputItem->type	= TRANSLATE_ANSI;
				}else{
					bInputItem->glyph	= hInputEvent->ie_Code;
					bInputItem->qual	= hInputEvent->ie_Qualifier;
					bInputItem->type	= TRANSLATE_AMIGA;
				};
				if(InputIndex<IME_VECTOR_SIZE)
				{
					InputIndex++; bInputItem++;
				}else{
					InputIndex=0L; bInputItem=NULL;
				};
				Context->State[ICSTATE_FIFO_IW]=InputIndex;
				Context->State[ICSTATE_FIFO_PW]=(ULONG)bInputItem;
				Self->IExec->ReleaseSemaphore(&Self->Lock);
				Self->IExec->Signal(Self->IExec->FindTask(NULL),hDaemon->ioSignal);
			}
			prev=hInputEvent;
		}while((hInputEvent=hInputEvent->ie_NextEvent)!=NULL);
	return((APTR)rc);
}

//	Refeed Octets through input.device with ignore marker for the filter ?

void  InputForward(struct TagItem *EGlyph,struct DaemonApplication *hDaemon)
{
	return;
}

ULONG LanguageDefaultHook(struct LanguageContextHook *lch,APTR LanguageContext,APTR m)
{
	ULONG rc=0L, *Message=m;
	struct PerceptionIFace *IPerception=lch->PerceptionLib;
	struct UtilityIFace *IUtility=lch->UtilityLib;
	struct TagItem *Vector=NULL, *rBuffer=NULL, VCommand;

	if(LanguageContext)
	{
		VCommand.ti_Tag=LCSTATE_VECTOR;
		VCommand.ti_Data=0L;
		Vector=(APTR)IPerception->GetLanguageContextAttr(LanguageContext,(APTR)&VCommand);
		if(Vector)
			rBuffer=IUtility->FindTagItem(LCSTATE_CHORDBUFF,Vector);
	};

	switch(Message[0])
	{
		case LANGUAGE_TRANSLATE_AMIGA:
			KDEBUG("Perception-IME//English.Language [%lx:%lx]\n", Message[1], Message[3]);
			break;
		case LANGUAGE_TRANSLATE_ANSI:
			KDEBUG("Perception-IME//English.Language [%lx:%lx]\n", Message[1], Message[3]);
			break;
		default:
			KDEBUG("Perception-IME//English.Language [%lx:%lx] Unknown Method:Data\n",
				Message[0], Message[1], Message[2], Message[3]);
			break;
	}

	return(rc);
}

/**/
