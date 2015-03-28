/*
 * iesnoop -- Input Event snooper
 *
 * This program snoops the input event handler chain at the
 * priority specified.
 */

#include <devices/input.h>

#include <proto/dos.h>
#include <proto/exec.h>


typedef struct InputEvent* (*SNOOPFUNC)(struct InputEvent*, APTR);


struct IOStdReq* addHandler(int8 priority, SNOOPFUNC snoopfunc);
void removeHandler(struct IOStdReq* ioreq);
struct InputEvent* snoop(struct InputEvent* chain, APTR dummy);
struct Window* getConsoleWindow();


int main(int argc, char** argv)
{
	int32 priority = 10;

	if (argc > 1)
	{
		(void)IDOS->StrToLong(argv[1], &priority);
	}

	struct IOStdReq* req = addHandler(priority, snoop);
	if ( req == 0 ) {
		IDOS->Printf("addHandler failed\n");
		return RETURN_FAIL;
	}

	IDOS->Printf("InputEvent snoop installed at priority %ld\n", priority);

	struct Window *win = getConsoleWindow();
	IDOS->Printf("Console Window pointer=%p\n", win);
	IExec->DebugPrintF("Console Window pointer=%p\n", win);

	IDOS->Printf("Press CTRL-C to quit\n");
	IExec->Wait(SIGBREAKF_CTRL_C);

	removeHandler(req);

	return RETURN_OK;
}


struct InputEvent* snoop(struct InputEvent* chain, APTR dummy UNUSED)
{
	struct InputEvent* ie = chain;
	while (ie != 0) {
		if (ie->ie_Class == IECLASS_RAWKEY)
		{
			IExec->DebugPrintF("ie_Class=%p ie_SubClass=%p ie_Code=%p ie_Qualifier=%p ie_EventAddress=%p\n",
				ie->ie_Class, ie->ie_SubClass, ie->ie_Code, ie->ie_Qualifier, ie->ie_EventAddress);
		}

		ie = ie->ie_NextEvent;
	}

	return chain; // We never modify anything in the input event chain.
}


struct IOStdReq* addHandler(int8 priority, SNOOPFUNC snoopfunc)
{
	struct MsgPort* io_port = IExec->AllocSysObjectTags(ASOT_PORT, TAG_END);

	if (io_port != 0) {
		struct IOStdReq* io_req = IExec->AllocSysObjectTags(ASOT_IOREQUEST,
			ASOIOR_Size, sizeof(struct IOStdReq),
			ASOIOR_ReplyPort, io_port,
			TAG_END);

		if (io_req != 0) {
			struct Interrupt* interrupt = IExec->AllocSysObjectTags(ASOT_INTERRUPT,
				ASOINTR_Code, snoopfunc,
				ASOINTR_Data, 0,
				TAG_END);

			if (interrupt != 0) {
				interrupt->is_Node.ln_Type = NT_UNKNOWN;
				interrupt->is_Node.ln_Name = "input event snoop";
				interrupt->is_Node.ln_Pri  = priority;

				int8 error = IExec->OpenDevice("input.device", 0, (struct IORequest*)io_req, 0);

				if (error == IOERR_SUCCESS) {
					io_req->io_Command = IND_ADDHANDLER;
					io_req->io_Data    = interrupt;

					error = IExec->DoIO((struct IORequest*)io_req);
					if (error == IOERR_SUCCESS) {
						return io_req;
					}

					IExec->CloseDevice((struct IORequest*)io_req);
				}

				IExec->FreeSysObject(ASOT_INTERRUPT, interrupt);
				interrupt = 0;
			}

			IExec->FreeSysObject(ASOT_IOREQUEST, io_req);
			io_req = 0;
		}

		IExec->FreeSysObject(ASOT_PORT, io_port);
		io_port = 0;
	}

	return 0;
}


void removeHandler(struct IOStdReq* ioreq)
{
	if (ioreq != 0) {
		ioreq->io_Command = IND_REMHANDLER;
		(void)IExec->DoIO((struct IORequest*)ioreq);

		IExec->CloseDevice((struct IORequest*)ioreq);

		IExec->FreeSysObject(ASOT_PORT, ioreq->io_Message.mn_ReplyPort);
		IExec->FreeSysObject(ASOT_INTERRUPT, ioreq->io_Data);
		IExec->FreeSysObject(ASOT_IOREQUEST, ioreq);
	}
}


struct Window* getConsoleWindow()
{
	struct Window* window = 0;

	struct DosPacket64* packet = IDOS->AllocDosObjectTags(DOS_STDPKT, TAG_END);

	if (packet != 0) {
		struct MsgPort* con_port = IDOS->GetConsolePort();

		if (con_port != 0) {
			struct ConsoleWindowData data;

			int32 result = IDOS->DoPkt(con_port, ACTION_OBTAIN_CON_INFO,
				(int32)&data, sizeof(struct ConsoleWindowData), 0, 0, 0);

			if (result != FALSE) {
				(void)IDOS->DoPkt(con_port, ACTION_RELEASE_CON_INFO,
					(int32)&data, sizeof(struct ConsoleWindowData), 0, 0, 0);

				window = data.ConsoleWindow;
			}
		}

		IDOS->FreeDosObject(DOS_STDPKT, packet);
		packet =0;
	}

	return window;
}
